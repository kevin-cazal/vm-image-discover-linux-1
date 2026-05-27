/* Validate post-cp layout under ~/101; flag emitted from XOR-decoded digest. */
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
		86, 92, 69, 86, 87, 68, 82, 19, 87, 83, 87, 92, 8, 69, 80, 13,
		65, 86, 64, 3, 5, 84, 80, 90, 75, 91, 92, 23, 80, 70, 83, 2
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

	NEED_DIR("");
	NEED_DIR("code");
	NEED_FILE("code/infinite_loop_timer.py");
	NEED_FILE("code/read_dev_zero_until_eof.c");
	NEED_DIR("links");
	NEED_FILE("links/qrcode1");
	NEED_FILE("links/qrcode2");
	NEED_FILE("memo1.txt");
	NEED_FILE("memo2.txt");
	NEED_DIR("memos");
	NEED_DIR("works");
	NEED_FILE("works/essay1.txt");
	NEED_FILE("works/essay2.txt");
	NEED_FILE(".secret1.txt");
	NEED_FILE(".secret2.txt");

#undef NEED_DIR
#undef NEED_FILE

	if (fail) {
		check_fail_msg(
		    "Copie incomplète : vérifiez memo1.txt, memo2.txt, memos/, "
		    ".secret*, links/, works/, code/ dans ~/101.");
	}

	/* Build the expected flag value at runtime (no cleartext digest stored). */
	decode_flag_secret(expect, sizeof(expect), flag_enc, sizeof(flag_enc));
	emit_flag_hex(expect);
	return 0;
}
