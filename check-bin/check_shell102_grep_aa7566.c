/* Validate flight_AA7566_info.txt under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	86, 13, 16, 85, 94, 18, 3, 16, 0, 5, 7, 93, 95, 66, 81, 10,
	66, 87, 19, 80, 86, 82, 84, 94, 69, 86, 88, 66, 4, 17, 8, 1
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/flight_AA7566_info.txt", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/flight_AA7566_info.txt "
		    "(consultez l'énoncé — data_102/)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire flight_AA7566_info.txt.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans flight_AA7566_info.txt "
		    "(vérifiez grep, la redirection et le numéro de vol AA7566).");
	}

	emit_flag_hex(got);
	return 0;
}
