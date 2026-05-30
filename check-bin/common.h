#ifndef CHECK_COMMON_H
#define CHECK_COMMON_H

#include <stddef.h>

#define CHECK_HOME_DEFAULT "/home/user42"

const char *check_home(void);
void check_err(const char *msg);
void check_fail_msg(const char *msg);

int check_path_is_file(const char *path);
int check_path_is_dir(const char *path);
int check_path_absent(const char *path);

void decode_xor(char *out, size_t out_len,
                const unsigned char *enc, size_t enc_len,
                const unsigned char *key, size_t key_len);

/* Decode XOR-obfuscated 32-char hex digest (key in common.c). */
void decode_flag_secret(char *hex33, size_t out_len,
                        const unsigned char *enc, size_t enc_len);

void emit_flag_hex(const char *hex32);
void emit_flag_body(const char *body);

int md5_hex_file(const char *path, char hex[33]);
int md5_hex_file_sorted(const char *path, char hex[33]);
int md5_hex_tree(const char *root, char hex[33]);

#endif
