# GUITAR GUY

## Description

This is a simple Guitar Hero-style rhythm game built using [raylib](https://www.raylib.com/) in C. It supports a basic note map format, four key inputs, scoring, visual feedback, and displays the song title and artist.

## 🎮 Features

- 4 playable columns: `A`, `S`, `D`, `F` keys
- Color-coded note columns: green, red, blue, yellow
- Smooth note fall and hit detection
- Anti-spam scoring logic (prevents holding down keys)
- Visual feedback for button presses and successful hits
- Song metadata display (title + artist)
- Simple map parser from `.chart` file
- Uses `raylib` for graphics

## 📁 Map File Format
Place map files in the `assets/maps` directory. Maps use the following `.chart` format:

```
[Song]
Name = "Song Title"
Artist = "Artist Name"
Offset = 0
Resolution = 192
Difficulty = 1
Tempo = 500000
Notes = 262

[Track]
0 = N 0
576 = N 1
768 = N 3
1056 = N 2
...
```
- `Offset`: Seconds to delay before the first note.
- `Resolution`: Ticks per beat (like MIDI PPQ).
- `Tempo`: Microseconds per beat.
- `Track` entries define notes by tick and column (0-3).

## 🛠️ Build Instructions

This project uses **CMake**. You must have `raylib` installed or allow CMake to fetch it automatically.

### 🔧 Requirements

- C compiler (e.g. GCC, Clang)
- CMake >= 3.16
- GNU Make / Ninja
- raylib (will be fetched automatically if not found)

### Project Dependencies
- raylib
- raygui

### 🧱 Building

#### Instructions

```bash
git clone https://github.com/Xaron11/guitar_guy_public.git
cd guitar_guy
mkdir build && cd build
cmake ..
cmake --build .
```
The game executable will be in `build/bin/`.

#### Run
```bash
cd bin
./guitar_guy
```

## ⌨️ Controls
| Key | Action        |
| --- | ------------- |
| A   | Green column  |
| S   | Red column    |
| D   | Blue column   |
| F   | Yellow column |
| ESC | Pause/Exit    |
