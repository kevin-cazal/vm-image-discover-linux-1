/* Validate grades_first10.csv under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	6, 81, 74, 86, 86, 19, 83, 71, 5, 6, 83, 5, 11, 18, 7, 86,
	67, 3, 19, 82, 82, 80, 85, 92, 67, 82, 87, 66, 6, 22, 5, 0
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/grades_first10.csv", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/grades_first10.csv "
		    "(consultez l'énoncé — data_102/)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire grades_first10.csv.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans grades_first10.csv "
		    "(10 premières lignes de données de grades.csv, sans en-tête ?)");
	}

	emit_flag_hex(got);
	return 0;
}
