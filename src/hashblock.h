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
inline uint256 Hash9(const T1 pbegin, const T1 pend)

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






#endif // HASHBLOCK_H
