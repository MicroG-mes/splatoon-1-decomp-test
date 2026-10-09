import sys

with open('original/Gambit.elf', 'rb') as f:
    elf = f.read()

terms = [b'AirDancer', b'HeavyCrane', b'Jerry', b'JudgeSleep', b'ClimbLift', b'Hohei']
for t in terms:
    pos = 0
    count = 0
    print(f"=== Term: {t.decode()} ===")
    while count < 8:
        pos = elf.find(t, pos)
        if pos == -1: break
        s_start = elf.rfind(b'\0', 0, pos) + 1
        s_end = elf.find(b'\0', pos)
        s = elf[s_start:s_end].decode('ascii', errors='ignore')
        print(f"  at 0x{pos:08X}: \"{s}\"")
        pos += len(t)
        count += 1
