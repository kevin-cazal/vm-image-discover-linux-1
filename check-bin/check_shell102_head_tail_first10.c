/* Validate grades_first10.txt under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	5, 12, 64, 87, 94, 65, 7, 65, 2, 4, 9, 2, 13, 67, 84, 14,
	66, 86, 16, 84, 6, 87, 85, 90, 66, 91, 93, 70, 84, 65, 9, 1
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/grades_first10.txt", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/grades_first10.txt "
		    "(head -10 grades.csv > grades_first10.txt depuis data_102 ?)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire grades_first10.txt.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans grades_first10.txt "
		    "(10 premières lignes de grades.csv ?)");
	}

	emit_flag_hex(got);
	return 0;
}
