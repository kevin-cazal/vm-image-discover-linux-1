/* Validate delivery archive on host share (content fingerprint). */
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const unsigned char expect_enc[] = {
	85, 81, 74, 6, 94, 67, 1, 66, 3, 4, 8, 92, 94, 18, 5, 91,
	64, 93, 64, 85, 4, 83, 81, 15, 21, 87, 12, 64, 3, 19, 6, 85
};

static const char exp_members[] =
	"101/\n"
	"101/links/\n"
	"101/links/ubuntu\n"
	"101/links/wikipedia_linux\n"
	"101/memos/\n"
	"101/memos/memo1.txt\n"
	"101/memos/memo2.txt\n"
	"101/secret1.txt\n"
	"101/secret2.txt\n"
	"101/works/\n"
	"101/works/essay2.txt\n";

static char *read_stream(FILE *fp)
{
	char buf[4096];
	size_t cap = 8192;
	size_t len = 0;
	char *data = malloc(cap);

	if (!data)
		return NULL;
	while (fgets(buf, sizeof(buf), fp)) {
		size_t chunk = strlen(buf);

		if (len + chunk + 1 > cap) {
			cap *= 2;
			data = realloc(data, cap);
			if (!data)
				return NULL;
		}
		memcpy(data + len, buf, chunk);
		len += chunk;
	}
	data[len] = '\0';
	return data;
}

static int content_md5(const char *archive, char hex[33])
{
	char cmd[768];
	char tmp[] = "/tmp/check_livrableXXXXXX";
	FILE *fp;
	char line[128];
	int i;

	if (!mkdtemp(tmp))
		return -1;

	snprintf(cmd, sizeof(cmd),
	    "tar -xf '%s' -C '%s' 2>/dev/null && "
	    "cd '%s' && find 101 -type f -print0 | sort -z | "
	    "xargs -0 md5sum 2>/dev/null | LC_ALL=C sort | md5sum | awk '{print $1}'",
	    archive, tmp, tmp);

	fp = popen(cmd, "r");
	if (!fp) {
		rmdir(tmp);
		return -1;
	}
	if (!fgets(line, sizeof(line), fp)) {
		pclose(fp);
		rmdir(tmp);
		return -1;
	}
	pclose(fp);

	/* remove temp tree */
	snprintf(cmd, sizeof(cmd), "rm -rf '%s'", tmp);
	(void)system(cmd);

	for (i = 0; line[i]; i++) {
		if (line[i] == '\n' || line[i] == '\r')
			line[i] = '\0';
	}
	if (strlen(line) != 32)
		return -1;
	strcpy(hex, line);
	return 0;
}

int main(void)
{
	const char *archive = "/mnt/host/.delivery_101.tar";
	char expect[40];
	char got_hex[33];
	FILE *fp;
	char *listing = NULL;

	if (!check_path_is_file(archive)) {
		fprintf(stderr,
		    "Archive introuvable : %s (copiez .delivery_101.tar dans /mnt/host)\n",
		    archive);
		exit(1);
	}

	fp = popen("tar -tf '/mnt/host/.delivery_101.tar' 2>/dev/null | "
	           "sed 's|^\\./||' | LC_ALL=C sort | tr -d '\\r'", "r");
	if (!fp) {
		check_fail_msg("Impossible de lire l'archive.");
	}
	listing = read_stream(fp);
	pclose(fp);
	if (!listing) {
		check_fail_msg("Impossible de lire l'archive.");
	}

	if (strcmp(listing, exp_members) != 0) {
		free(listing);
		check_fail_msg(
		    "Contenu de l'archive incorrect (tar -tf). "
		    "Vérifiez le dossier 101 archivé après rm/nano/mv.");
	}
	free(listing);

	if (content_md5(archive, got_hex) != 0) {
		check_fail_msg("Impossible d'extraire ou d'empreinter l'archive.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got_hex, expect) != 0) {
		check_fail_msg(
		    "Contenu des fichiers dans l'archive incorrect (empreinte md5).");
	}

	emit_flag_hex(got_hex);
	return 0;
}
