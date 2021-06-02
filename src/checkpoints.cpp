// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (   1337, uint256("0xa89eec7b93ca8f8c2c5e09e275915dca9beb59f112be530ef68bdbc2b80ace8c"))
        (  50000, uint256("0x8b2fe8b2164a4feff5bed53d476b1d6e05bdde75911bbd7de8f3b17abd4373db"))
        ( 100000, uint256("0x538114f1f3bf9b2bbad7f0760f1bbf00fac662b6dfe5dd67a8cbc267ff0c8850"))
        ( 150000, uint256("0x977d1e187965905a1fea59b0ff97335ec04e0910c1461c8f772bd31397bc70c8"))
        ( 200000, uint256("0xd399af477980508208116b96a637077a9ce903fac811756cb61aa4b788cd3322"))
        ( 250000, uint256("0x81c3cbf6be7ef9012b5129b1c5fc5ed266efc017f35a3a05bb7cb0575461a7f3"))
        ( 300000, uint256("0xe158217eb04dc2692c1e5567601d5ede59aea67dbef10961a5bb8bf84adf7e79"))
        ( 350000, uint256("0x500101259d892f71329691f00ddfc1795ad5fbc301485b83b88c44a163dad82e"))
        ( 400000, uint256("0x16cb34795f5dac1e3a3e509c276b93a6ec58af312442f9603431319f5907101c"))
        ( 450000, uint256("0xade6dcc4d0d38fe4c34da8c0b11df2fab1457e771c04fcbf44311b7b56e9a214"))
        ( 500000, uint256("0x9b22c590482a841947e1f0c6d09a7fd4d018a05fc3845468fc6801c0bc2ac698"))
        ( 555555, uint256("0x0b3ed16c8d5adc5935e64d809c69ba816fc88eac3d06608468ab1765bffebe95"))
        ( 600000, uint256("0x16c80186a8c193837dab985722f4c5f29b661db36725ba204ab81ab70694e819"))
        ( 650000, uint256("0xfa2d90b68dfa295917093ef516e5e1c784d65f350645b8d02717574551814f3e"))
        ( 700000, uint256("0x1d89d1854a20e1c8a89550536e314577aa79f8634482b36f7d163b06272bbace"))
        ( 750000, uint256("0xe82ab6c1f2ad25238d9e94c51ae63cac40883322c87861a5a633d76c4c5de778"))
        ( 800000, uint256("0xd62b6a4107eb4cb0ac299967c51f3d3f8c6c743aba27f4a664be927b147f1828"))
        ( 850000, uint256("0xd1cbc544338eb8465b6ac34c54466a86c43d05383cbc5948cd8c16391de18fdc"))
        ( 900000, uint256("0x43e62f62fce264e5d5a761bfa06b1d35620a83b3b7c1b040e5164416efb92e05"))
        ( 950000, uint256("0xb38dc77cd4ba2b9d2422fdded893268c9d607cde593bf70c3c7ba183cd1f17fe"))
        ( 1000000, uint256("0x3d48e38e8fb151ca5c03e69276e10253423d9b9e8c0a2610b0c5c04f6bb46aaf"))
        ( 1050000, uint256("0x0788a7276ced9a788329514277328b3ba6673e29f077074db2339f6bccb13d92"))
        ( 1100000, uint256("0x319b254789a8a3386bbd1f8015bd52847704ed75f4e84841231713269ea95f43"))
        ( 1150000, uint256("0x8e1221af67513b97943ea78f2a71928fd24cef9d5033e7f2cb797055e89ebeb5"))
        ( 1200000, uint256("0xd32b09c3f172f5fc21dd2a974adba0a09b5b88018bae28bbc37bcc917493966d"))
        ( 1250000, uint256("0x2494673027015ab5789eda34f0893bded5a1590ad3527b803d89fcbe25a25918"))
        ( 1300000, uint256("0x4a5a742fe4334aeecf6b66afc87a273d4f1d69b654215fb2d7bb4ff82eb45a69"))
        ( 1350000, uint256("0xd7933980a3a39c8eb84890ac34334e3dbdc1eecf4230200d403f7041f65fe06c"))
        ( 1400000, uint256("0xad15110db4b923ede1a547c5e0e8f41b39826d7a0e13c7e8c9c946cb0df03951"))
        ( 1450000, uint256("0x1999e746a949abcda3c20cc5c051529ed97c39bf758e7f9e226900724eef701f"))
        ( 1500000, uint256("0x4408e64fc66d4e10f7ab910ec3753e13721a66711c421594602776da1881062e"))
        ( 1550000, uint256("0x20439229bfcf10982690f3fd231b1b23c16d7c4714aeb557e304ac0fdae63ad1"))
        ( 1600000, uint256("0x5a2d615d89363d71747b0cd1cba952c017469c2f6e592a362faf7b9346f456dd"))
        ( 1650000, uint256("0x16509ca7f8f488aeda3e99eb2a7b233bd723e8a8dde29c30b588f58854edddca"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1421787289, // * UNIX timestamp of last checkpoint block
        239377,     // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        150.0     // * estimated number of transactions per day after checkpoint


    };

    const CCheckpointData &Checkpoints() {
        return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
        //return true;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        /*const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);*/
        return 0.0;
    }

    int GetTotalBlocksEstimate()
    {
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        //const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        //return checkpoints.rbegin()->first;
        return 0;

    }

    unsigned int LastCheckPoint()
    {
        if (Checkpoints().mapCheckpoints->empty())
            return 0;

        //return Checkpoints().mapCheckpoints->rbegin()->first;
        return 0;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        /*const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }*/
        return NULL;
    }
}
