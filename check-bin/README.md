# Shell 101 check binaries

Static i386 C programs installed on the VM as `/usr/local/bin/check_shell101_*`.

## Build

From the repo host (Docker required), same pattern as `vm-bridge-send/`:

```sh
./build.sh
```

`../build.sh` invokes this automatically before packing the disk image.

## Flags

- **cp / mv:** flag digest computed at runtime (`tree` output → MD5); nothing stored in the binary.
- **grep_aa7566 / sort_grades / sort_on_time:** expected 32-char hex digest stored XOR-obfuscated (`discover101` key). On success the flag is assembled on the stack (`shell1{` + hex + `}`) — no contiguous `shell1{…}` string in `.rodata`.

If a CTFd flag changes, update the `flag_enc` / `expect_enc` array in the matching `.c` (run `_xor_gen.py` locally to print new bytes) and rebuild.

## Guest filesystem

Only stripped binaries land in `rootfs/usr/local/bin/`. No `.c` sources or Docker artifacts are copied into the image.
