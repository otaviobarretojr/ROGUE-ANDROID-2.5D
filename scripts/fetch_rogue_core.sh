#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="$ROOT/upstream/pokeemerald-rogue"
URL="https://github.com/MinZe25/pokeemerald-rogue-vita.git"
BRANCH="vita_port"

if [[ -d "$DEST/.git" ]]; then
  git -C "$DEST" fetch --depth=1 origin "$BRANCH"
  git -C "$DEST" checkout -B "$BRANCH" FETCH_HEAD
else
  rm -rf "$DEST"
  git clone --depth=1 --branch "$BRANCH" "$URL" "$DEST"
fi

git -C "$DEST" rev-parse HEAD
