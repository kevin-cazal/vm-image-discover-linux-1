/* Validate concatenated CSV at /mnt/host/all_data.csv */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	93, 15, 68, 1, 11, 70, 1, 20, 3, 1, 84, 85, 11, 71, 84, 86,
	66, 80, 64, 2, 2, 5, 81, 81, 22, 87, 95, 16, 4, 67, 0, 86
};

int main(void)
{
	const char *path = "/mnt/host/all_data.csv";
	char got[33];
	char expect[40];

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : /mnt/host/all_data.csv "
		    "(concaténez les .csv de data_102_delivery puis copiez sur /mnt/host ?)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire /mnt/host/all_data.csv.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans /mnt/host/all_data.csv "
		    "(find . -name '*.csv' -exec cat {} + depuis data_102_delivery ?)");
	}

	emit_flag_hex(got);
	return 0;
}
