// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "db.h"
#include "init.h"
#include "bitcoinrpc.h"
#include "wallet.h"
#include "ecdsa.h"

using namespace json_spirit;
using namespace std;

// Return average network hashes per second based on the last 'lookup' blocks,
// or from the last difficulty change if 'lookup' is nonpositive.
// If 'height' is nonnegative, compute the estimate at the time when a given block was found.
Value GetNetworkHashPS(int lookup, int height) {
    CBlockIndex *pb = pindexBest;

    if (height >= 0 && height < nBestHeight)
        pb = FindBlockByHeight(height);

    if (pb == NULL || !pb->nHeight)
        return 0;

    // If lookup is -1, then use blocks since last difficulty change.
    if (lookup <= 0)
        lookup = pb->nHeight % 2016 + 1;

    // If lookup is larger than chain, then set it to chain length.
    if (lookup > pb->nHeight)
        lookup = pb->nHeight;

    CBlockIndex *pb0 = pb;
    int64 minTime = pb0->GetBlockTime();
    int64 maxTime = minTime;
    for (int i = 0; i < lookup; i++) {
        pb0 = pb0->pprev;
        int64 time = pb0->GetBlockTime();
        minTime = std::min(time, minTime);
        maxTime = std::max(time, maxTime);
    }

    // In case there's a situation where minTime == maxTime, we don't want a divide by zero exception.
    if (minTime == maxTime)
        return 0;

    uint256 workDiff = pb->nChainWork - pb0->nChainWork;
    int64 timeDiff = maxTime - minTime;

    return (boost::int64_t)(workDiff.getdouble() / timeDiff);
}

Value getnetworkhashps(const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 2)
        throw runtime_error(
            "getnetworkhashps [blocks] [height]\n"
            "Returns the estimated network hashes per second based on the last 120 blocks.\n"
            "Pass in [blocks] to override # of blocks, -1 specifies since last difficulty change.\n"
            "Pass in [height] to estimate the network speed at the time when a certain block was found.");

    return GetNetworkHashPS(params.size() > 0 ? params[0].get_int() : 120, params.size() > 1 ? params[1].get_int() : -1);
}


// Key used by getwork/getblocktemplate miners.
// Allocated in InitRPCMining, free'd in ShutdownRPCMining
static CReserveKey* pMiningKey = NULL;

void InitRPCMining()
{
    if (!pwalletMain)
        return;

    // getwork/getblocktemplate mining rewards paid here:
    pMiningKey = new CReserveKey(pwalletMain);
}

void ShutdownRPCMining()
{
    if (!pMiningKey)
        return;

    delete pMiningKey; pMiningKey = NULL;
}

Value getgenerate(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getgenerate\n"
            "Returns true or false.");

    if (!pMiningKey)
        return false;

    return GetBoolArg("-gen");
}


Value setgenerate(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
        throw runtime_error(
            "setgenerate <generate> [genproclimit]\n"
            "<generate> is true or false to turn generation on or off.\n"
            "Generation is limited to [genproclimit] processors, -1 is unlimited.");

    bool fGenerate = true;
    if (params.size() > 0)
        fGenerate = params[0].get_bool();

    if (params.size() > 1)
    {
        int nGenProcLimit = params[1].get_int();
        mapArgs["-genproclimit"] = itostr(nGenProcLimit);
    }
    mapArgs["-gen"] = (fGenerate ? "1" : "0");

    assert(pwalletMain != NULL);
    GenerateBitcoins(fGenerate, pwalletMain);
    return Value::null;
}


Value gethashespersec(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "gethashespersec\n"
            "Returns a recent hashes per second performance measurement while generating.");

    if (GetTimeMillis() - nHPSTimerStart > 8000)
        return (boost::int64_t)0;
    return (boost::int64_t)dHashesPerSec;
}


Value getmininginfo(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getmininginfo\n"
            "Returns an object containing mining-related information.");

    Object obj;
    obj.push_back(Pair("blocks",        (int)nBestHeight));
    obj.push_back(Pair("currentblocksize",(uint64_t)nLastBlockSize));
    obj.push_back(Pair("currentblocktx",(uint64_t)nLastBlockTx));
    obj.push_back(Pair("difficulty",    (double)GetDifficulty()));
    obj.push_back(Pair("errors",        GetWarnings("statusbar")));
    obj.push_back(Pair("generate",      GetBoolArg("-gen")));
    obj.push_back(Pair("genproclimit",  (int)GetArg("-genproclimit", -1)));
    obj.push_back(Pair("hashespersec",  gethashespersec(params, false)));
    obj.push_back(Pair("networkhashps", getnetworkhashps(params, false)));
    obj.push_back(Pair("pooledtx",      (uint64_t)mempool.size()));
    obj.push_back(Pair("testnet",       false));
    return obj;
}

