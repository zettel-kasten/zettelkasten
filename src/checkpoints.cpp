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
    static MapCheckpoints mapCheckpoints/* =
        boost::assign::map_list_of
        (    500, uint256("0x2a67a6c0098895380a2ff82acda288176606c6f3c750fd99e37e81d698502abd"))
        (  14000, uint256("0x22ec417280cb773328d00de54e12778daf5b3456ffa77c6e88ab71fdaf1fa67e"))
        (  93000, uint256("0x60324bf217b223c295282aeaa7fcd38ab4e5dc5b44129564128fbaec38fed070"))
        ( 146400, uint256("0xd5f22372d5f32b18ac8d32f7e026379194b44fd560be097bca8337187adf40d6"))
        ( 191560, uint256("0xbabe3c0ed8a4ebbc2164716b4b066355cad952dfc811bec5accce0eadf92088e"))
        ( 231930, uint256("0xa8f4770e17a8c80c4ea6676b1cdf7cf1987be09cb55f0d10c45522625501dec2"))*/
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1421787289, // * UNIX timestamp of last checkpoint block
        239377,     // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        150.0     // * estimated number of transactions per day after checkpoint


    };

    static MapCheckpoints mapCheckpointsTestnet/* =
        boost::assign::map_list_of
        (   44, uint256("0000058784a8cdd3b4dc5ce684d24a1d5fecc3d47f9cd71218d04a1a322318a1"))*/
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1393373461,
        3000,
        30
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return true;

        /*const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;*/
        return true;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (fTestNet || pindex==NULL)
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
        if (fTestNet) return 0; // Testnet has no checkpoints
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
        if (fTestNet) return NULL; // Testnet has no checkpoints
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
