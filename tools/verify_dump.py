#!/usr/bin/env python3
"""
Splatoon (Wii U) Binary Verifier
Checks the dumped executable (Gambit.rpx / Gambit.elf) and displays diagnostics.
"""

import os
import sys
import hashlib
from pathlib import Path

# Common locations to check for the executable
SEARCH_PATHS = [
    Path("original/Gambit.rpx"),
    Path("original/Gambit.elf"),
    Path("original/code/Gambit.rpx"),
    Path("Gambit.rpx"),
    Path("code/Gambit.rpx"),
]

def calculate_hashes(file_path):
    sha1 = hashlib.sha1()
    md5 = hashlib.md5()
    with open(file_path, "rb") as f:
        while chunk := f.read(1024 * 1024):
            sha1.update(chunk)
            md5.update(chunk)
    return sha1.hexdigest(), md5.hexdigest()

def inspect_binary(file_path):
    size = os.path.getsize(file_path)
    sha1, md5 = calculate_hashes(file_path)

    with open(file_path, "rb") as f:
        magic = f.read(4)

    is_elf = magic == b"\x7fELF"
    
    print("\n" + "=" * 60)
    print(" [Splatoon 1 / Gambit Binary Verification]")
    print("=" * 60)
    print(f"File Path : {file_path.resolve()}")
    print(f"File Size : {size:,} bytes ({size / (1024*1024):.2f} MB)")
    print(f"SHA-1     : {sha1}")
    print(f"MD5       : {md5}")
    
    if is_elf:
        print("Format    : Standard ELF (or decompressed Cafe RPX)")
    else:
        print("Format    : Cafe RPX (compressed Wii U executable)")

    print("\nNext Steps:")
    print("1. If using Ghidra:")
    print("   - Install the GhidraRPXLoader extension (see tools/setup_ghidra.py).")
    print("   - In Ghidra, open File -> Import File and select this RPX/ELF.")
    print("2. Extract strings and symbols:")
    print(f"   - Run: python tools/extract_symbols.py {file_path}")
    print("3. Matching decompilation:")
    print("   - Configure objdiff using objdiff.json to verify matched functions.")
    print("=" * 60)
    return True

def main():
    root = Path(__file__).resolve().parent.parent
    os.chdir(root)

    # Allow custom path passed as command line argument
    if len(sys.argv) > 1:
        target = Path(sys.argv[1])
        if target.exists():
            inspect_binary(target)
            return

    # Check search paths
    for p in SEARCH_PATHS:
        if p.exists():
            inspect_binary(p)
            return

    print("\n[!] No Splatoon executable found in expected locations.")
    print("Please copy 'Gambit.rpx' (or 'Gambit.elf') into the 'original/' folder:")
    print(f"  {root / 'original' / 'Gambit.rpx'}")
    print("\nOr run this script with the path to your dump:")
    print("  python tools/verify_dump.py <path_to_Gambit.rpx>")

if __name__ == "__main__":
    main()
