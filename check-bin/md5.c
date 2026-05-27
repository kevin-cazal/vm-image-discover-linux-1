/* Public-domain MD5 (RFC 1321), trimmed for check binaries. */
#include "md5.h"

#include <stdio.h>
#include <string.h>

#define MD5_F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | ~(z)))

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static void md5_transform(unsigned int state[4], const unsigned char block[64])
{
	unsigned int aa = state[0], bb = state[1], cc = state[2], dd = state[3];
	unsigned int x[16];
	unsigned int i;

	for (i = 0; i < 16; i++) {
		x[i] = (unsigned int)block[i * 4]
		    | ((unsigned int)block[i * 4 + 1] << 8)
		    | ((unsigned int)block[i * 4 + 2] << 16)
		    | ((unsigned int)block[i * 4 + 3] << 24);
	}

#define STEP(fn, va, vb, vc, vd, xk, s, ac) \
	do { \
		(va) += fn((vb), (vc), (vd)) + (xk) + (ac); \
		(va) = (vb) + ROTL((va), (s)); \
	} while (0)

	/* Round 1 */
	STEP(MD5_F, aa, bb, cc, dd, x[0], 7, 0xd76aa478);
	STEP(MD5_F, dd, aa, bb, cc, x[1], 12, 0xe8c7b756);
	STEP(MD5_F, cc, dd, aa, bb, x[2], 17, 0x242070db);
	STEP(MD5_F, bb, cc, dd, aa, x[3], 22, 0xc1bdceee);
	STEP(MD5_F, aa, bb, cc, dd, x[4], 7, 0xf57c0faf);
	STEP(MD5_F, dd, aa, bb, cc, x[5], 12, 0x4787c62a);
	STEP(MD5_F, cc, dd, aa, bb, x[6], 17, 0xa8304613);
	STEP(MD5_F, bb, cc, dd, aa, x[7], 22, 0xfd469501);
	STEP(MD5_F, aa, bb, cc, dd, x[8], 7, 0x698098d8);
	STEP(MD5_F, dd, aa, bb, cc, x[9], 12, 0x8b44f7af);
	STEP(MD5_F, cc, dd, aa, bb, x[10], 17, 0xffff5bb1);
	STEP(MD5_F, bb, cc, dd, aa, x[11], 22, 0x895cd7be);
	STEP(MD5_F, aa, bb, cc, dd, x[12], 7, 0x6b901122);
	STEP(MD5_F, dd, aa, bb, cc, x[13], 12, 0xfd987193);
	STEP(MD5_F, cc, dd, aa, bb, x[14], 17, 0xa679438e);
	STEP(MD5_F, bb, cc, dd, aa, x[15], 22, 0x49b40821);

	/* Round 2 */
	STEP(MD5_G, aa, bb, cc, dd, x[1], 5, 0xf61e2562);
	STEP(MD5_G, dd, aa, bb, cc, x[6], 9, 0xc040b340);
	STEP(MD5_G, cc, dd, aa, bb, x[11], 14, 0x265e5a51);
	STEP(MD5_G, bb, cc, dd, aa, x[0], 20, 0xe9b6c7aa);
	STEP(MD5_G, aa, bb, cc, dd, x[5], 5, 0xd62f105d);
	STEP(MD5_G, dd, aa, bb, cc, x[10], 9, 0x02441453);
	STEP(MD5_G, cc, dd, aa, bb, x[15], 14, 0xd8a1e681);
	STEP(MD5_G, bb, cc, dd, aa, x[4], 20, 0xe7d3fbc8);
	STEP(MD5_G, aa, bb, cc, dd, x[9], 5, 0x21e1cde6);
	STEP(MD5_G, dd, aa, bb, cc, x[14], 9, 0xc33707d6);
	STEP(MD5_G, cc, dd, aa, bb, x[3], 14, 0xf4d50d87);
	STEP(MD5_G, bb, cc, dd, aa, x[8], 20, 0x455a14ed);
	STEP(MD5_G, aa, bb, cc, dd, x[13], 5, 0xa9e3e905);
	STEP(MD5_G, dd, aa, bb, cc, x[2], 9, 0xfcefa3f8);
	STEP(MD5_G, cc, dd, aa, bb, x[7], 14, 0x676f02d9);
	STEP(MD5_G, bb, cc, dd, aa, x[12], 20, 0x8d2a4c8a);

	/* Round 3 */
	STEP(MD5_H, aa, bb, cc, dd, x[5], 4, 0xfffa3942);
	STEP(MD5_H, dd, aa, bb, cc, x[8], 11, 0x8771f681);
	STEP(MD5_H, cc, dd, aa, bb, x[11], 16, 0x6d9d6122);
	STEP(MD5_H, bb, cc, dd, aa, x[14], 23, 0xfde5380c);
	STEP(MD5_H, aa, bb, cc, dd, x[1], 4, 0xa4beea44);
	STEP(MD5_H, dd, aa, bb, cc, x[4], 11, 0x4bdecfa9);
	STEP(MD5_H, cc, dd, aa, bb, x[7], 16, 0xf6bb4b60);
	STEP(MD5_H, bb, cc, dd, aa, x[10], 23, 0xbebfbc70);
	STEP(MD5_H, aa, bb, cc, dd, x[13], 4, 0x289b7ec6);
	STEP(MD5_H, dd, aa, bb, cc, x[0], 11, 0xeaa127fa);
	STEP(MD5_H, cc, dd, aa, bb, x[3], 16, 0xd4ef3085);
	STEP(MD5_H, bb, cc, dd, aa, x[6], 23, 0x04881d05);
	STEP(MD5_H, aa, bb, cc, dd, x[9], 4, 0xd9d4d039);
	STEP(MD5_H, dd, aa, bb, cc, x[12], 11, 0xe6db99e5);
	STEP(MD5_H, cc, dd, aa, bb, x[15], 16, 0x1fa27cf8);
	STEP(MD5_H, bb, cc, dd, aa, x[2], 23, 0xc4ac5665);

	/* Round 4 */
	STEP(MD5_I, aa, bb, cc, dd, x[0], 6, 0xf4292244);
	STEP(MD5_I, dd, aa, bb, cc, x[7], 10, 0x432aff97);
	STEP(MD5_I, cc, dd, aa, bb, x[14], 15, 0xab9423a7);
	STEP(MD5_I, bb, cc, dd, aa, x[5], 21, 0xfc93a039);
	STEP(MD5_I, aa, bb, cc, dd, x[12], 6, 0x655b59c3);
	STEP(MD5_I, dd, aa, bb, cc, x[3], 10, 0x8f0ccc92);
	STEP(MD5_I, cc, dd, aa, bb, x[10], 15, 0xffeff47d);
	STEP(MD5_I, bb, cc, dd, aa, x[1], 21, 0x85845dd1);
	STEP(MD5_I, aa, bb, cc, dd, x[8], 6, 0x6fa87e4f);
	STEP(MD5_I, dd, aa, bb, cc, x[15], 10, 0xfe2ce6e0);
	STEP(MD5_I, cc, dd, aa, bb, x[6], 15, 0xa3014314);
	STEP(MD5_I, bb, cc, dd, aa, x[13], 21, 0x4e0811a1);
	STEP(MD5_I, aa, bb, cc, dd, x[4], 6, 0xf7537e82);
	STEP(MD5_I, dd, aa, bb, cc, x[11], 10, 0xbd3af235);
	STEP(MD5_I, cc, dd, aa, bb, x[2], 15, 0x2ad7d2bb);
	STEP(MD5_I, bb, cc, dd, aa, x[9], 21, 0xeb86d391);

#undef STEP

	state[0] += aa;
	state[1] += bb;
	state[2] += cc;
	state[3] += dd;
}

