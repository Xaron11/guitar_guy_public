import sys
import os
from mido import MidiFile, tempo2bpm

# Chart resolution (ticks per quarter note) used in .chart files:
CHART_PPQ = 192

def midi_tick_to_chart_tick(midi_tick, midi_ppq):
    """
    Convert a MIDI tick (at midi_ppq PPQ) to a chart tick (at CHART_PPQ).
    """
    return int(midi_tick * CHART_PPQ / midi_ppq)

def extract_tempo(midi):
    """
    Return the first tempo (in μs per quarter note) found in the MIDI.
    If none is found, default to 120 BPM (500_000 μs per quarter note).
    """
    for track in midi.tracks:
        for msg in track:
            if msg.type == 'set_tempo':
                return msg.tempo
    return 500_000  # default = 120 BPM

def collect_note_events(midi):
    """
    Traverse all tracks, collect (absolute_tick, note_number) for each note_on with velocity > 0.
    Returns a list of (tick, note) tuples.
    """
    events = []
    for track in midi.tracks:
        abs_tick = 0
        for msg in track:
            abs_tick += msg.time
            if msg.type == 'note_on' and msg.velocity > 0:
                events.append((abs_tick, msg.note))
    return events

def generate_chart_sections(midi_path, midi):
    """
    Build the [Song] and [Track] sections as strings.
    """
    base = os.path.splitext(os.path.basename(midi_path))[0]
    # file format: name - artist.mid
    name = base.split(" - ")[0] if " - " in base else base
    artist = base.split(" - ")[1] if " - " in base else "Unknown Artist"
    offset    = 0
    resolution= CHART_PPQ
    difficulty= 1

    # Extract first tempo
    mpqn = extract_tempo(midi)
    bpm  = tempo2bpm(mpqn)

    # Collect all note_on events
    midi_ppq = midi.ticks_per_beat
    raw_events = collect_note_events(midi)


    skip_interval = 6 - difficulty

    # Map every note → one of four lanes (0..3) via (note % 4)
    chart_events = []
    for i, (tick, note) in enumerate(raw_events):
        if i % skip_interval != 0:
            continue
        fret = note % 4
        chart_tick = midi_tick_to_chart_tick(tick, midi_ppq)
        chart_events.append((chart_tick, fret))

    # Sort by tick ascending
    chart_events.sort(key=lambda x: x[0])


    # [Song] section
    song_lines = [
        "[Song]",
        f"Name = \"{name}\"",
        f"Artist = \"{artist}\"",
        f"Offset = {offset}",
        f"Resolution = {resolution}",
        f"Difficulty = {difficulty}",
        f"Tempo = {mpqn}",
        f"Notes = {len(chart_events)}",
        ""
    ]

    # [Track] section
    track_lines = ["[Track]"]
    for ctick, fret in chart_events:
        track_lines.append(f"{ctick} = N {fret}")
    track_lines.append("")

    return "\n".join(song_lines), "\n".join(track_lines)

def write_chart(output_path, song_section, track_section):
    """
    Write the assembled sections into the output .chart file.
    """
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(song_section + "\n")
        f.write(track_section + "\n")

def main():
    if len(sys.argv) != 2:
        print("Usage: python map_creator.py input.mid")
        sys.exit(1)

    midi_path  = f'./{sys.argv[1]}'
    chart_path = f'./output/{os.path.splitext(os.path.basename(midi_path))[0]}.chart'

    if not os.path.isfile(midi_path):
        print(f"Error: MIDI file '{midi_path}' not found.")
        sys.exit(1)

    try:
        midi = MidiFile(midi_path)
    except Exception as e:
        print(f"Error: Could not open MIDI file: {e}")
        sys.exit(1)

    song_sec, track_sec = generate_chart_sections(midi_path, midi)
    write_chart(chart_path, song_sec, track_sec)
    print(f"Successfully wrote 4-lane .chart to '{chart_path}'")

if __name__ == "__main__":
    main()