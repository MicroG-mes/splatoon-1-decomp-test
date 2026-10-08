import json
import sys

def main():
    kw = sys.argv[1] if len(sys.argv) > 1 else "King"
    mode = sys.argv[2] if len(sys.argv) > 2 else "classes"
    if mode == "funcs":
        with open("tools/database/functions.json", "r", encoding="utf-8") as f:
            funcs = json.load(f)
        matches = [fn for fn in funcs if kw.lower() in fn["name"].lower()]
        print(f"Keyword '{kw}' in functions: {len(matches)} matches")
        for m in matches[:20]:
            print(f"  {m['name']} @ 0x{m['int_address']:08X} (size: {m['size']})")
    else:
        with open("tools/database/classes.json", "r", encoding="utf-8") as f:
            classes = json.load(f)
        matches = [c for c in classes if kw.lower() in c["class_name"].lower()]
        print(f"Keyword '{kw}' in classes: {len(matches)} matches")
        for m in matches[:15]:
            print(f"  {m['class_name']} @ {m['vtable_address']} ({len(m['methods'])} methods)")

if __name__ == "__main__":
    main()
