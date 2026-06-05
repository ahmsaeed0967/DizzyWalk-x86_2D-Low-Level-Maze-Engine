# 🎮 Dizzy Walk — 2D Maze Simulation in x86 Assembly

<div align="center">

![COAL](https://img.shields.io/badge/Language-x86%20Assembly-red?style=for-the-badge&logo=assemblyscript)
![Irvine32](https://img.shields.io/badge/Library-Irvine32-blue?style=for-the-badge)
![DOSBox](https://img.shields.io/badge/Platform-Windows%20Console-green?style=for-the-badge&logo=windows)
![Semester](https://img.shields.io/badge/Semester-Spring%202026-orange?style=for-the-badge)
![Course](https://img.shields.io/badge/Course-COAL-purple?style=for-the-badge)

**A fully interactive 2D maze adventure game built entirely in x86 Assembly Language.**
*Guide Professor Dizzy through a dangerous world of pits, coins, walls, lakes, and buildings — before he gets lost forever.*

</div>

---

## 📖 Project Overview

**Dizzy Walk** is a semester project for the *Computer Organization and Assembly Language (COAL)* course, Spring 2026.

The game simulates **Professor Dizzy** — a hilariously disoriented character who wanders through a colorful 2D maze filled with obstacles, coins, stumble zones, and a destination he desperately needs to reach. Players can either watch the professor walk randomly (Auto-Pilot mode) or take full keyboard control and navigate the maze themselves.

Everything — the game logic, the user interface, the file I/O, the sound effects, and the random movement engine — is written in **pure x86 Assembly** using the **Irvine32 library**.

> 💡 **Why this project?** It demonstrates how low-level assembly concepts like registers, interrupts, procedures, loops, and memory addressing can be combined to build a real, playable game.

---

## 📸 Project Preview

| Screen | Description |
|--------|-------------|
| ![Welcome Screen](Game%20Screens%20Images/screen1.png) | **Welcome Screen** — Intro banner with team credits |
| ![Retro Splash](Game%20Screens%20Images/Screen2.png) | **Retro Arcade Splash** — Animated start screen |
| ![Game Mode Menu](Game%20Screens%20Images/screen3.png) | **Mission Select Menu** — Choose your play mode |
| ![Maze Gameplay](Game%20Screens%20Images/screen4.png) | **Live Maze** — Professor navigating the 50×30 grid |
| ![Status Panel](Game%20Screens%20Images/screen5.png) | **Status Panel** — Real-time HUD with stats and 3D view |
| ![Pit Danger](Game%20Screens%20Images/screen6.png) | **Danger Warning** — Pit proximity alert |
| ![Win Screen](Game%20Screens%20Images/screen7.png) | **Victory Screen** — Professor reached home! |
| ![Game Over](Game%20Screens%20Images/screen8.png) | **Game Over Screen** — The professor fell into a pit |

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🗺️ **2D Maze Grid** | A 50-column × 30-row console maze with walls, lakes, buildings, pits, stumble zones, coins, and a destination |
| 🤖 **Auto-Pilot Mode** | Professor moves randomly in any of 4 directions, picked by a random number generator |
| ⌨️ **Keyboard Mode** | Full player control using W/A/S/D keys to guide the professor manually |
| 🎯 **4 Game Modes** | Auto-Pilot (Limited Steps), Auto-Pilot (Endless), Manual Walk (Limited Steps), Manual Walk (Endless) |
| 💰 **Coin Collection** | 8 coins scattered across the maze; picking one increases the professor's wallet and plays a sound |
| 🕳️ **Pit Hazards** | 4 deadly pits trigger immediate Game Over if stepped on |
| 🤸 **Stumble Zones** | 3 special tiles where the professor trips and drops his key |
| 🔑 **Key Mechanic** | Professor carries a key to enter his home; losing it via stumble changes the ending |
| 📊 **Live HUD Panel** | Right-side panel showing current step count, wallet balance, key status, movement mode, and run type |
| 🧭 **Pseudo-3D View** | A stylized 3D corridor rendering panel shows what object lies directly ahead of the professor |
| ⚠️ **Pit Proximity Warning** | A danger alert fires when any of the 4 neighboring cells contains a pit |
| 🎵 **Sound Effects** | Distinct beep tones for coin collection, victory, and game over events |
| 🎬 **Scene Transitions** | Animated loading effects between screens using alternating colors |
| 🏁 **Dynamic End Screen** | Win (green banner) or Lose (red banner) with final score, coins, and steps displayed |
| 📝 **Adventure Log** | Full run history saved to `adventure_log.txt`, including path coordinates and coin pickup locations |
| 🎨 **Color-Coded Map** | Every cell type has a distinct console color for easy visual recognition |
| 🕹️ **Retro Splash Screen** | Arcade-style animated intro with score, palm trees, clouds, and coin icons |

---

## 🛠️ Technologies Used

| Technology | Purpose |
|------------|---------|
| **x86 Assembly (MASM)** | Entire game logic, UI, I/O, and control flow |
| **Irvine32 Library** | Console I/O (`WriteString`, `ReadChar`, `Gotoxy`, `SetTextColor`, `Clrscr`, `Delay`, `Randomize`, `RandomRange`) |
| **Windows API — `Beep`** | Generating audio tones for game events (coin, win, game over) |
| **Windows Console** | Colored character-based rendering for the 2D maze and all panels |
| **File I/O (Win32)** | Writing the adventure log to disk using `CreateFileA` / `WriteFile` |
| **Visual Studio / MASM** | Assembly and linking environment |

---

## 📚 COAL Concepts Implemented

| Concept | Usage in This Project |
|---------|----------------------|
| **Registers** | `EAX`, `EBX`, `ECX`, `EDX`, `ESI`, `EDI` used for row/col math, loop counters, character comparisons, color values, and return values throughout every procedure |
| **Arithmetic Instructions** | `imul` for index calculation (`row × MAZE_COLS + col`), `inc`/`dec` for movement and loop stepping, `add`/`sub` for pointer arithmetic |
| **Conditional Jumps** | `je`, `jne`, `jl`, `jge`, `jg` control all game logic branches — movement bounds, cell type checks, win/lose conditions |
| **Unconditional Jumps** | `jmp` used for looping constructs, fallthrough bypasses, and game state transitions |
| **Procedures (PROC/ENDP)** | 30+ modular procedures each handling a single responsibility (see Project Structure) |
| **Stack Operations** | `push`/`pop` and `pushad`/`popad` used in every procedure to preserve caller registers |
| **Macros** | `SetColor MACRO attr` — a reusable macro that safely wraps `SetTextColor` while preserving `EAX` |
| **Memory Addressing** | Direct offset addressing (`OFFSET mazeGrid`, `OFFSET msgWelcome`), indexed addressing via `ESI`/`EDI` for maze cell read/write |
| **Data Segment** | `.data` section declares all variables: maze grid, coin/pit/stumble arrays, path history, message strings, file buffers |
| **Arrays** | `mazeGrid` (1500-byte flat array), `pathRow`/`pathCol` (500-entry step log), `coinRow`/`coinCol`/`coinActive` (8-entry coin table), `pitRow`/`pitCol`, `stumbleRow`/`stumbleCol` |
| **EQU Constants** | Symbolic constants for maze dimensions (`MAZE_ROWS`, `MAZE_COLS`), object characters (`CHAR_PIT`, `CHAR_COIN`, `CHAR_WALL`), colors, and directions |
| **DUP Operator** | `DWORD MAX_STEPS DUP(0)` and `BYTE MAZE_ROWS * MAZE_COLS DUP(CHAR_EMPTY)` for initializing large arrays |
| **Loops (`LOOP` + manual)** | `loop` instruction used in header/banner drawing; `cmp`+`jl`+`jmp` patterns used in maze rendering and forward-scan loops |
| **String Operations** | `WriteString` via `EDX` pointer; custom `AppendStr`/`AppendDec` procedures for building the adventure log buffer |
| **Interrupts / API Calls** | `call ExitProcess` (program termination), `PROTO`-declared `Beep` Windows API, Irvine32 I/O routines acting as interrupt wrappers |
| **Random Number Generation** | `call Randomize` seeds the generator; `call RandomRange` with `EAX=4` picks a random direction (0–3) for auto-pilot movement |
| **File I/O** | `CreateFileA`, `WriteFile`, `CloseHandle` via Irvine32/Win32 to persist the adventure log to disk |
| **Flag Register** | `cmp` sets flags tested by all conditional jumps; `test eax, 1` used for odd/even alternation in scene transitions |
| **Modular Code Organization** | Three-file structure: `Main.asm` (entry point), `Variables.inc` (data), `DizzyWalk_Procedures.inc` (all procedures) |

---

## 📁 Project Structure

```
DizzyWalk-Assembly-Game/
│
├── Main.asm                     ← Entry point; main game loop
├── Variables.inc                ← All constants, data segment, message strings
├── DizzyWalk_Procedures.inc     ← All 30+ procedures (game logic, rendering, I/O)
├── adventure_log.txt            ← Auto-generated run history (created on game exit)
│
└── Game Screens Images/
    ├── screen1.png              ← Welcome / Credits screen
    ├── Screen2.png              ← Retro arcade splash
    ├── screen3.png              ← Mission select menu
    ├── screen4.png              ← Live maze gameplay
    ├── screen5.png              ← Status and 3D panel
    ├── screen6.png              ← Pit danger warning
    ├── screen7.png              ← Win screen
    └── screen8.png              ← Game over screen
```

---

## 🕹️ Controls

| Key | Action |
|-----|--------|
| `W` | Move Professor **Up** |
| `S` | Move Professor **Down** |
| `A` | Move Professor **Left** |
| `D` | Move Professor **Right** |
| `R` | Switch to **Random (Auto-Pilot)** mode mid-game |
| `Q` | **Quit** the simulation immediately |
| `Any Key` | Advance through intro screens and splash |
| `1` | Select Auto-Pilot + Limited Steps mode |
| `2` | Select Auto-Pilot + Endless mode |
| `3` | Select Manual Walk + Limited Steps mode |
| `4` | Select Manual Walk + Endless mode |

---

## 🗺️ Map Legend

| Symbol | Color | Meaning |
|--------|-------|---------|
| `P` | 🟡 Yellow | Professor Dizzy (the player character) |
| `D` | 🔵 Cyan | Destination / Home Door |
| `$` | 🟢 Green | Coin (collect for score) |
| `O` | 🔴 Red | Pit (instant game over) |
| `#` | ⚫ Dark Gray | Wall (impassable) |
| `~` | 🔵 Teal | Lake (navigable terrain) |
| `B` | 🟠 Orange | Building (navigable terrain) |
| `S` | 🟣 Magenta | Stumble zone (drops key on contact) |
| `*` | 🔵 Blue | Trail (cells already visited) |
| `.` | White | Empty path |

---

## ⚙️ How It Works — Step by Step

1. **Launch** — The program starts, clears the screen, and shows the Welcome banner with team credits.
2. **Retro Splash** — An animated arcade-style intro screen plays, showing a score bar, palm trees, clouds, and coin icons.
3. **Scene Transition** — An animated color-wipe effect transitions between screens.
4. **Mission Select** — The player chooses one of 4 game modes from a styled menu box.
5. **Maze Initialization** — `InitMaze` places all objects into the 50×30 flat array: walls (border + interior), 8 coins, 4 pits, 3 stumble zones, lakes, buildings, and the destination cell.
6. **Professor Spawns** — Professor Dizzy starts at the center of the maze (row 15, col 25).
7. **Game Loop Begins** — Each iteration of the main loop:
   - Checks if `gameOver` flag is set → exits loop if true
   - Checks if the step limit has been reached (in Limited mode) → ends game if exceeded
   - Records the current position to the path history arrays
   - Calls either `DoRandomMove` (auto-pilot) or `DoKeyboardMove` (manual) to move the professor
   - Calls `CheckCell` to evaluate what the professor just stepped on (coin, pit, stumble, destination)
   - Calls `DrawMaze` to repaint the entire console display
8. **Cell Events:**
   - **Coin** → wallet increases, coin removed from map, `Beep_Coin` plays
   - **Pit** → `Beep_GameOver` plays, game ends with "Fell into a pit" reason
   - **Stumble** → key is dropped (affects ending outcome)
   - **Destination (with key)** → `Beep_Win` plays, game ends with WIN
   - **Destination (without key)** → game ends but professor sleeps outside (LOSE)
9. **Adventure Log** — `SaveAdventureLog` builds a text buffer containing the full run summary (name, mode, steps, path coordinates, coin pickups, end reason) and writes it to `adventure_log.txt`.
10. **End Screen** — A full-color banner (green for WIN, red for LOSE) shows the final reason, score (wallet × 25), coin count, and step count.

---

## 🔧 Installation & Execution

### Prerequisites

- **Microsoft Visual Studio** with MASM (or standalone MASM 6.x+)
- **Irvine32 Library** installed and linked
- **Windows OS** (the game uses Windows console APIs)

### Steps

```bash
# Step 1: Clone or extract the project folder
# Place all files in the same directory

# Step 2: Open Visual Studio Developer Command Prompt

# Step 3: Assemble the main file
ml /c /coff /Zi Main.asm

# Step 4: Link with Irvine32
link /subsystem:console Main.obj Irvine32.lib kernel32.lib user32.lib

# Step 5: Run the executable
Main.exe
```

> ⚠️ **Note:** Ensure `Irvine32.inc`, `Irvine32.lib`, and all `.inc` files are in the same directory as `Main.asm`, or update your include/library paths in the project settings.

> 💡 **Using Visual Studio IDE?** Create a new MASM project, add all three files (`Main.asm`, `Variables.inc`, `DizzyWalk_Procedures.inc`) to the project, link Irvine32, and build with F7.

---

## 🏗️ Technical Internals

### Maze Representation
The maze is stored as a **flat 1D byte array** (`mazeGrid`, 1500 bytes = 30 rows × 50 cols). Any cell at position `(row, col)` is accessed via the formula:

```
index = (row × 50) + col
```

The `GetMazeCell` and `SetMazeCell` procedures handle all reads and writes using `ESI`/`EDI` pointer arithmetic.

### Movement Engine
- **Auto-Pilot:** `RandomRange` generates a number 0–3, mapped to UP/DOWN/LEFT/RIGHT. Each direction procedure checks maze boundaries and wall collisions before updating `profRow`/`profCol`.
- **Keyboard:** `ReadChar` captures a single keypress; lowercase letters are normalized to uppercase by subtracting 32. Each valid key calls the matching move procedure and updates `facingDir` for the 3D panel.

### Collision Detection
`CheckCell` reads the character at the professor's new position after every move and branches on the result using `cmp`/`je` chains:
- `CHAR_COIN` → collect and deactivate
- `CHAR_PIT` → trigger game over
- `CHAR_STUMBLE` → drop key
- `CHAR_DEST` → check key status, end game

### Pseudo-3D View
`ScanForwardObject` scans up to 5 cells ahead in the current `facingDir`. The first non-empty, non-trail cell found is recorded in `viewObjChar` and `viewObjDist`, and displayed in the status panel alongside a static ASCII corridor frame.

### File I/O
`SaveAdventureLog` assembles a complete text report into a 4096-byte buffer using `AppendStr` and `AppendDec` helper procedures, then writes it to disk using Win32 `CreateFileA` and `WriteFile` calls.

---

## 🧩 Procedure Reference

| Procedure | Responsibility |
|-----------|---------------|
| `GetMazeCell` | Read a character from the maze grid at (row, col) |
| `SetMazeCell` | Write a character to the maze grid at (row, col) |
| `InitMaze` | Initialize all maze objects at their fixed positions |
| `DrawMaze` | Repaint the full console: maze + all UI panels |
| `DrawHeaderPanel` | Draw the top HUD bar with title and step info |
| `DrawStatusPanel` | Draw the right-side live status box |
| `DrawLegendPanel` | Draw the bottom controls/legend reference box |
| `MoveUp / MoveDown / MoveLeft / MoveRight` | Move professor one cell; handle bounds and wall collisions |
| `DoRandomMove` | Pick a random direction and call the matching move procedure |
| `DoKeyboardMove` | Read a keypress and map it to movement or mode change |
| `CheckCell` | Evaluate the cell the professor just stepped on |
| `RecordStep` | Log current position to `pathRow`/`pathCol` arrays |
| `IsAdjacentToPit` | Scan 4 neighbors for a pit; return 1 if danger nearby |
| `ScanForwardObject` | Look ahead up to 5 cells in facing direction |
| `GetRandomDir` | Return a random number 0–3 for direction selection |
| `SaveAdventureLog` | Build and write the adventure log file to disk |
| `AppendStr / AppendDec` | Helper procedures for assembling the log buffer |
| `ShowEndScreen` | Render the final WIN or LOSE screen with stats |
| `SceneTransition` | Display an animated loading/wipe effect between screens |
| `DrawRetroSplash` | Render the animated arcade-style intro splash screen |
| `SetupGameOptions` | Display the mission select menu and configure game variables |
| `Beep_Coin / Beep_Win / Beep_GameOver` | Play distinct audio tones for key game events |

---

## 🎯 Game Ending Conditions

| Condition | Outcome |
|-----------|---------|
| Professor reaches Destination **with key** | ✅ **WIN** — Entered home successfully |
| Professor reaches Destination **without key** | ❌ **LOSE** — Sleeping outside (key was lost at a stumble tile) |
| Professor steps on a **Pit** | ❌ **LOSE** — Fell into a pit |
| **Step limit** reached (in Limited mode) | ❌ **LOSE** — Professor gave up |
| Player presses **Q** | ❌ **QUIT** — Simulation ended by user |

---

## 📋 Adventure Log Sample

After every session, `adventure_log.txt` is written automatically:

```
===== DIZZY WALK ADVENTURE LOG =====

Professor Name: Professor Dizzy
Run Type: Endless until destination/criteria
Step Limit: 500
  Wallet: 1
  Key: YES
Step: 43
End Reason: Fell into pit

Path (row,col) entries:
(15, 25)
(14, 25)
...

Treasures (coins) picked at:
```

<div align="center">

*"Even the dizziest professor can find his way home — with a little assembly."* 🎓

</div>
