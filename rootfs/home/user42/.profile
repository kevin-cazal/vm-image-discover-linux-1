# Discover Linux — Shell 101 atelier (login on hvc0)

export LANG=C.UTF-8
export LC_CTYPE=C.UTF-8

case "$(tty)" in
/dev/hvc0)
	if [ -w /dev/hvc1 ] && [ ! -f /tmp/.splash_ready_sent ]; then
		touch /tmp/.splash_ready_sent
		/usr/local/bin/vm-bridge-send splash-ready
	fi

	clear
	cat <<'EOF'

  Discover Linux — Shell 101
  Atelier ligne de commande

  Bienvenue ! Consultez l'énoncé dans le panneau à gauche du navigateur.
  Travaillez dans ce terminal ; déposez vos livrables dans :

      /mnt/host

  (partage avec l'ordinateur hôte — fichiers visibles côté navigateur)

EOF

	/usr/local/sbin/mount-host-share 2>/dev/null || true
	cd "$HOME" || true
	;;
esac
