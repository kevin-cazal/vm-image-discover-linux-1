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

step 'Micro editor bindings (browser-safe shortcuts)'
if [ -x /usr/bin/micro ]; then
	mv /usr/bin/micro /usr/bin/_micro
	mkdir -p /etc/micro
	chmod +r /etc/micro
	printf '%s' '{"CtrlSpace": "NextSplit", "Altq": "Quit"}' > /etc/micro/bindings.json
	cat > /usr/bin/micro <<'MICROWRAP'
#!/bin/sh
exec /usr/bin/_micro -config-dir /etc/micro "$@"
MICROWRAP
	chmod +x /usr/bin/micro
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
fi

rm -rf /var/cache/apk/*
