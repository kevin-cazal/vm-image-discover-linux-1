/* Validate concatenated CSV at /mnt/host/all_data.csv */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	2, 90, 71, 85, 12, 69, 4, 65, 8, 9, 5, 92, 11, 17, 85, 13,
	66, 3, 74, 0, 1, 80, 92, 92, 64, 86, 91, 78, 0, 64, 3, 9
};
/* expect: md5 of `LC_ALL=C sort all_data.csv` (order-independent) */

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

	if (md5_hex_file_sorted(path, got) != 0) {
		check_fail_msg("Impossible de lire /mnt/host/all_data.csv.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans /mnt/host/all_data.csv "
		    "(concaténez tous les .csv de data_102_delivery, sans all_data.csv ?)");
	}

	emit_flag_hex(got);
	return 0;
}
