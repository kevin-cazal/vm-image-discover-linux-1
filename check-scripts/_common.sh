# shellcheck shell=sh
# Shared helpers for Shell 101 VM check scripts.

err() {
	printf '%s\n' "$*" >&2
}

ok() {
	printf 'shell1{%s}\n' "$1"
	exit 0
}

require_file() {
	if [ ! -f "$1" ]; then
		err "Fichier manquant : $1"
		return 1
	fi
}

require_dir() {
	if [ ! -d "$1" ]; then
		err "Répertoire manquant : $1"
		return 1
	fi
}

require_absent() {
	if [ -e "$1" ]; then
		err "Élément encore présent (devrait être absent) : $1"
		return 1
	fi
}
