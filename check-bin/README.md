# Shell 101–102 check binaries

Static i386 C programs installed on the VM as `/usr/local/bin/check_shell*`.

## Build

From the repo host (Docker required), same pattern as `vm-bridge-send/`:

```sh
doas ./build.sh
```

Uses `/var/run/docker.sock` when readable (typical with `doas`); otherwise the user’s rootless Docker socket.

`../build.sh` invokes this automatically before packing the disk image.

## Flags

- **cp / mv:** flag digest computed at runtime (`tree` output → MD5); nothing stored in the binary.
- **micro / rm / livrable1 / Shell 102 checkers:** expected 32-char hex digest stored XOR-obfuscated (`discover101` key). On success the flag is assembled on the stack (`shell1{` + hex + `}`) — no contiguous `shell1{…}` string in `.rodata`.

If a CTFd flag changes, update the `flag_enc` / `expect_enc` array in the matching `.c` (run `_xor_gen.py` locally to print new bytes) and rebuild.

## Guest filesystem

Only stripped binaries land in `rootfs/usr/local/bin/`. No `.c` sources or Docker artifacts are copied into the image.
