/* Validate sorted_grades.csv under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	86, 92, 18, 91, 13, 66, 80, 75, 87, 6, 85, 83, 91, 21, 5, 88,
	79, 7, 16, 83, 4, 4, 2, 95, 16, 82, 87, 70, 6, 20, 8, 9
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/sorted_grades.csv", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/sorted_grades.csv "
		    "(sort -t, -k3 -n grades.csv > sorted_grades.csv depuis data_102 ?)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire sorted_grades.csv.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans sorted_grades.csv "
		    "(tri numérique par la 3e colonne — les notes — en ordre croissant ?)");
	}

	emit_flag_hex(got);
	return 0;
}
