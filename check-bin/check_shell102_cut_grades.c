/* Validate grades_only.txt under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	7, 95, 67, 1, 87, 64, 3, 23, 6, 3, 7, 84, 11, 75, 90, 92,
	16, 80, 16, 1, 85, 87, 93, 91, 70, 84, 91, 23, 84, 70, 82, 86
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/grades_only.txt", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/grades_only.txt "
		    "(consultez l'énoncé — data_102/)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire grades_only.txt.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans grades_only.txt "
		    "(3e colonne — les notes — de grades.csv, sans en-tête ?)");
	}

	emit_flag_hex(got);
	return 0;
}
