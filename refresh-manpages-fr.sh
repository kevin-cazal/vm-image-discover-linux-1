#!/bin/sh
# Refresh rootfs/usr/share/man/fr from vendor/manpages-fr_*.deb (host tool).
# Run after updating vendor/*.deb. Rebuilds mandoc.db via makewhatis.
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname "$0")" && pwd)
MPFR_DEB_VERSION="${MPFR_DEB_VERSION:-4.18.1-1}"
MPFR_DEB="${MPFR_DEB:-$SCRIPT_DIR/vendor/manpages-fr_${MPFR_DEB_VERSION}_all.deb}"
DEST="$SCRIPT_DIR/rootfs/usr/share/man/fr"

if [ ! -f "$MPFR_DEB" ]; then
	printf 'ERROR: missing vendored package: %s\n' "$MPFR_DEB" >&2
	exit 1
fi

refresh_with_docker() {
	if [ -z "${DOCKER_HOST:-}" ] && [ -S "${XDG_RUNTIME_DIR:-/run/user/$(id -u)}/docker.sock" ]; then
		export DOCKER_HOST="unix://${XDG_RUNTIME_DIR:-/run/user/$(id -u)}/docker.sock"
	fi
	if ! command -v docker >/dev/null 2>&1; then
		return 1
	fi
	docker run --rm \
		-v "$SCRIPT_DIR/rootfs:/rootfs:rw" \
		-v "$MPFR_DEB:/vendor.deb:ro" \
		alpine:3.21 \
		sh -eu -c '
			apk add --no-cache binutils mandoc mandoc-apropos >/dev/null
			tmp=$(mktemp -d)
			trap "rm -rf \"$tmp\"" EXIT
			cd "$tmp"
			ar x /vendor.deb
			tar -xf data.tar.*
			rm -rf /rootfs/usr/share/man/fr
			cp -a usr/share/man/fr /rootfs/usr/share/man/fr
			/usr/sbin/makewhatis /rootfs/usr/share/man/fr
		'
}

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

if ! command -v ar >/dev/null 2>&1 || ! command -v tar >/dev/null 2>&1; then
	printf 'ar/tar missing; using Docker\n' >&2
	refresh_with_docker || exit 1
	printf 'Refreshed %s from %s (docker)\n' "$DEST" "$MPFR_DEB" >&2
	exit 0
fi

cp "$MPFR_DEB" "$tmpdir/manpages-fr.deb"
(cd "$tmpdir" && ar x manpages-fr.deb && tar -xf data.tar.* ./usr/share/man/fr)

rm -rf "$DEST"
if ! mkdir -p "$(dirname "$DEST")" 2>/dev/null || ! cp -a "$tmpdir/usr/share/man/fr" "$DEST" 2>/dev/null; then
	printf 'Cannot write %s; using Docker\n' "$DEST" >&2
	refresh_with_docker || {
		printf 'ERROR: re-run as root (e.g. doas %s) or fix Docker\n' "$0" >&2
		exit 1
	}
	printf 'Refreshed %s from %s (docker)\n' "$DEST" "$MPFR_DEB" >&2
	exit 0
fi

if command -v makewhatis >/dev/null 2>&1; then
	makewhatis "$DEST"
elif [ -x /usr/sbin/makewhatis ]; then
	/usr/sbin/makewhatis "$DEST"
elif refresh_with_docker; then
	:
else
	printf 'ERROR: makewhatis not found and Docker unavailable\n' >&2
	exit 1
fi

printf 'Refreshed %s from %s\n' "$DEST" "$MPFR_DEB" >&2
