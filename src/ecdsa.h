#ifndef ECDSA_H
#define ECDSA_H

#include "bignum.h"

// Faster ECDSA signer for mining.
// DO NOT sign multiple messages with the same instance of this class, this is not safe.
// It is ok to sign many different hashes during mining because only one of it will be broadcasted.
// See https://en.wikipedia.org/wiki/Elliptic_Curve_DSA beginning with the words
// "As the standard notes, it is crucial to select different k for different signatures".
class CSignerECDSA
{
    CAutoBN_CTX ctx;

    CBigNum order;
    CBigNum kinv;
    CBigNum pmr;
    CBigNum prk;

public:

    std::string GetPMR()
    {
        return pmr.ToString(16);
    }

    std::string GetKInv()
    {
        return kinv.ToString(16);
    }

    std::string GetPRK()
    {
        return prk.ToString(16);
    }

    // Initialize signer and part of signature with random data which is not depended on message being signed.
    CSignerECDSA(const uint8_t PrivData[32], unsigned char Signature[65]);

    // Initialize rest of signature with data specific to message being signed.
    void SignFast(const uint256 &hash, unsigned char Signature[65]);
};

#endif // ECDSA_H
