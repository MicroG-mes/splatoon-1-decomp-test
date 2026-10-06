# Splatoon 1 (Wii U) Decompilation & Reverse Engineering

A matching decompilation and reverse-engineering repository for **Splatoon** on the Nintendo Wii U (internal project codename: **Gambit**).

---

## 🎯 Target Overview

- **Platform**: Nintendo Wii U (Cafe OS)
- **CPU**: IBM PowerPC 750CL derivative ("Espresso", 3 cores, 32-bit Big Endian)
- **Executable**: `Gambit.rpx` / `Gambit.elf`
- **Compiler**: Green Hills Software Multi C/C++ Compiler for Cafe (`cxppc`)
- **Underlying Engine**: Nintendo EAD `sead` framework + `agl` (Auto Graphics Library) + NintendoWare for Cafe (NW4F)

---

## 📁 Repository Structure

```text
splatoondecomp/
├── original/            # Place your dumped Gambit.rpx or Gambit.elf here
│   └── README.txt
├── src/                 # Reconstructed C++ source files
│   ├── Game/            # Splatoon game logic (Player, Enemy, Weapon, MapObj)
│   ├── CSystem/         # Core system managers
│   ├── sead/            # Nintendo EAD sead library implementation
│   └── agl/             # Nintendo agl graphics library
├── include/             # C++ header files
│   ├── types.h          # Standard Nintendo and decomp typedefs
│   ├── Game/            # Game headers (GamePlayer.h, etc.)
│   ├── sead/            # sead framework headers
│   └── cafe/            # Cafe OS / SDK headers
├── tools/               # Automation and analysis scripts
│   ├── verify_dump.py   # Verifies dumped RPX/ELF size, hash, and format
│   ├── extract_symbols.py # Recovers strings, class RTTI, and original file paths
│   └── setup_ghidra.py  # Assists with Ghidra & GhidraRPXLoader setup
├── CMakeLists.txt       # Build system for compiling reconstructed code
├── objdiff.json         # Configuration for the objdiff matching diff tool
└── README.md
```

---

## 🚀 Quick Start Guide

### Step 1: Provide the Game Executable
Copy your dumped `Gambit.rpx` from your Wii U dump into the `original/` folder:
```text
splatoondecomp/original/Gambit.rpx
```
*(On your Wii U dump, this is typically located inside `/vol/content/.../code/Gambit.rpx` or `code/Gambit.rpx`)*.

### Step 2: Verify Your Dump
Run the verification script to inspect your executable's hash, size, and version:
```powershell
python tools/verify_dump.py
```

### Step 3: Extract Strings and Source Layout
Nintendo Wii U binaries retain rich assertion strings, RTTI class names, and source file paths (e.g. `home/Cafe/Gambit/App/Program/Game/...`). Extract them with:
```powershell
python tools/extract_symbols.py
```
This generates:
- `tools/extracted/source_paths.txt` (All original source file references)
- `tools/extracted/classes.txt` (All RTTI class names and singletons)
- `tools/extracted/all_strings.txt` (Full printable string dump)

---

## 🔍 Static Analysis with Ghidra

1. **Install Java 17+ & Ghidra**:
   - OpenJDK: `winget install Microsoft.OpenJDK.21`
   - Ghidra: Download from [Ghidra Releases](https://github.com/NationalSecurityAgency/ghidra/releases) or `winget install NSA.Ghidra`.
2. **Install GhidraRPXLoader**:
   - Download the latest extension zip from [GhidraRPXLoader Releases](https://github.com/Maschell/GhidraRPXLoader/releases).
   - In Ghidra, open `File` -> `Install Extensions...`, click `+` and select the zip.
   - Restart Ghidra.
3. **Open the Game**:
   - Create a new project, select `File` -> `Import File...`, and choose `original/Gambit.rpx`.
   - Ghidra will automatically recognize it as a Cafe RPX binary.
   - Run Auto Analysis. Run the script `fix_primary_imports.java` to resolve Cafe SDK imports!

---

## ⚖️ Matching Assembly with objdiff

[objdiff](https://github.com/encounter/objdiff) is the modern standard for decompilation projects to compare decompiled C++ output against the original binary:

1. Download **objdiff** from [GitHub Releases](https://github.com/encounter/objdiff/releases).
2. Open objdiff and select this project directory `splatoondecomp`.
3. It will load `objdiff.json` and allow live function-by-function comparison as you decompile C++ functions in `src/`!