void md5_init(MD5_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xefcdab89;
	ctx->state[2] = 0x98badcfe;
	ctx->state[3] = 0x10325476;
}

void md5_update(MD5_CTX *ctx, const unsigned char *data, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++) {
		ctx->data[ctx->datalen++] = data[i];
		if (ctx->datalen == 64) {
			md5_transform(ctx->state, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void md5_final(unsigned char digest[16], MD5_CTX *ctx)
{
	unsigned int i = ctx->datalen;
	unsigned char pad[64];

	if (ctx->datalen < 56) {
		pad[0] = 0x80;
		memset(pad + 1, 0, 55 - ctx->datalen);
		md5_update(ctx, pad, 56 - ctx->datalen);
	} else {
		pad[0] = 0x80;
		memset(pad + 1, 0, 63 - ctx->datalen);
		md5_update(ctx, pad, 64 - ctx->datalen);
		pad[0] = 0;
		memset(pad, 0, 56);
		md5_update(ctx, pad, 56);
	}

	ctx->bitlen += (unsigned long long)ctx->datalen * 8;
	for (i = 0; i < 8; i++) {
		pad[i] = (unsigned char)(ctx->bitlen >> (i * 8));
	}
	md5_update(ctx, pad, 8);

	for (i = 0; i < 4; i++) {
		digest[i * 4] = (unsigned char)(ctx->state[i] & 0xff);
		digest[i * 4 + 1] = (unsigned char)((ctx->state[i] >> 8) & 0xff);
		digest[i * 4 + 2] = (unsigned char)((ctx->state[i] >> 16) & 0xff);
		digest[i * 4 + 3] = (unsigned char)((ctx->state[i] >> 24) & 0xff);
	}
}

void md5_hex_digest(const unsigned char digest[16], char out[33])
{
	static const char hex[] = "0123456789abcdef";
	size_t i;

	for (i = 0; i < 16; i++) {
		out[i * 2] = hex[digest[i] >> 4];
		out[i * 2 + 1] = hex[digest[i] & 0x0f];
	}
	out[32] = '\0';
}
