#!/usr/bin/env python3
import argparse
import pathlib
import struct

ELFCLASS64=2
ELFDATA2LSB=1
SHT_SYMTAB=2
SHT_RELA=4
R_AARCH64_NONE=0
R_AARCH64_ABS32=258
STB_LOCAL=0

EHDR_FMT="<16sHHIQQQIHHHHHH"
SHDR_FMT="<IIQQQQIIQQ"
SYM_FMT="<IBBHQQ"
RELA_FMT="<QQq"

def cstr(blob, off):
    end=blob.find(b"\0", off)
    return blob[off:end].decode("utf-8", "replace")

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--object", required=True)
    ap.add_argument("--registry-out", required=True)
    ap.add_argument("--manifest-out", required=True)
    args=ap.parse_args()

    path=pathlib.Path(args.object)
    data=bytearray(path.read_bytes())

    eh=struct.unpack_from(EHDR_FMT,data,0)
    ident=eh[0]
    if ident[4] != ELFCLASS64 or ident[5] != ELFDATA2LSB:
        raise SystemExit("expected ELF64 little-endian object")

    e_shoff=eh[6]
    e_shentsize=eh[11]
    e_shnum=eh[12]
    e_shstrndx=eh[13]
    if e_shentsize != struct.calcsize(SHDR_FMT):
        raise SystemExit(f"unexpected section header size {e_shentsize}")

    sections=[]
    for i in range(e_shnum):
        vals=struct.unpack_from(SHDR_FMT,data,e_shoff+i*e_shentsize)
        sections.append({
            "name_off":vals[0],"type":vals[1],"flags":vals[2],"addr":vals[3],
            "offset":vals[4],"size":vals[5],"link":vals[6],"info":vals[7],
            "align":vals[8],"entsize":vals[9],
        })
    shstr_sec=sections[e_shstrndx]
    shstr=bytes(data[shstr_sec["offset"]:shstr_sec["offset"]+shstr_sec["size"]])
    for s in sections:
        s["name"]=cstr(shstr,s["name_off"]) if s["name_off"] < len(shstr) else ""

    tokens={}
    entries=[None]
    manifest=[]
    patched=0

    for relsec_index,relsec in enumerate(sections):
        if relsec["type"] != SHT_RELA:
            continue
        target_index=relsec["info"]
        if target_index >= len(sections):
            continue
        target=sections[target_index]
        symtab=sections[relsec["link"]]
        if symtab["type"] != SHT_SYMTAB:
            raise SystemExit("RELA section does not link a SYMTAB")
        strtab=sections[symtab["link"]]
        strings=bytes(data[strtab["offset"]:strtab["offset"]+strtab["size"]])

        syms=[]
        count=symtab["size"]//symtab["entsize"]
        for i in range(count):
            st=struct.unpack_from(SYM_FMT,data,symtab["offset"]+i*symtab["entsize"])
            syms.append({
                "name":cstr(strings,st[0]) if st[0] < len(strings) else "",
                "info":st[1],"other":st[2],"shndx":st[3],"value":st[4],"size":st[5],
            })

        rel_count=relsec["size"]//relsec["entsize"]
        for i in range(rel_count):
            roff=relsec["offset"]+i*relsec["entsize"]
            r_offset,r_info,r_addend=struct.unpack_from(RELA_FMT,data,roff)
            r_type=r_info & 0xffffffff
            sym_idx=r_info >> 32
            if r_type != R_AARCH64_ABS32:
                continue
            if sym_idx >= len(syms):
                raise SystemExit("bad relocation symbol index")
            sym=syms[sym_idx]
            name=sym["name"]
            if not name:
                raise SystemExit("ABS32 relocation with unnamed symbol")

            key=(name,r_addend)
            token=tokens.get(key)
            if token is None:
                token=len(entries)
                tokens[key]=token
                entries.append(key)

            patch_off=target["offset"]+r_offset
            if patch_off+4 > target["offset"]+target["size"]:
                raise SystemExit("relocation points outside target section")
            struct.pack_into("<I",data,patch_off,token)

            # Neutralize the relocation. The token is now final data.
            struct.pack_into("<Q",data,roff+8,(sym_idx << 32) | R_AARCH64_NONE)
            patched+=1
            manifest.append(
                f"{token:5d}  {target['name']}+0x{r_offset:x}  {name}"
                + (f"{r_addend:+d}" if r_addend else "")
            )

    if patched == 0:
        raise SystemExit("no AArch64 ABS32 script pointer relocations found")

    path.write_bytes(data)

    reg=[
        '.section .rodata.android_script_ptrs,"a",%progbits',
        '.p2align 3',
        '.global gAndroidScriptPointerTable',
        '.type gAndroidScriptPointerTable,%object',
        'gAndroidScriptPointerTable:',
        '    .xword 0',
    ]
    for item in entries[1:]:
        name,addend=item
        expr=name
        if addend > 0:
            expr=f"{name}+{addend}"
        elif addend < 0:
            expr=f"{name}{addend}"
        reg.append(f"    .xword {expr}")
    reg += [
        '.global gAndroidScriptPointerCount',
        '.type gAndroidScriptPointerCount,%object',
        'gAndroidScriptPointerCount:',
        f'    .xword {len(entries)}',
        ''
    ]
    pathlib.Path(args.registry_out).write_text("\n".join(reg))
    pathlib.Path(args.manifest_out).write_text(
        f"tokens: {len(entries)-1}\nrelocations patched: {patched}\n\n"
        + "\n".join(manifest) + "\n"
    )

if __name__=="__main__":
    main()
