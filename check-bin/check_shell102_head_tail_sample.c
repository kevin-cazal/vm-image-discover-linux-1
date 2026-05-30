/* Validate grades_sample.txt under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	7, 91, 67, 85, 94, 78, 82, 70, 87, 9, 6, 86, 91, 16, 82, 10,
	64, 81, 70, 2, 5, 83, 93, 81, 68, 5, 87, 70, 7, 20, 85, 82
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/grades_sample.txt", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/grades_sample.txt "
		    "(consultez l'énoncé — data_102/)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire grades_sample.txt.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans grades_sample.txt "
		    "(12 lignes à partir de la 5e ligne hors en-tête ?)");
	}

	emit_flag_hex(got);
	return 0;
}
