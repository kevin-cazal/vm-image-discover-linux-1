#!/bin/sh
# Install Shell 101 check scripts into the vm-image rootfs.
set -eu
SCRIPT_DIR=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
DEST="${1:-$SCRIPT_DIR/../rootfs/home/user42/bin}"

mkdir -p "$DEST"
for name in check_shell101_cp check_shell101_mv check_shell101_micro check_shell101_rm check_shell101_livrable1; do
	install -m 755 "$SCRIPT_DIR/$name" "$DEST/$name"
done
install -m 644 "$SCRIPT_DIR/_common.sh" "$DEST/check_shell101_common.sh"
# Rewrite _common.sh → check_shell101_common.sh (match path suffix only)
for name in check_shell101_cp check_shell101_mv check_shell101_micro check_shell101_rm check_shell101_livrable1; do
	sed -i 's|/_common\.sh|/check_shell101_common.sh|' "$DEST/$name"
done
