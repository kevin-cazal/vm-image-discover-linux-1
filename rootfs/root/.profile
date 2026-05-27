# ttyS0 is wired to v86 serial0 during headless .v86b builds (root autologin).
# user42 splash + splash-ready run on hvc0 in the browser; mirror the handshake here.
case "$(tty 2>/dev/null)" in
/dev/ttyS0)
	if [ ! -f /tmp/.splash_ready_sent ] && [ -w /dev/hvc1 ] 2>/dev/null; then
		touch /tmp/.splash_ready_sent
		/usr/local/bin/vm-bridge-send splash-ready 2>/dev/null || true
	fi
	;;
esac
