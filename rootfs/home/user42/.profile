# Discover Linux — Shell 101 atelier (login on hvc0)

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

EOF
	cd "$HOME" || true
	;;
esac
