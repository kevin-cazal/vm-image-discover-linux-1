/* Validate post-mv layout under ~/101; flag emitted from XOR-decoded digest. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static int join_path(char *buf, size_t buflen, const char *a, const char *b)
{
	return snprintf(buf, buflen, "%s/%s", a, b) < (int)buflen;
}

int main(void)
{
	char root[256];
	char path[320];
	int fail = 0;
	const char *home = check_home();
	char expect[40];

	/* XOR-obfuscated 32-hex digest for the CTFd flag. */
	static const unsigned char flag_enc[] = {
		0, 94, 70, 6, 12, 18, 7, 16, 9, 0, 6, 82, 12, 21, 90, 10,
		19, 0, 68, 0, 0, 84, 84, 11, 16, 84, 94, 19, 84, 17, 85, 2
	};

	snprintf(root, sizeof(root), "%s/101", home);

#define NEED_DIR(sub) \
	do { \
		if (!join_path(path, sizeof(path), root, (sub))) { \
			fail = 1; \
			break; \
		} \
		if (!check_path_is_dir(path)) \
			fail = 1; \
	} while (0)

#define NEED_FILE(sub) \
	do { \
		if (!join_path(path, sizeof(path), root, (sub))) { \
			fail = 1; \
			break; \
		} \
		if (!check_path_is_file(path)) \
			fail = 1; \
	} while (0)

#define NEED_ABSENT(sub) \
	do { \
		if (!join_path(path, sizeof(path), root, (sub))) { \
			fail = 1; \
			break; \
		} \
		if (!check_path_absent(path)) \
			fail = 1; \
	} while (0)

	NEED_DIR("");
	NEED_FILE("links/ubuntu");
	NEED_FILE("links/wikipedia_linux");
	NEED_ABSENT("links/qrcode1");
	NEED_ABSENT("links/qrcode2");
	NEED_FILE("memos/memo1.txt");
	NEED_FILE("memos/memo2.txt");
	NEED_ABSENT("memo1.txt");
	NEED_ABSENT("memo2.txt");
	NEED_FILE("secret1.txt");
	NEED_FILE("secret2.txt");

#undef NEED_DIR
#undef NEED_FILE
#undef NEED_ABSENT

	if (fail) {
		check_fail_msg(
		    "État incorrect après mv : memos dans memos/, liens renommés, "
		    "secrets visibles à la racine de ~/101.");
	}

	/* Build the expected flag value at runtime (no cleartext digest stored). */
	decode_flag_secret(expect, sizeof(expect), flag_enc, sizeof(flag_enc));
	emit_flag_hex(expect);
	return 0;
}