static std::string prefixToWidth(const std::string& Str, int Size, char C)
{
    int PrefixLen = Size - Str.length();
    if (PrefixLen <= 0)
        return Str;

    std::string Prefix(PrefixLen, C);
    return Prefix + Str;
}

template<typename T>
std::string HexStrAceLight(const T itbegin, const T itend, bool fSpaces=false)
{
    std::string rv;
    static const char hexmap[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    rv.reserve((itend-itbegin)*3);
    for(T it = itbegin; it < itend; ++it)
    {
        unsigned int val_int = (unsigned int)(*it);
        //qDebug() << "HHH" << QString::number(val_int,16);
        //unsigned char val = (unsigned char)(*it);
        //if(fSpaces && it != itbegin)
        //    rv.push_back(' ');
        unsigned char METHOD = ((unsigned char*)&val_int)[0];
        unsigned char BYTE_A = ((unsigned char*)&val_int)[1];
        unsigned char BYTE_B = ((unsigned char*)&val_int)[2];
        rv.push_back(hexmap[METHOD>>4]);
        rv.push_back(hexmap[METHOD&15]);
        rv.push_back(hexmap[BYTE_A>>4]);
        rv.push_back(hexmap[BYTE_A&15]);
        rv.push_back(hexmap[BYTE_B>>4]);
        rv.push_back(hexmap[BYTE_B&15]);
    }

    return rv;
}

template<typename T>
inline std::string HexStrAceLight(const T& vch, bool fSpaces=false)
{
    return HexStrAceLight(vch.begin(), vch.end(), fSpaces);
}

Value getwork(const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 2)
        throw runtime_error(
            "getwork [data, coinbase]\n"
            "Returns work data.\n"
            "\n"
            "getwork [hash]\n"
            "Returns new work data or true if no newer data available.\n"
            "\n"
            "getwork [data, coinbase]\n"
            "Submit work.\n"
        );

    if (vNodes.empty())
        throw JSONRPCError(RPC_CLIENT_NOT_CONNECTED, "ZettelKasten is not connected!");

    if (IsInitialBlockDownload())
        throw JSONRPCError(RPC_CLIENT_IN_INITIAL_DOWNLOAD, "ZettelKasten is downloading blocks...");

    static CBlockTemplate* pblocktemplate = NULL;
    static uint256 curBlockHash = 0;

    vector<unsigned char> vchData;
    if (params.size() >= 1)
        vchData = ParseHex(params[0].get_str());

    if (vchData.size() == 0 || vchData.size() == 32)
    {
        std::string PrivAddress = GetArg("-miningprivkey", "");

        CKey MiningKey;
        if (!PrivAddress.empty())
        {
            CBitcoinSecret Secret;
            Secret.SetString(PrivAddress);
            if (Secret.IsValid())
                MiningKey = Secret.GetKey();
        }

        CPubKey pubkey;
        CKey PrivKey;

        if (MiningKey.IsValid())
        {
            PrivKey = MiningKey;
            pubkey = MiningKey.GetPubKey();
        }
        else
        {
            if (!pMiningKey->GetReservedKey(pubkey))
                throw JSONRPCError(RPC_WALLET_ERROR, "Error: can't get new address");
            if (!pwalletMain->GetKey(pubkey.GetID(), PrivKey))
                throw JSONRPCError(RPC_WALLET_UNLOCK_NEEDED, "Error: need to unlock the wallet");
        }

        // Update block
        static unsigned int nTransactionsUpdatedLast = 0;
        static CBlockIndex* pindexPrev = NULL;
        static int64 nStart = 0;
        if (pindexPrev != pindexBest || (nTransactionsUpdated != nTransactionsUpdatedLast && GetTime() - nStart > 20))
        {
            nTransactionsUpdatedLast = nTransactionsUpdated;
            pindexPrev = pindexBest;
            nStart = GetTime();

            // Create new block
            delete pblocktemplate;
            pblocktemplate = CreateNewBlockWithKey(pubkey.GetID());

            curBlockHash = pblocktemplate->block.GetHash();
        }
        if (vchData.size() == 32)
        {
            if (curBlockHash == *(uint256*)&vchData[0])
                return true;
        }

        CBlock* pblock = &pblocktemplate->block; // pointer for convenience

        CSignerECDSA Signer(PrivKey.begin(), pblock->MinerSignature.begin());

        // Update nTime
        pblock->UpdateTime(pindexPrev);
        pblock->nNonce = 0;

        uint256 hashTarget = CBigNum().SetCompact(pblock->nBits).getuint256();

        CDataStream ssBlock(SER_NETWORK, PROTOCOL_VERSION);
        ssBlock << *pblock;

        std::string kinv = prefixToWidth(Signer.GetKInv(), 64, '0');
        std::string pmr  = prefixToWidth(Signer.GetPMR() , 64, '0');
        std::string prk  = prefixToWidth(Signer.GetPRK() , 64, '0');

        CBufferStream<185> header = pblock->SerializeHeaderForHash2();
        CBufferStream<MAX_BLOCK_SIZE> txs(SER_GETHASH, 0);
        txs << pblock->vtx;

        Object result;
        result.push_back(Pair("data",     HexStr(header.begin(), header.end())));
        result.push_back(Pair("target",   HexStr(BEGIN(hashTarget), END(hashTarget))));
        result.push_back(Pair("hash",     HexStr(BEGIN(curBlockHash), END(curBlockHash))));
        result.push_back(Pair("kinv",     kinv));
        result.push_back(Pair("pmr",      pmr));
        result.push_back(Pair("prk",      prk));
        result.push_back(Pair("tx",       HexStr(txs.begin(), txs.end())));
        result.push_back(Pair("true_privkey", HexStr(PrivKey.begin(), PrivKey.end())));
        result.push_back(Pair("privkey",  pmr));

        if(pblock->nHeight >=SDKPGAB_START_HEIGHT){
            ABCBytesForSDKPGAB bytes;

            bytes = GetABCBytesForSDKPGABFromHash(pindexPrev->GetBlockHash());

            result.push_back(Pair("A",HexStr(BEGIN(bytes.A), END(bytes.A))));
            result.push_back(Pair("B",HexStr(BEGIN(bytes.B), END(bytes.B))));
        }

        if(pblock->nHeight >=SDKPGABSPCSSWSSBP_START_HEIGHT){
            uint256 hashPrevBlock = pindexBest->GetBlockHash();
            uint256 pubkey_hashPrevBlock;

            pubkey_hashPrevBlock = SDKPGABSPCSSWSSBP_GetPublicKeyFromPrivateKey(hashPrevBlock);

            result.push_back(Pair("prevhashblock",   HexStr(BEGIN(pubkey_hashPrevBlock), END(pubkey_hashPrevBlock))));
        }

        std::vector<uint256> merkle = pblock->GetMerkleBranch(0);
        Array merkle_arr;

        BOOST_FOREACH(uint256 merkleh, merkle) {
            printf("%s\n", merkleh.ToString().c_str());
            merkle_arr.push_back(HexStr(BEGIN(merkleh), END(merkleh)));
        }

        result.push_back(Pair("merkle", merkle_arr));

        if(pblock->nHeight >=SDKPGABSPCSSWSSBP_ACELIGHT_START_HEIGHT){
            std::vector<unsigned int> SDKPGABSPCSSWSSBP_ACELIGHT_INSTRUCTION_CHAIN;

            const uint32_t dist = (pblock->nHeight-SDKPGABSPCSSWSSBP_ACELIGHT_START_HEIGHT);
            const uint32_t instr_i = (dist/SDKPGABSPCSSWSSBP_ACELIGHT_SPACING);

            if(SDKPGABSPCSSWSSBP_ACELIGHT_INSTRUCTION_CHAIN.size() < (instr_i+1)){
                SDKPGABSPCSSWSSBP_ACELIGHT_INSTRUCTION_CHAIN.resize(instr_i+1);

                const uint32_t ace_first_height = SDKPGABSPCSSWSSBP_ACELIGHT_START_HEIGHT-SDKPGABSPCSSWSSBP_ACELIGHT_DISTANCE;

                for(int i = 0; i<=instr_i;i++){
                    SDKPGABSPCSSWSSBP_ACELIGHT_INSTRUCTION_CHAIN.at(i) =
                            FindBlockByHeight(ace_first_height
                                              +(i*SDKPGABSPCSSWSSBP_ACELIGHT_SPACING))->GetBlockHash().Get64(0);
                };
            };

            result.push_back(Pair("ace_light_instructions",     HexStrAceLight(SDKPGABSPCSSWSSBP_ACELIGHT_INSTRUCTION_CHAIN.begin(), SDKPGABSPCSSWSSBP_ACELIGHT_INSTRUCTION_CHAIN.end())));

            unsigned int ace_light_strength = SDKPGABSPCSSWSSBP_ACELIGHT_INSTRUCTION_CHAIN.size();
            result.push_back(Pair("ace_light_strength",HexStr(BEGIN(ace_light_strength), END(ace_light_strength))));
        }

        return result;
    }
    else if (vchData.size() == 185)
    {
        // Parse parameters
        CBlockHeader header;
        CDataStream(vchData, SER_NETWORK, PROTOCOL_VERSION) >> header;

        CBlock* pblock = &pblocktemplate->block;
        if (header.hashPrevBlock != pblock->hashPrevBlock)
            return false;
        *(CBlockHeader*)pblock = header;

        if (params.size() == 2)
        {
            vector<unsigned char> coinbase = ParseHex(params[1].get_str());
            CDataStream(coinbase, SER_NETWORK, PROTOCOL_VERSION) >> pblock->vtx[0];
        }

        return CheckWork(pblock, *pwalletMain, pMiningKey);
    }
    throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid parameter");
}


