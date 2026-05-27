/* Validate post-rm layout under ~/101. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const unsigned char expect_enc[] = {
	83, 89, 23, 91, 13, 16, 83, 16, 0, 8, 84, 2, 13, 68, 90, 91,
	79, 1, 67, 1, 86, 4, 0, 95, 66, 87, 87, 67, 87, 68, 5, 82
};

int main(void)
{
	char root[256];
	char path[320];
	char got[33];
	char expect[40];
	const char *home = check_home();

	snprintf(root, sizeof(root), "%s/101", home);

	snprintf(path, sizeof(path), "%s/code", root);
	if (!check_path_absent(path)) {
		check_fail_msg(
		    "Le dossier code/ est encore présent sous ~/101 (utilisez rm -r).");
	}

	snprintf(path, sizeof(path), "%s/works/essay1.txt", root);
	if (!check_path_absent(path)) {
		check_fail_msg("Le fichier works/essay1.txt est encore présent.");
	}

	snprintf(path, sizeof(path), "%s/works/essay2.txt", root);
	if (!check_path_is_file(path)) {
		check_fail_msg("Fichier manquant : ~/101/works/essay2.txt");
	}

	if (md5_hex_file(path, got) != 0) {
		check_fail_msg("Impossible de lire works/essay2.txt.");
	}

	decode_flag_secret(expect, sizeof(expect), expect_enc, sizeof(expect_enc));
	if (strcmp(got, expect) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans works/essay2.txt "
		    "(essay1 supprimé mais autre modification ?).");
	}

	emit_flag_hex(got);
	return 0;
}
