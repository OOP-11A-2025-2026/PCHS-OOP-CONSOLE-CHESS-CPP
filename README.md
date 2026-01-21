# Console Chess C++

Another console chess app made by dummies

## 🛠 Build & Run (Quick Guide)

### Requirements
* C++ compiler with C++17 support
* CMake 3.15+

#### macOS

```bash
brew install cmake
```

#### Linux (Ubuntu/Debian)

```bash
sudo apt install build-essential cmake
```

#### Windows
* Install Visual Studio (C++ workload) or MinGW
* Install CMake and add it to PATH

### 🔨 Build
From the project root:

```bash
cmake -S . -B build
cmake --build build
```

This creates the executable inside the `build/` folder.

### ▶ Run

```bash
./build/chess
```

(On Windows)

```cmd
build\chess.exe
```

### 🔁 Clean Build (if something breaks)

```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

### 🧪 Tests
The current `main.cpp` runs built-in tests automatically.
Just run:

```bash
./build/chess
```

### 💡 Notes
* Uses C++17 (`std::optional`, etc.)
* All headers are in `include/`
* All source files are in `src/`
* Built using CMake

## 💻 Development Setup (VS Code)

### Install Extensions
* C/C++ (Microsoft)
* CMake Tools (Microsoft)

## 📁 Project Structure

```
console-chess-cpp/
├── CMakeLists.txt           # Build configuration
├── README.md                # This file
│
├── include/                 # Header files (.h)
│   ├── board/
│   │   ├── Board.h
│   │   ├── Square.h
│   │   ├── Move.h
│   │   └── PawnPromotionHandler.h
│   │
│   ├── pieces/
│   │   ├── Piece.h
│   │   ├── Pawn.h
│   │   ├── Rook.h
│   │   ├── Knight.h
│   │   ├── Bishop.h
│   │   ├── Queen.h
│   │   └── King.h
│   │
│   ├── enums/
│   │   ├── Color.h
│   │   └── PieceType.h
│   │
│   ├── game/
│   │   └── Game.h
│   │
│   ├── cli/
│   │   ├── ChessCLI.h
│   │   ├── BoardPrinter.h
│   │   └── PieceRenderer.h
│   │
│   ├── input/
│   │   └── MoveParser.h
│   │
│   ├── timer/
│   │   └── GameTimer.h
│   │
│   └── pgn/
│       ├── PGNParser.h
│       └── PGNExporter.h
│
├── src/                     # Implementation files (.cpp)
│   ├── main.cpp
│   │
│   ├── board/
│   │   ├── Board.cpp
│   │   ├── Square.cpp
│   │   ├── Move.cpp
│   │   └── PawnPromotionHandler.cpp
│   │
│   ├── pieces/
│   │   ├── Piece.cpp
│   │   ├── Pawn.cpp
│   │   ├── Rook.cpp
│   │   ├── Knight.cpp
│   │   ├── Bishop.cpp
│   │   ├── Queen.cpp
│   │   └── King.cpp
│   │
│   ├── game/
│   │   └── Game.cpp
│   │
│   ├── cli/
│   │   ├── ChessCLI.cpp
│   │   ├── BoardPrinter.cpp
│   │   └── PieceRenderer.cpp
│   │
│   ├── input/
│   │   └── MoveParser.cpp
│   │
│   ├── timer/
│   │   └── GameTimer.cpp
│   │
│   └── pgn/
│       ├── PGNParser.cpp
│       └── PGNExporter.cpp
│
└── build/                   # Build artifacts (generated)
```

## 👥 Contributors

- Dilyana Vasileva
- Darian Ganev
- Yordan Tsonev
- Venelin Jelev