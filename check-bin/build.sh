#!/bin/sh
set -e

cd "$(dirname "$0")"

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
DEST="$SCRIPT_DIR/../rootfs/usr/local/bin"
OUT_DIR="$SCRIPT_DIR/out"
NAMES="check_shell101_cp check_shell101_mv check_shell101_nano check_shell101_rm check_shell101_livrable1 check_shell102_grep_aa7566 check_shell102_sort_grades check_shell102_sort_on_time check_shell102_head_tail_last5 check_shell102_head_tail_first10 check_shell102_head_tail_sample check_shell102_cut_grades check_shell102_cut_cars check_shell102_sed check_shell102_find_concat check_shell102_grep_avance"
STAGING=$(mktemp -d)
trap 'rm -rf "$STAGING"' EXIT

build_native() {
	if ! command -v gcc >/dev/null 2>&1; then
		return 1
	fi
	for name in $NAMES; do
		# shellcheck disable=SC2086
		gcc -O2 -Wall -Wextra -s -static -m32 -o "$STAGING/$name" \
			common.c md5.c "$name.c" || return 1
		strip "$STAGING/$name" 2>/dev/null || true
	done
	return 0
}

docker_sock() {
	if [ -n "${DOCKER_HOST:-}" ]; then
		return 0
	fi
	if [ -S /var/run/docker.sock ] && [ -r /var/run/docker.sock ]; then
		export DOCKER_HOST=unix:///var/run/docker.sock
		return 0
	fi
	if [ -S "${XDG_RUNTIME_DIR:-/run/user/$(id -u)}/docker.sock" ]; then
		export DOCKER_HOST="unix://${XDG_RUNTIME_DIR:-/run/user/$(id -u)}/docker.sock"
		return 0
	fi
	return 1
}

build_docker() {
	docker_sock || true
	if ! command -v docker >/dev/null 2>&1; then
		return 1
	fi
	if [ -z "${DOCKER_HOST:-}" ]; then
		return 1
	fi
	IMAGE=check-bin-i386-alpine
	CONTAINER=check-bin-i386-alpine-tmp
	docker build --platform linux/386 -t "$IMAGE" -f Containerfile . || return 1
	docker rm -f "$CONTAINER" 2>/dev/null || true
	docker create --name "$CONTAINER" "$IMAGE" || return 1
	for name in $NAMES; do
		docker cp "$CONTAINER:/out/$name" "$STAGING/$name" || return 1
		strip "$STAGING/$name" 2>/dev/null || true
	done
	docker rm "$CONTAINER" || true
	return 0
}

if build_docker; then
	:
elif build_native; then
	echo "Built check_shell101_* with gcc -m32 (Docker unavailable)" >&2
else
	echo "Need Docker (doas ./build.sh) or gcc -m32 for native fallback" >&2
	exit 1
fi

install -d "$OUT_DIR"
for name in $NAMES; do
	install -m 755 "$STAGING/$name" "$OUT_DIR/$name"
done

if install -d "$DEST" 2>/dev/null && [ -w "$DEST" ]; then
	for name in $NAMES; do
		install -m 755 "$STAGING/$name" "$DEST/$name"
	done
	echo "Installed check_shell* in $DEST"
else
	echo "Installed check_shell* in $OUT_DIR (rootfs not writable; re-run: doas ./build.sh)" >&2
fi
