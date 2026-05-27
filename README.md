# vm-image-discover-linux-1

Alpine Linux guest disk image for [shell-1](https://github.com/kevin-cazal/shell-1) (Discover Linux / Shell 101–102).

## Prerequisites

- Linux with root (for image build)
- Docker (to build `vm-bridge-send` and `mount-host-share` static binaries)
- git submodules initialized

## Build

```sh
git submodule update --init --recursive
doas ./build.sh
```

Produces `alpine-bios-512M.img` by default (`IMAGE_SIZE=512M`).

Override:

```sh
IMAGE_SIZE=256M IMAGE=./alpine-bios-256M.img doas ./build.sh
```

## Guest layout

- User `user42` (bash), autologin on `hvc0`
- Challenge files under `/home/user42/`
- Livrables via virtio 9p at **`/mnt/host`** (not `/delivery`)
- `vm-bridge-send splash-ready` on first browser console login (for `.v86b` builds)

## Packages

See [`packages`](packages). Course tools include `micro`, `mandoc`, `coreutils-doc`, and French translations (`manpages-fr` via [`install-manpages-fr.sh`](install-manpages-fr.sh) at build time). The guest defaults to `LANG=fr_FR.UTF-8` so `man` shows French pages when available (English fallback otherwise).

Image build needs network access when `configure.sh` runs (Debian `manpages-fr` package download).

## Shell 101 check binaries

Built with Docker (or `gcc -m32` fallback) via [`check-bin/build.sh`](check-bin/build.sh); installed to `/usr/local/bin/check_shell101_*`. `build.sh` runs this before packing the image. No C sources are shipped on the guest.
