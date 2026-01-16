# Console Chess C++

A modern chess engine implementation in C++ with a command-line interface.

## 📋 Table of Contents

- [Requirements](#requirements)
- [Project Structure](#project-structure)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [Development Setup](#development-setup)
- [Testing](#testing)
- [Troubleshooting](#troubleshooting)

## 🔧 Requirements

### System Requirements

- **Operating System**: macOS, Linux, or Windows
- **C++ Compiler**: 
  - macOS: Clang (comes with Xcode Command Line Tools)
  - Linux: GCC 7.0+ or Clang 5.0+
  - Windows: MSVC 2017+, MinGW, or Clang
- **CMake**: Version 3.15 or higher
- **C++ Standard**: C++17 or higher

### Installing Requirements

#### macOS

```bash
# Install Xcode Command Line Tools (includes Clang)
xcode-select --install

# Install CMake (using Homebrew)
brew install cmake
```

#### Linux (Ubuntu/Debian)

```bash
# Install build essentials and CMake
sudo apt update
sudo apt install build-essential cmake
```

#### Linux (Fedora/RHEL)

```bash
# Install development tools and CMake
sudo dnf groupinstall "Development Tools"
sudo dnf install cmake
```

#### Windows

1. Install [Visual Studio 2017+](https://visualstudio.microsoft.com/) with C++ support, OR
2. Install [MinGW-w64](https://www.mingw-w64.org/)
3. Install [CMake](https://cmake.org/download/) and add to PATH

### Verifying Installation

```bash
# Check C++ compiler
g++ --version      # For GCC
clang++ --version  # For Clang

# Check CMake
cmake --version
```

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

## 🔨 Building the Project

### Quick Build (Recommended)

```bash
# Navigate to project directory
cd /path/to/console-chess-cpp

# Configure CMake (creates build files)
cmake -S . -B build

# Build the project
cmake --build build

# Run the executable
./build/chess
```

### Step-by-Step Build

#### 1. Clone or Navigate to Project

```bash
cd /path/to/console-chess-cpp
```

#### 2. Create Build Directory

```bash
mkdir build
cd build
```

#### 3. Configure with CMake

```bash
# From inside build directory
cmake ..

# OR specify build type (Debug or Release)
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### 4. Compile the Project

**Using Make (macOS/Linux):**
```bash
make
```

**Using CMake (all platforms):**
```bash
cmake --build .
```

**Using Make with multiple cores (faster):**
```bash
make -j4  # Uses 4 cores
```

#### 5. Run the Executable

```bash
# From build directory
./chess

# OR from project root
./build/chess
```

### Clean Build (Start Fresh)

```bash
# Remove build directory
rm -rf build

# Rebuild from scratch
mkdir build
cd build
cmake ..
make
```

## 🚀 Running the Application

### Basic Usage

```bash
./build/chess
```

### Expected Output (Current Test Version)

```
============================================
      CHESS ENGINE TEST SUITE
============================================

=== Testing Square Class ===

1. Creating squares:
   e4: e4 (file=4, rank=3)
   e2: e2
   a1: a1
   h8: h8

[... additional test output ...]

============================================
      ALL TESTS COMPLETED SUCCESSFULLY!
============================================
```

## 💻 Development Setup

### VS Code Setup

1. **Install Extensions:**
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)
   - C/C++ Extension Pack

2. **Configure IntelliSense:**

Create `.vscode/c_cpp_properties.json`:

```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/include",
                "${workspaceFolder}/include/**"
            ],
            "defines": [],
            "macFrameworkPath": [],
            "compilerPath": "/usr/bin/clang++",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "macos-clang-arm64",
            "compileCommands": "${workspaceFolder}/build/compile_commands.json"
        }
    ],
    "version": 4
}
```

**Note:** Change `"intelliSenseMode"` based on your system:
- Apple Silicon Mac: `"macos-clang-arm64"`
- Intel Mac: `"macos-clang-x64"`
- Linux: `"linux-gcc-x64"` or `"linux-clang-x64"`
- Windows: `"windows-msvc-x64"` or `"windows-gcc-x64"`

3. **Reload VS Code:**
   - Press `Cmd+Shift+P` (Mac) or `Ctrl+Shift+P` (Windows/Linux)
   - Type "Developer: Reload Window"

### Build Configurations

#### Debug Build (for development)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Debug builds include:
- Debug symbols
- No optimizations
- Better error messages
- Slower execution

#### Release Build (for production)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Release builds include:
- No debug symbols
- Full optimizations (-O3)
- Faster execution
- Smaller binary size

## 🧪 Testing

### Running Tests

The current `main.cpp` contains comprehensive tests for:
- Square class (creation, parsing, equality)
- Move class (regular moves, promotions)
- Enums (PieceType, Color)

```bash
./build/chess
```

### Adding New Tests

Edit `src/main.cpp` to add new test cases following the existing pattern.

## 🐛 Troubleshooting

### Common Issues and Solutions

#### 1. `cmake: command not found`

**Solution:** Install CMake
```bash
# macOS
brew install cmake

# Linux
sudo apt install cmake
```

#### 2. `No such file or directory: 'board/Square.h'`

**Solutions:**
- Verify file exists: `ls include/board/Square.h`
- Clean and rebuild: `rm -rf build && cmake -S . -B build && cmake --build build`
- Check VS Code IntelliSense configuration

#### 3. `std::optional` errors or C++17 features not recognized

**Solution:** Ensure CMakeLists.txt has:
```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

Then rebuild:
```bash
rm -rf build
cmake -S . -B build
cmake --build build
```

#### 4. VS Code shows red underlines but code compiles

**Solution:** 
1. Make sure `compile_commands.json` is generated
2. Update `.vscode/c_cpp_properties.json` (see Development Setup)
3. Reload VS Code window

#### 5. Permission denied when running executable

**Solution:**
```bash
chmod +x build/chess
./build/chess
```

#### 6. Build fails with "undefined reference" errors

**Solutions:**
- Ensure all `.cpp` files are in `src/` directory
- Check that CMakeLists.txt includes all source files
- Verify function implementations match declarations

### Getting Help

If you encounter other issues:

1. Check the CMake output for specific error messages
2. Verify all header files are in `include/` and implementation files in `src/`
3. Ensure file names match exactly (case-sensitive on macOS/Linux)
4. Try a clean build: `rm -rf build && cmake -S . -B build && cmake --build build`

## 📝 Notes

- **C++17 Required:** This project uses `std::optional` and other C++17 features
- **Header Organization:** All headers use paths relative to `include/` directory
- **Include Guards:** Headers use `#ifndef` guards (e.g., `#ifndef SQUARE_H`)
- **Const Correctness:** Getter methods are marked `const`
- **Memory Management:** Uses RAII and smart pointers where appropriate

## 📄 License

[Add your license information here]

## 👥 Contributors

[Add contributor information here]