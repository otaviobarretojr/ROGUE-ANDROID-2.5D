#!/usr/bin/env python3
import argparse
import collections
import pathlib
import re

def load_symbols(path):
    symbols=[]
    for raw in pathlib.Path(path).read_text(errors="ignore").splitlines():
        raw=raw.strip()
        if not raw:
            continue
        parts=raw.split()
        symbols.append(parts[-1])
    return symbols

def source_files(root):
    for base in ("src","gflib"):
        p=root/base
        if p.exists():
            yield from p.rglob("*.c")

def likely_provider(text, symbol):
    esc=re.escape(symbol)
    patterns=[
        re.compile(rf"(?m)^(?!\s*(?:extern|static)\b)[^;\n]*\b{esc}\s*\([^;]*\)\s*\{{"),
        re.compile(rf"(?m)^(?!\s*(?:extern|static)\b)[^;\n]*\b{esc}\b[^;\n]*="),
    ]
    return any(p.search(text) for p in patterns)

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("unresolved")
    ap.add_argument("core")
    ap.add_argument("--details",required=True)
    ap.add_argument("--summary",required=True)
    args=ap.parse_args()

    core=pathlib.Path(args.core)
    symbols=load_symbols(args.unresolved)
    files=[]
    for path in source_files(core):
        try:
            files.append((path,path.read_text(errors="ignore")))
        except OSError:
            pass

    providers=collections.defaultdict(list)
    unresolved_without_provider=[]
    detail_lines=[]

    for symbol in symbols:
        hits=[]
        for path,text in files:
            if likely_provider(text,symbol):
                rel=path.relative_to(core).as_posix()
                hits.append(rel)
                providers[rel].append(symbol)
        if hits:
            detail_lines.append(f"{symbol}: "+", ".join(sorted(hits)))
        else:
            unresolved_without_provider.append(symbol)
            detail_lines.append(f"{symbol}: <no C provider found>")

    pathlib.Path(args.details).write_text("\n".join(detail_lines)+"\n")

    ranked=sorted(providers.items(), key=lambda kv:(-len(kv[1]),kv[0]))
    summary=[
        f"unresolved symbols: {len(symbols)}",
        f"symbols with C provider candidates: {len(symbols)-len(unresolved_without_provider)}",
        f"symbols without C provider candidate: {len(unresolved_without_provider)}",
        "",
        "provider candidates ranked by unresolved symbols supplied:",
    ]
    for path,syms in ranked:
        summary.append(f"{len(syms):4d}  {path}")
        summary.append("      "+", ".join(sorted(syms)))
    pathlib.Path(args.summary).write_text("\n".join(summary)+"\n")

if __name__=="__main__":
    main()
