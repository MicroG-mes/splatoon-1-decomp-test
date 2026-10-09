import struct, os

def decompress_yaz0(data):
    if data[:4] != b'Yaz0':
        return data
    uncomp_size = struct.unpack('>I', data[4:8])[0]
    src_pos = 16
    dst = bytearray()
    valid_bits = 0
    curr_byte = 0
    while len(dst) < uncomp_size:
        if valid_bits == 0:
            curr_byte = data[src_pos]
            src_pos += 1
            valid_bits = 8
        if (curr_byte & 0x80) != 0:
            dst.append(data[src_pos])
            src_pos += 1
        else:
            b1 = data[src_pos]
            b2 = data[src_pos+1]
            src_pos += 2
            dist = ((b1 & 0x0F) << 8) | b2
            copy_src = len(dst) - (dist + 1)
            num_bytes = b1 >> 4
            if num_bytes == 0:
                num_bytes = data[src_pos] + 0x12
                src_pos += 1
            else:
                num_bytes += 2
            for _ in range(num_bytes):
                dst.append(dst[copy_src])
                copy_src += 1
        curr_byte <<= 1
        valid_bits -= 1
    return bytes(dst)

def inspect_bfres(path):
    if not os.path.exists(path):
        print(f"Not found: {path}")
        return
    with open(path, 'rb') as f:
        decomp = decompress_yaz0(f.read())
    fres_pos = decomp.find(b'FRES')
    if fres_pos == -1:
        print(f'{path}: No FRES found')
        return
    data = decomp[fres_pos:]
    total_verts = 0
    fmdl_pos = 0
    while True:
        fmdl_pos = data.find(b'FMDL', fmdl_pos)
        if fmdl_pos == -1:
            break
        name_off = struct.unpack('>I', data[fmdl_pos+4:fmdl_pos+8])[0]
        actual_name_pos = fmdl_pos + 4 + name_off
        name_end = data.find(b'\0', actual_name_pos)
        model_name = data[actual_name_pos:name_end].decode('ascii', errors='ignore')
        
        num_fvtx = struct.unpack('>H', data[fmdl_pos+0x20:fmdl_pos+0x22])[0]
        num_fshp = struct.unpack('>H', data[fmdl_pos+0x22:fmdl_pos+0x24])[0]
        
        next_fmdl = data.find(b'FMDL', fmdl_pos + 4)
        if next_fmdl == -1:
            next_fmdl = len(data)
            
        vtx_rel = struct.unpack('>I', data[fmdl_pos+0x10:fmdl_pos+0x14])[0]
        fvtx_base = fmdl_pos + 0x10 + vtx_rel
        fvtx_list = []
        for v in range(num_fvtx):
            off = fvtx_base + v * 0x20
            if off + 32 <= len(data) and data[off:off+4] == b'FVTX':
                nv = struct.unpack('>I', data[off+8:off+12])[0]
                fvtx_list.append(nv)
        if len(fvtx_list) < num_fvtx:
            scan = fvtx_base
            while scan + 32 <= next_fmdl and len(fvtx_list) < num_fvtx:
                if data[scan:scan+4] == b'FVTX':
                    nv = struct.unpack('>I', data[scan+8:scan+12])[0]
                    fvtx_list.append(nv)
                scan += 4
                
        shp_count = 0
        shp_verts = 0
        scan_s = fmdl_pos
        while scan_s + 64 <= next_fmdl:
            if data[scan_s:scan_s+4] == b'FSHP':
                shp_count += 1
                vtx_idx = struct.unpack('>H', data[scan_s+0x28:scan_s+0x2A])[0]
                if vtx_idx < len(fvtx_list):
                    shp_verts += fvtx_list[vtx_idx]
            scan_s += 4
        print(f'{path} -> Model: "{model_name}", FVTX: {fvtx_list}, FSHP: {shp_count}, Verts: {shp_verts}')
        total_verts += shp_verts
        fmdl_pos += 4
    print(f'Total verts for {path}: {total_verts}\n')

for p in [
    'content/Model/Enm_Hohei.szs',
    'content/Model/Obj_AirDancer.szs',
    'content/Model/Lft_HeavyCraneMachine.szs',
    'content/Model/Npc_JudgeSleep.szs',
    'content/Model/Npc_Judge.szs',
    'content/Model/Obj_Jerry00.szs',
    'content/Model/Lft_ClimbLift.szs'
]:
    inspect_bfres(p)
