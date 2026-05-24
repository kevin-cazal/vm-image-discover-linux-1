# Headless bundle build: signal splash-ready once root is on serial (ttyS0).
case "$(tty)" in
/dev/ttyS0)
	if [ ! -f /tmp/.splash_ready_sent ] && [ -w /dev/hvc1 ]; then
		touch /tmp/.splash_ready_sent
		/usr/local/bin/vm-bridge-send splash-ready
	fi
	;;
esac
