#!/bin/sh
set -eu

_step_counter=0
step() {
	_step_counter=$(( _step_counter + 1 ))
	printf '\n\033[1;36m%d) %s\033[0m\n' $_step_counter "$@" >&2
}

uname -a

step 'Set up timezone'
setup-timezone -z Europe/Paris

step 'Set up loopback only'
cat > /etc/network/interfaces <<-EOF
	iface lo inet loopback
EOF
ln -s networking /etc/init.d/net.lo

step 'Adjust rc.conf'
sed -Ei \
	-e 's/^[# ](rc_depend_strict)=.*/\1=YES/' \
	-e 's/^[# ](rc_parallel)=.*/\1=NO/' \
	-e 's/^[# ](rc_logger)=.*/\1=YES/' \
	-e 's/^[# ](unicode)=.*/\1=YES/' \
	/etc/rc.conf

step 'Enable services'
rc-update add net.lo boot
rc-update add termencoding boot

step 'Create non-root user'
adduser -D -s /bin/bash -h /home/user42 user42
addgroup user42 video
addgroup user42 input
addgroup user42 tty
mkdir -p /home/user42/memos
chown -R user42:user42 /home/user42/
chown -R root:root /root/

step 'Nano editor bindings (browser-safe shortcuts)'
if command -v nano >/dev/null 2>&1; then
	cat > /etc/nanorc <<'NANORC'
# Unbind shortcuts that browsers intercept even when the terminal is focused.
unbind ^W all
bind M-W whereis all

unbind ^T main
bind M-T spell main

unbind ^N main
bind M-N newbuffer main

unbind ^R main
bind M-R insert main

unbind ^F all
bind M-F whereis all

unbind ^S main

# Keep ^O (save) and ^X (exit) — standard nano, browser-safe enough.
NANORC
	chmod 644 /etc/nanorc
fi

step 'Setup autologin'
sed -i '/^hvc0::/d; /^ttyS0::/d; /^tty1::respawn:/d' /etc/inittab
chmod +x /etc/init.d/agetty-hvc0 /etc/init.d/agetty-ttyS0
rc-update add agetty-hvc0 default
rc-update add agetty-ttyS0 default

sed -Ei \
	-e 's|^[# ]*(default_kernel_opts)=.*|\1="console=tty0 console=hvc0"|' \
	/etc/update-extlinux.conf
update-extlinux

step 'Host 9p share and vm-bridge'
if [ ! -x /usr/local/sbin/mount-host-share ]; then
	echo "mount-host-share missing: run mount-host-share/build.sh before building the image" >&2
	exit 1
fi
if [ ! -x /usr/local/bin/vm-bridge-send ]; then
	echo "vm-bridge-send missing: run vm-bridge-send/build.sh before building the image" >&2
	exit 1
fi

mkdir -p /mnt/host /etc/modules-load.d
printf '%s\n' 9p 9pnet_virtio > /etc/modules-load.d/host9p.conf
chmod +x /etc/init.d/mount-host9p
rc-update add mount-host9p boot
chmod 4755 /usr/local/bin/vm-bridge-send
chmod 4755 /usr/local/bin/vm-bridge-raw
chmod 4755 /usr/local/bin/vm-bridge-read
chmod 4755 /usr/local/bin/vm-bridge-listen
chmod 4755 /usr/local/sbin/mount-host-share

if [ -f /etc/fstab ]; then
	sed -i 's/\trelatime\t/\tnoatime\t/' /etc/fstab
	if ! grep -q '[[:space:]]/mnt/host[[:space:]]' /etc/fstab; then
		printf '%s\n' 'host9p	/mnt/host	9p	trans=virtio,version=9p2000.L,noauto,nofail	0 0' >> /etc/fstab
	fi
fi

if [ -f /etc/profile ] && ! grep -q '^export MANPATH=/usr/share/man/fr:/usr/share/man$' /etc/profile; then
	printf '%s\n' 'export MANPATH=/usr/share/man/fr:/usr/share/man' >> /etc/profile
fi

step 'French manual pages (rootfs)'
if [ ! -d /usr/share/man/fr ] || [ -z "$(find /usr/share/man/fr -name '*.gz' 2>/dev/null | head -n 1)" ]; then
	echo "French man pages missing from image skeleton: run ./refresh-manpages-fr.sh in vm-image" >&2
	exit 1
fi

step 'Man page index'
if command -v makewhatis >/dev/null 2>&1; then
	makewhatis
fi

rm -rf /var/cache/apk/*
