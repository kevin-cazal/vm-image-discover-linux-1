/* Validate grep avance outputs: CSV in ~/data_102_delivery, archive on /mnt/host. */
#include "common.h"

#include <stdio.h>
#include <string.h>

static const char *work_dir(void)
{
	static char dir[320];
	const char *home = check_home();

	snprintf(dir, sizeof(dir), "%s/data_102_delivery", home);
	return dir;
}

static void path_in_work(char *out, size_t out_len, const char *name)
{
	snprintf(out, out_len, "%s/%s", work_dir(), name);
}

static int count_lines(const char *path)
{
	FILE *fp = fopen(path, "r");
	char line[512];
	int n = 0;

	if (!fp)
		return -1;
	while (fgets(line, sizeof(line), fp))
		n++;
	fclose(fp);
	return n;
}

static int md5_member_of_tar_sorted(const char *archive, const char *member,
                                    const char *expect_hex)
{
	char cmd[768];
	char got[33];

	snprintf(cmd, sizeof(cmd),
	    "tar -xOf '%s' '%s' 2>/dev/null | LC_ALL=C sort | md5sum | awk '{print $1}'",
	    archive, member);
	FILE *fp = popen(cmd, "r");
	if (!fp)
		return -1;
	if (!fgets(got, sizeof(got), fp)) {
		pclose(fp);
		return -1;
	}
	pclose(fp);
	got[strcspn(got, "\r\n")] = '\0';
	if (strlen(got) != 32)
		return -1;
	if (strcmp(got, expect_hex) != 0)
		return -1;
	return 0;
}

static int tar_has_members(const char *archive)
{
	char cmd[512];
	char line[256];
	int n = 0;

	snprintf(cmd, sizeof(cmd),
	    "tar -tzf '%s' 2>/dev/null | LC_ALL=C sort", archive);
	FILE *fp = popen(cmd, "r");
	if (!fp)
		return 0;
	while (fgets(line, sizeof(line), fp)) {
		if (strstr(line, "all_data.csv\n") ||
		    strstr(line, "all_data_valid_ip.csv\n") ||
		    strstr(line, "all_data_invalid_ip.csv\n"))
			n++;
	}
	pclose(fp);
	return n == 3;
}

int main(void)
{
	char valid_path[320];
	char invalid_path[320];
	char flag_body[32];
	char got[33];
	const char *expect_valid = "4902672a4a5d3ee1eef469f07cce1364";
	const char *expect_invalid = "e1d9c403c9abba836c0d65ca51f043cf";
	const char *expect_source = "f346c3a39948bb6b4f811a853548e229";
	const char *source = "/mnt/host/all_data.csv";
	const char *archive = "/mnt/host/all_data.tar.gz";
	int n_valid;
	int n_invalid;

	path_in_work(valid_path, sizeof(valid_path), "all_data_valid_ip.csv");
	path_in_work(invalid_path, sizeof(invalid_path), "all_data_invalid_ip.csv");

	if (!check_path_is_file(source)) {
		check_fail_msg(
		    "Fichier source introuvable : /mnt/host/all_data.csv "
		    "(exercice concat CSV sur /mnt/host ?)");
	}

	if (!check_path_is_file(valid_path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102_delivery/all_data_valid_ip.csv");
	}

	if (!check_path_is_file(invalid_path)) {
		check_fail_msg(
		    "Fichier manquant : ~/data_102_delivery/all_data_invalid_ip.csv");
	}

	if (!check_path_is_file(archive)) {
		check_fail_msg(
		    "Archive manquante : /mnt/host/all_data.tar.gz "
		    "(tar.gz avec all_data.csv, all_data_valid_ip.csv et all_data_invalid_ip.csv ?)");
	}

	if (md5_hex_file_sorted(valid_path, got) != 0) {
		check_fail_msg("Impossible de lire all_data_valid_ip.csv.");
	}
	if (strcmp(got, expect_valid) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans all_data_valid_ip.csv "
		    "(lignes e-mail de all_data.csv, IP sans 0. en début ni .0 en fin ?)");
	}

	if (md5_hex_file_sorted(invalid_path, got) != 0) {
		check_fail_msg("Impossible de lire all_data_invalid_ip.csv.");
	}
	if (strcmp(got, expect_invalid) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans all_data_invalid_ip.csv "
		    "(IP commençant par 0. ou se terminant par .0 ?)");
	}

	if (!tar_has_members(archive)) {
		check_fail_msg(
		    "all_data.tar.gz doit contenir all_data.csv, "
		    "all_data_valid_ip.csv et all_data_invalid_ip.csv.");
	}

	if (md5_member_of_tar_sorted(archive, "all_data.csv", expect_source) != 0 ||
	    md5_member_of_tar_sorted(archive, "all_data_valid_ip.csv",
	                              expect_valid) != 0 ||
	    md5_member_of_tar_sorted(archive, "all_data_invalid_ip.csv",
	                              expect_invalid) != 0) {
		check_fail_msg(
		    "Contenu inattendu dans all_data.tar.gz "
		    "(mêmes fichiers que sur le disque ?).");
	}

	n_valid = count_lines(valid_path);
	n_invalid = count_lines(invalid_path);
	if (n_valid < 0 || n_invalid < 0) {
		check_fail_msg("Impossible de compter les adresses IP.");
	}

	snprintf(flag_body, sizeof(flag_body), "%d %d", n_valid, n_invalid);
	emit_flag_body(flag_body);
	return 0;
}
