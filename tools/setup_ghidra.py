#!/usr/bin/env python3
"""
Ghidra Setup Helper for Wii U RPX Analysis
Checks environment and provides instructions/download links for GhidraRPXLoader.
"""

import sys
import shutil
import urllib.request
import json
from pathlib import Path

GHIDRA_RPX_REPO = "Maschell/GhidraRPXLoader"

def check_java():
    java = shutil.which("java")
    if java:
        print(f"[+] Java runtime found: {java}")
        return True
    else:
        print("[-] Java runtime (JDK 17 or 21) not detected on PATH.")
        print("    Ghidra requires Java. You can install it via: winget install Microsoft.OpenJDK.21")
        return False

def check_ghidra():
    ghidra = shutil.which("ghidraRun") or shutil.which("ghidra")
    if ghidra:
        print(f"[+] Ghidra found: {ghidra}")
        return True
    else:
        print("[-] Ghidra not found in PATH (if installed in custom directory, that is fine).")
        print("    Download Ghidra at: https://github.com/NationalSecurityAgency/ghidra/releases")
        return False

def get_latest_rpx_loader_release():
    print(f"[*] Checking latest GhidraRPXLoader release from {GHIDRA_RPX_REPO}...")
    api_url = f"https://api.github.com/repos/{GHIDRA_RPX_REPO}/releases/latest"
    try:
        req = urllib.request.Request(api_url, headers={"User-Agent": "splatoondecomp-setup"})
        with urllib.request.urlopen(req, timeout=10) as resp:
            data = json.loads(resp.read().decode("utf-8"))
            tag = data.get("tag_name", "unknown")
            assets = data.get("assets", [])
            print(f"[+] Found GhidraRPXLoader release: {tag}")
            for a in assets:
                name = a.get("name", "")
                url = a.get("browser_download_url", "")
                if name.endswith(".zip"):
                    print(f"    Download URL: {url}")
                    return url
    except Exception as e:
        print(f"[-] Could not query GitHub API ({e}).")
        print("    You can manually download the zip at: https://github.com/Maschell/GhidraRPXLoader/releases")
    return None

def main():
    print("=" * 65)
    print(" [Ghidra Wii U Reverse Engineering Environment Setup]")
    print("=" * 65)
    
    check_java()
    check_ghidra()
    loader_url = get_latest_rpx_loader_release()

    print("\n--- Installation Steps ---")
    print("1. Download the GhidraRPXLoader extension zip file.")
    if loader_url:
        print(f"   Link: {loader_url}")
    else:
        print("   Link: https://github.com/Maschell/GhidraRPXLoader/releases")
    print("2. In Ghidra:")
    print("   - Open Ghidra.")
    print("   - Click 'File' -> 'Install Extensions...'")
    print("   - Click the '+' (plus) icon in the top right and select the downloaded .zip.")
    print("   - Ensure 'GhidraRPXLoader' is checked in the list.")
    print("   - Restart Ghidra.")
    print("3. Import Gambit.rpx:")
    print("   - Create or open a Ghidra project.")
    print("   - Click 'File' -> 'Import File...' and select 'original/Gambit.rpx'.")
    print("   - Format should automatically detect as 'Cafe RPX Loader'.")
    print("   - Language should detect as 'PowerPC:BE:32:Gekko_Broadway' or Espresso.")
    print("4. Auto Analysis:")
    print("   - Open Gambit in the CodeBrowser.")
    print("   - When prompted to analyze, click Yes.")
    print("   - Make sure 'Decompiler Parameter ID' and 'Demangler GNU' are checked.")
    print("   - Run the script 'fix_primary_imports.java' to resolve OS/Cafe library symbols!")
    print("=" * 65)

if __name__ == "__main__":
    main()