Value getblocktemplate(const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
            "getblocktemplate [params]\n"
            "Returns data needed to construct a block to work on:\n"
            "  \"version\" : block version\n"
            "  \"previousblockhash\" : hash of current highest block\n"
            "  \"transactions\" : contents of non-coinbase transactions that should be included in the next block\n"
            "  \"coinbaseaux\" : data that should be included in coinbase\n"
            "  \"coinbasevalue\" : maximum allowable input to coinbase transaction, including the generation award and transaction fees\n"
            "  \"target\" : hash target\n"
            "  \"mintime\" : minimum timestamp appropriate for next block\n"
            "  \"curtime\" : current timestamp\n"
            "  \"mutable\" : list of ways the block template may be changed\n"
            "  \"noncerange\" : range of valid nonces\n"
            "  \"sigoplimit\" : limit of sigops in blocks\n"
            "  \"sizelimit\" : limit of block size\n"
            "  \"bits\" : compressed target of next block\n"
            "  \"height\" : height of the next block\n"
            "  \"payee1\" : required payee1\n"
            "  \"votes\" : show vote candidates for this block\n"
            "See https://en.bitcoin.it/wiki/BIP_0022 for full specification.");

    std::string strMode = "template";
    if (params.size() > 0)
    {
        const Object& oparam = params[0].get_obj();
        const Value& modeval = find_value(oparam, "mode");
        if (modeval.type() == str_type)
            strMode = modeval.get_str();
        else if (modeval.type() == null_type)
        {
            /* Do nothing */
        }
        else
            throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid mode");
    }

    if (strMode != "template")
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid mode");

    if (vNodes.empty())
        throw JSONRPCError(RPC_CLIENT_NOT_CONNECTED, "ZettelKasten is not connected!");

    if (IsInitialBlockDownload())
        throw JSONRPCError(RPC_CLIENT_IN_INITIAL_DOWNLOAD, "ZettelKasten is downloading blocks...");

    // Update block
    static unsigned int nTransactionsUpdatedLast;
    static CBlockIndex* pindexPrev;
    static int64 nStart;
    static CBlockTemplate* pblocktemplate;
    if (pindexPrev != pindexBest ||
        (nTransactionsUpdated != nTransactionsUpdatedLast && GetTime() - nStart > 5))
    {
        // Clear pindexPrev so future calls make a new block, despite any failures from here on
        pindexPrev = NULL;

        // Store the pindexBest used before CreateNewBlock, to avoid races
        nTransactionsUpdatedLast = nTransactionsUpdated;
        CBlockIndex* pindexPrevNew = pindexBest;
        nStart = GetTime();

        // Create new block
        if(pblocktemplate)
        {
            delete pblocktemplate;
            pblocktemplate = NULL;
        }
        CScript scriptDummy = CScript() << OP_TRUE;
        pblocktemplate = CreateNewBlock(scriptDummy);
        if (!pblocktemplate)
            throw JSONRPCError(RPC_OUT_OF_MEMORY, "Out of memory");

        // Need to update only after we know CreateNewBlock succeeded
        pindexPrev = pindexPrevNew;
    }
    CBlock* pblock = &pblocktemplate->block; // pointer for convenience

    // Update nTime
    pblock->UpdateTime(pindexPrev);
    pblock->nNonce = 0;

    Array transactions;
    map<uint256, int64_t> setTxIndex;
    int i = 0;
    BOOST_FOREACH (CTransaction& tx, pblock->vtx)
    {
        uint256 txHash = tx.GetHash();
        setTxIndex[txHash] = i++;

        if (tx.IsCoinBase())
            continue;

        Object entry;

        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << tx;
        entry.push_back(Pair("data", HexStr(ssTx.begin(), ssTx.end())));

        entry.push_back(Pair("hash", txHash.GetHex()));

        Array deps;
        BOOST_FOREACH (const CTxIn &in, tx.vin)
        {
            if (setTxIndex.count(in.prevout.hash))
                deps.push_back(setTxIndex[in.prevout.hash]);
        }
        entry.push_back(Pair("depends", deps));

        int index_in_template = i - 1;
        entry.push_back(Pair("fee", pblocktemplate->vTxFees[index_in_template]));
        entry.push_back(Pair("sigops", pblocktemplate->vTxSigOps[index_in_template]));

        transactions.push_back(entry);
    }

    Object aux;
    aux.push_back(Pair("flags", HexStr(COINBASE_FLAGS.begin(), COINBASE_FLAGS.end())));

    uint256 hashTarget = CBigNum().SetCompact(pblock->nBits).getuint256();

    Array aMutable;
    if (aMutable.empty())
    {
        aMutable.push_back("time");
        aMutable.push_back("transactions");
        aMutable.push_back("prevblock");
    }

    Object result;
    result.push_back(Pair("version", pblock->nVersion));
    result.push_back(Pair("previousblockhash", pblock->hashPrevBlock.GetHex()));
    result.push_back(Pair("transactions", transactions));
    result.push_back(Pair("coinbaseaux", aux));
    result.push_back(Pair("coinbasevalue", (int64_t)pblock->vtx[0].GetValueOut()));
    result.push_back(Pair("target", hashTarget.GetHex()));
    result.push_back(Pair("mintime", (int64_t)pindexPrev->GetMedianTimePast()+1));
    result.push_back(Pair("mutable", aMutable));
    result.push_back(Pair("noncerange", "00000000ffffffff"));
    result.push_back(Pair("sigoplimit", (int64_t)MAX_BLOCK_SIGOPS));
    result.push_back(Pair("sizelimit", (int64_t)MAX_BLOCK_SIZE));
    result.push_back(Pair("curtime", (int64_t)pblock->nTime));
    result.push_back(Pair("bits", HexBits(pblock->nBits)));
    result.push_back(Pair("height", (int64_t)(pindexPrev->nHeight+1)));

    if(pblock->payee != CScript()){
        CTxDestination address1;
        ExtractDestination(pblock->payee, address1);
        CBitcoinAddress address2(address1);
        result.push_back(Pair("payee", address2.ToString().c_str()));
    } else {
        result.push_back(Pair("payee", ""));
    }

    return result;
}

Value submitblock(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
        throw runtime_error(
            "submitblock <hex data> [optional-params-obj]\n"
            "[optional-params-obj] parameter is currently ignored.\n"
            "Attempts to submit new block to network.\n"
            "See https://en.bitcoin.it/wiki/BIP_0022 for full specification.");

    vector<unsigned char> blockData(ParseHex(params[0].get_str()));
    CDataStream ssBlock(blockData, SER_NETWORK, PROTOCOL_VERSION);
    CBlock pblock;
    try {
        ssBlock >> pblock;
    }
    catch (std::exception &e) {
        throw JSONRPCError(RPC_DESERIALIZATION_ERROR, "Block decode failed");
    }

    CValidationState state;
    bool fAccepted = ProcessBlock(state, NULL, &pblock);
    if (!fAccepted)
        return "rejected"; // TODO: report validation state

    return Value::null;
}
