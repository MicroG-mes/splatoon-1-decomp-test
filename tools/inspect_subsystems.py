import json

with open('tools/database/functions.json', 'r') as f:
    funcs = json.load(f)

def inspect_range(start_hex, end_hex, label):
    start = int(start_hex, 16)
    end = int(end_hex, 16)
    print(f'=== {label} ({start_hex} - {end_hex}) ===')
    matched = [fn for fn in funcs if start <= fn['int_address'] <= end]
    for fn in matched[:25]:
        strs = [s for s in fn.get('strings', []) if len(s) > 2]
        print(f"{fn['address']} (len {fn['size']}): {strs[:3]}")

inspect_range('0x02835000', '0x02841000', 'Shop Systems')
inspect_range('0x02618000', '0x0261A000', 'Judd (Judge)')
inspect_range('0x0249B000', '0x0249D000', 'Spyke (Gear Order)')
inspect_range('0x02762000', '0x02765000', 'Lobby')
inspect_range('0x021B5000', '0x021B9000', 'Squid Jump / MiniGame')
