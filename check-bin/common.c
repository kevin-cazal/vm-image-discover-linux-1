#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const unsigned char xor_key[] = {
	'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', '1', '0', '1'
};

const char *check_home(void)
{
	const char *home = getenv("HOME");

	if (home && home[0])
		return home;
	return CHECK_HOME_DEFAULT;
}

void check_err(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

void check_fail_msg(const char *msg)
{
	check_err(msg);
	exit(1);
}

int check_path_is_file(const char *path)
{
	struct stat st;

	if (stat(path, &st) != 0) {
		fprintf(stderr, "Fichier manquant : %s\n", path);
		return 0;
	}
	if (!S_ISREG(st.st_mode)) {
		fprintf(stderr, "Fichier manquant : %s\n", path);
		return 0;
	}
	return 1;
}

int check_path_is_dir(const char *path)
{
	struct stat st;

	if (stat(path, &st) != 0) {
		fprintf(stderr, "Répertoire manquant : %s\n", path);
		return 0;
	}
	if (!S_ISDIR(st.st_mode)) {
		fprintf(stderr, "Répertoire manquant : %s\n", path);
		return 0;
	}
	return 1;
}

int check_path_absent(const char *path)
{
	struct stat st;

	if (stat(path, &st) == 0) {
		fprintf(stderr, "Élément encore présent (devrait être absent) : %s\n", path);
		return 0;
	}
	return 1;
}

void decode_xor(char *out, size_t out_len,
                const unsigned char *enc, size_t enc_len,
                const unsigned char *key, size_t key_len)
{
	size_t i;

	if (out_len < enc_len + 1)
		return;
	for (i = 0; i < enc_len; i++)
		out[i] = (char)(enc[i] ^ key[i % key_len]);
	out[enc_len] = '\0';
}

void emit_flag_hex(const char *hex32)
{
	emit_flag_body(hex32);
}

void emit_flag_body(const char *body)
{
	char out[80];
	const char pfx[] = { 's', 'h', 'e', 'l', 'l', '1', '{', '\0' };
	const char sfx[] = { '}', '\0' };

	strcpy(out, pfx);
	strncat(out, body, sizeof(out) - strlen(out) - 2);
	strcat(out, sfx);
	fputs(out, stdout);
	fputc('\n', stdout);
}

static int md5_hex_from_popen(const char *cmd, char hex[33])
{
	FILE *fp;
	char out[256];
	size_t i;
	size_t j = 0;

	fp = popen(cmd, "r");
	if (!fp)
		return -1;

	if (!fgets(out, sizeof(out), fp)) {
		pclose(fp);
		return -1;
	}
	pclose(fp);

	for (i = 0; out[i] && j < 32; i++) {
		char c = out[i];
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
			hex[j++] = c;
		}
	}

	if (j != 32) {
		return -1;
	}

	hex[32] = '\0';
	return 0;
}

int md5_hex_file(const char *path, char hex[33])
{
	char cmd[512];

	snprintf(cmd, sizeof(cmd), "md5sum '%s' 2>/dev/null", path);
	return md5_hex_from_popen(cmd, hex);
}

int md5_hex_file_sorted(const char *path, char hex[33])
{
	char cmd[512];

	snprintf(cmd, sizeof(cmd),
	    "LC_ALL=C sort '%s' 2>/dev/null | md5sum 2>/dev/null", path);
	return md5_hex_from_popen(cmd, hex);
}

int md5_hex_tree(const char *root, char hex[33])
{
	FILE *fp;
	char cmd[512];
	char out[256];
	size_t i;
	size_t j = 0;

	/* MD5 of raw tree output bytes (same intent as:
	   `tree /home/user42/101/ | md5sum - | awk '{print $1}'`).
	   Keep locale behavior identical to the VM defaults (no forced LC_ALL),
	   so the digest matches the expected constants. */
	snprintf(cmd, sizeof(cmd),
	         "tree -a '%s/' 2>/dev/null | md5sum - 2>/dev/null",
	         root);
	fp = popen(cmd, "r");
	if (!fp)
		return -1;

	if (!fgets(out, sizeof(out), fp)) {
		pclose(fp);
		return -1;
	}
	pclose(fp);

	for (i = 0; out[i] && j < 32; i++) {
		char c = out[i];
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
			hex[j++] = c;
		}
	}

	if (j != 32) {
		return -1;
	}

	hex[32] = '\0';
	return 0;
}

void decode_flag_secret(char *hex33, size_t out_len,
                        const unsigned char *enc, size_t enc_len)
{
	decode_xor(hex33, out_len, enc, enc_len, xor_key, sizeof(xor_key));
}
