/* Validate w0nd3rl4nd.txt under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	5, 10, 65, 0, 12, 67, 86, 20, 6, 83, 4, 1, 80, 66, 1, 87,
	69, 7, 16, 7, 83, 1, 85, 10, 22, 91, 93, 20, 3, 17, 7, 2
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/w0nd3rl4nd.txt", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/w0nd3rl4nd.txt "
		    "(sed 's/Alice/4l1c3/g' wonderland.txt > w0nd3rl4nd.txt ?)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire w0nd3rl4nd.txt.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans w0nd3rl4nd.txt "
		    "(remplacement Alice → 4l1c3 dans wonderland.txt ?)");
	}

	emit_flag_hex(got);
	return 0;
}
