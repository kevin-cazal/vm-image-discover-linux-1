/* Validate cars_without_personal_info.csv under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	86, 10, 21, 1, 14, 69, 6, 70, 83, 86, 3, 93, 11, 75, 84, 93,
	70, 92, 75, 2, 85, 9, 82, 89, 21, 83, 89, 64, 87, 68, 6, 0
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/cars_without_personal_info.csv",
	    home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/cars_without_personal_info.csv "
		    "(consultez l'énoncé — data_102/)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire cars_without_personal_info.csv.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans cars_without_personal_info.csv "
		    "(4 premières colonnes de cars.csv ?)");
	}

	emit_flag_hex(got);
	return 0;
}
