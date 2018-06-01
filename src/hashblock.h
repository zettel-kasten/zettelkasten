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


#endif // HASHBLOCK_H
