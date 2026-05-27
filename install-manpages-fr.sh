#!/bin/sh
# Install French manual pages from Debian manpages-fr (manpages-l10n project).
# Invoked from configure.sh during image build (network required).
set -eu

MPFR_DEB_VERSION="${MPFR_DEB_VERSION:-4.18.1-1}"
MPFR_DEB="manpages-fr_${MPFR_DEB_VERSION}_all.deb"
MPFR_URL="${MPFR_URL:-https://deb.debian.org/debian/pool/main/m/manpages-l10n/$MPFR_DEB}"

if [ -d /usr/share/man/fr ] && [ -n "$(find /usr/share/man/fr -name '*.gz' 2>/dev/null | head -n 1)" ]; then
	printf 'French man pages already present under /usr/share/man/fr\n' >&2
	exit 0
fi

apk add --no-cache curl binutils

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

printf 'Downloading %s\n' "$MPFR_URL" >&2
if ! curl -fsSL -o "$tmpdir/$MPFR_DEB" "$MPFR_URL"; then
	printf 'WARNING: could not download French manpages (%s). Continuing without them.\n' "$MPFR_URL" >&2
	exit 0
fi

cd "$tmpdir"
ar x "$MPFR_DEB"
tar -xf data.tar.* -C /

apk del --no-cache binutils curl

if command -v makewhatis >/dev/null 2>&1; then
	makewhatis /usr/share/man/fr
fi

printf 'Installed French man pages (%s)\n' "$MPFR_DEB_VERSION" >&2
