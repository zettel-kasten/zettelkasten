#include <stdint.h>
#include <assert.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include "uint256.h"
#include "bignum.h"
#include "key.h"
#include "ecdsa.h"

// Group order of secp256k1
static const uint256 g_Order("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");

// Necessary function from key.cpp
extern int EC_KEY_regenerate_key(EC_KEY *eckey, BIGNUM *priv_key);

CSignerECDSA::CSignerECDSA(const uint8_t PrivData[32], unsigned char Signature[65])
{
    order.setuint256(g_Order);

    EC_KEY* pkey = EC_KEY_new_by_curve_name(NID_secp256k1);
    const EC_GROUP *group = EC_KEY_get0_group(pkey);

    CBigNum privkey;
    BN_bin2bn(PrivData, 32, &privkey);
    EC_KEY_regenerate_key(pkey, &privkey);

    EC_POINT *tmp_point = EC_POINT_new(group);
    EC_POINT *test_point = EC_POINT_new(group);

    CBigNum r, X, Y;
    bool which = false;
    do
    {
        // get random k
        do
            BN_rand_range(&kinv, &order);
        while (!kinv);

        /* We do not want timing information to leak the length of k,
         * so we compute G*k using an equivalent scalar of fixed
         * bit-length. */
        kinv += order;
        if (BN_num_bits(&kinv) <= 256)
            kinv += order;

        // compute r the x-coordinate of generator * k
        EC_POINT_mul(group, tmp_point, &kinv, NULL, NULL, ctx);
        EC_POINT_get_affine_coordinates_GFp(group, tmp_point, &X, &Y, ctx);
        EC_POINT_set_compressed_coordinates_GFp(group, test_point, &X, 0, ctx);
        which = !!EC_POINT_cmp(group, tmp_point, test_point, ctx);
        BN_nnmod(&r, &X, &order, ctx);
    }
    while (!r);

    // compute the inverse of k
    BN_mod_inverse(&kinv, &kinv, &order, ctx);

    BN_mod_mul(&pmr, &privkey, &r, &order, ctx);

    BN_mod_mul(&prk, &pmr, &kinv, &order, ctx);

    memset(Signature, 0, 65);
    int nBitsR = BN_num_bits(&r);
    BN_bn2bin(&r, &Signature[33-(nBitsR+7)/8]);
    Signature[0] = 27 + which;

    EC_POINT_free(tmp_point);
    EC_POINT_free(test_point);
    EC_KEY_free(pkey);
}

void CSignerECDSA::SignFast(const uint256 &hash, unsigned char Signature[65])
{
    CBigNum m;
    BN_bin2bn((uint8_t*)&hash, 256/8, &m);

    // Spread-FIXME: replace with fixed-size arithmetic.
    CBigNum s;
    BN_mod_add_quick(&s, &pmr, &m, &order);
    BN_mod_mul(&s, &s, &kinv, &order, ctx);

    int nBitsS = BN_num_bits(&s);
    memset(Signature + 33, 0, 32);
    BN_bn2bin(&s, &Signature[65-(nBitsS+7)/8]);
}
