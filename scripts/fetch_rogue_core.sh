#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="$ROOT/upstream/pokeemerald-rogue"
URL="https://github.com/MinZe25/pokeemerald-rogue-vita.git"
REV="bc9a99b5d762a761679829fb68f269c473700c62"

if [[ ! -d "$DEST/.git" ]]; then
  rm -rf "$DEST"
  git init "$DEST"
  git -C "$DEST" remote add origin "$URL"
fi

git -C "$DEST" fetch --depth=1 origin "$REV"
git -C "$DEST" checkout --detach FETCH_HEAD
ACTUAL="$(git -C "$DEST" rev-parse HEAD)"
test "$ACTUAL" = "$REV"
echo "$ACTUAL"
