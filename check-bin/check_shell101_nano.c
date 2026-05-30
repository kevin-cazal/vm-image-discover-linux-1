/* Validate edited ~/101/memos/memo2.txt after nano exercise. */
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char flag_enc[] = {
	6, 92, 66, 6, 10, 19, 4, 64, 8, 2, 1, 5, 13, 74, 5, 92,
	79, 93, 68, 5, 84, 0, 86, 80, 16, 5, 12, 23, 6, 17, 5, 2
};

static const char fan[] =
    "- Écrire une fanfiction de 3 phrases entre une brique et un grille-pain";
/* e2e appends the line with a leading "- " (see tests/e2e/scenarios/shell101.mjs). */
static const char last[] = "- Convaincre un chat que je suis le chef";

static int file_contains_line(const char *path, const char *needle)
{
	FILE *fp;
	char line[512];
	int count = 0;

	fp = fopen(path, "r");
	if (!fp)
		return -1;
	while (fgets(line, sizeof(line), fp)) {
		size_t len = strlen(line);

		while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
			line[--len] = '\0';
		/* Mimic `grep -cF "$needle"`: count matching lines by substring. */
		if (strstr(line, needle) != NULL)
			count++;
	}
	fclose(fp);
	return count;
}

static int last_line_matches(const char *path, const char *needle)
{
	FILE *fp;
	char line[512];
	char prev[512];
	int found = 0;

	fp = fopen(path, "r");
	if (!fp)
		return -1;
	prev[0] = '\0';
	while (fgets(line, sizeof(line), fp)) {
		size_t len = strlen(line);

		while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
			line[--len] = '\0';
		if (line[0])
			strcpy(prev, line);
	}
	fclose(fp);
	if (prev[0] && strcmp(prev, needle) == 0)
		found = 1;
	return found;
}

int main(void)
{
	char path[320];
	char expect[40];
	const char *home = check_home();
	int fan_count;

	snprintf(path, sizeof(path), "%s/101/memos/memo2.txt", home);

	if (!check_path_is_file(path)) {
		check_fail_msg(
		    "Fichier introuvable : ~/101/memos/memo2.txt "
		    "(avez-vous fait l'exercice cp et mv ?)");
	}

	if (!last_line_matches(path, last)) {
		check_fail_msg(
		    "La dernière ligne du fichier ne correspond pas à l'énoncé.");
	}

	fan_count = file_contains_line(path, fan);
	if (fan_count < 0) {
		check_fail_msg("Impossible de lire le fichier memo2.txt.");
	}
	if (fan_count > 1) {
		check_fail_msg(
		    "Ligne en double encore présente : fanfiction brique / grille-pain");
	}
	if (fan_count < 1) {
		fprintf(stderr, "Ligne fanfiction attendue introuvable dans %s\n",
		    path);
		exit(1);
	}

	decode_flag_secret(expect, sizeof(expect), flag_enc, sizeof(flag_enc));
	emit_flag_hex(expect);
	return 0;
}
