/* Validate grades_last5.csv under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	7, 81, 71, 0, 13, 20, 85, 70, 7, 6, 2, 0, 8, 70, 1, 92,
	69, 87, 74, 87, 82, 82, 82, 12, 74, 87, 14, 71, 85, 67, 87, 85
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/grades_last5.csv", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/grades_last5.csv "
		    "(consultez l'énoncé — data_102/)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire grades_last5.csv.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans grades_last5.csv "
		    "(5 dernières lignes de grades.csv ?)");
	}

	emit_flag_hex(got);
	return 0;
}
