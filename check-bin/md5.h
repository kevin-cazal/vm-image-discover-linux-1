#ifndef CHECK_MD5_H
#define CHECK_MD5_H

#include <stddef.h>

typedef struct {
	unsigned char data[64];
	unsigned int datalen;
	unsigned long long bitlen;
	unsigned int state[4];
} MD5_CTX;

void md5_init(MD5_CTX *ctx);
void md5_update(MD5_CTX *ctx, const unsigned char *data, size_t len);
void md5_final(unsigned char digest[16], MD5_CTX *ctx);
void md5_hex_digest(const unsigned char digest[16], char out[33]);

#endif
