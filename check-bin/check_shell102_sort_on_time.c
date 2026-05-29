/* Validate on_time_flights_sorted.csv under ~/data_102. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	83, 91, 66, 6, 13, 21, 86, 22, 1, 7, 84, 7, 8, 67, 83, 12,
	19, 3, 74, 83, 7, 3, 80, 93, 64, 5, 11, 65, 82, 16, 4, 81
};

int main(void)
{
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(path, sizeof(path), "%s/data_102/on_time_flights_sorted.csv", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102/on_time_flights_sorted.csv "
		    "(grep on-time, sort -t, -k9 -n, redirection depuis data_102 ?)");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire on_time_flights_sorted.csv.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans on_time_flights_sorted.csv "
		    "(vols on-time triés par nombre de passagers croissant ?)");
	}

	emit_flag_hex(got);
	return 0;
}
