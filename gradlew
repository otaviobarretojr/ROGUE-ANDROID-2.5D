#!/bin/sh
set -eu
GRADLE_VERSION=8.9
BASE_DIR="${GRADLE_USER_HOME:-$HOME/.gradle}/rogue-bootstrap"
DIST="$BASE_DIR/gradle-$GRADLE_VERSION"
if [ ! -x "$DIST/bin/gradle" ]; then
  mkdir -p "$BASE_DIR"
  ZIP="$BASE_DIR/gradle-$GRADLE_VERSION-bin.zip"
  if command -v curl >/dev/null 2>&1; then
    curl -L --fail --retry 3 "https://services.gradle.org/distributions/gradle-$GRADLE_VERSION-bin.zip" -o "$ZIP"
  else
    wget -O "$ZIP" "https://services.gradle.org/distributions/gradle-$GRADLE_VERSION-bin.zip"
  fi
  unzip -q -o "$ZIP" -d "$BASE_DIR"
fi
exec "$DIST/bin/gradle" "$@"
