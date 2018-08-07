#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "uint256.h"
#include "sph_keccak.h"

//#include <QDebug>

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

static unsigned char SDKPGABSPC_sinetable[] =
{
	0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x05, 0x07,
	0x09, 0x0C, 0x0E, 0x11, 0x14, 0x17, 0x1A, 0x1E,
	0x21, 0x24, 0x27, 0x2A, 0x2D, 0x30, 0x33, 0x36,
	0x38, 0x3A, 0x3C, 0x3D, 0x3F, 0x3F, 0x40, 0x40,
	0x40, 0x40, 0x3F, 0x3F, 0x3D, 0x3C, 0x3A, 0x38,
	0x36, 0x33, 0x30, 0x2D, 0x2A, 0x27, 0x24, 0x21,
	0x1E, 0x1A, 0x17, 0x14, 0x11, 0x0E, 0x0C, 0x09,
	0x07, 0x05, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00,
};

template<typename T1>
inline uint256 HashSDK(const T1 pbegin, const T1 pend)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];


	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGAB_EVEN(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	hash[1] =~hash[1];

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGAB_ODD(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	hash[0] =~hash[0];

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}





template<typename T1>
inline uint256 HashSDKPGABSPC_EVEN(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	hash[1] =~hash[1];

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGABSPC_ODD(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	hash[0] =~hash[0];

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


#endif // HASHBLOCK_H
