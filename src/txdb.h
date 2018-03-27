// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_TXDB_LEVELDB_H
#define BITCOIN_TXDB_LEVELDB_H

#include "main.h"
#include "leveldb.h"

/** CCoinsView backed by the LevelDB coin database (chainstate/) */
class CCoinsViewDB : public CCoinsView
{
protected:
    CLevelDB db;
public:
    CCoinsViewDB(size_t nCacheSize, bool fMemory = false, bool fWipe = false);

    bool GetCoins(const uint256 &txid, CCoins &coins);
    bool SetCoins(const uint256 &txid, const CCoins &coins);
    bool HaveCoins(const uint256 &txid);
    CBlockIndex *GetBestBlock();
    bool SetBestBlock(CBlockIndex *pindex);
    bool BatchWrite(const std::map<uint256, CCoins> &mapCoins, CBlockIndex *pindex);
    bool GetStats(CCoinsStats &stats);
};

/** Access to the block database (blocks/index/) */
class CBlockTreeDB : public CLevelDB
{
public:
    CBlockTreeDB(size_t nCacheSize, bool fMemory = false, bool fWipe = false);
private:
    CBlockTreeDB(const CBlockTreeDB&);
    void operator=(const CBlockTreeDB&);
public:
    bool WriteBlockIndex(const CDiskBlockIndex& blockindex);
    bool ReadBestInvalidWork(CBigNum& bnBestInvalidWork);
    bool WriteBestInvalidWork(const CBigNum& bnBestInvalidWork);
    bool ReadBlockFileInfo(int nFile, CBlockFileInfo &fileinfo);
    bool WriteBlockFileInfo(int nFile, const CBlockFileInfo &fileinfo);
    bool ReadLastBlockFile(int &nFile);
    bool WriteLastBlockFile(int nFile);
    bool WriteReindexing(bool fReindex);
    bool ReadReindexing(bool &fReindex);
    bool ReadTxIndex(const uint256 &txid, CDiskTxPos &pos);
    bool WriteTxIndex(const std::vector<std::pair<uint256, CDiskTxPos> > &list);
    bool WriteFlag(const std::string &name, bool fValue);
    bool ReadFlag(const std::string &name, bool &fValue);
    bool LoadBlockIndexGuts();
	bool ReadSyncCheckpoint(uint256& hashCheckpoint);
    bool WriteSyncCheckpoint(uint256 hashCheckpoint);
    bool ReadCheckpointPubKey(std::string& strPubKey);
    bool WriteCheckpointPubKey(const std::string& strPubKey);
};

/** Transactions by address */
class CAddressDB : public CLevelDB
{
    CAddressDB(const CAddressDB&);
    void operator=(const CAddressDB&);

public:
    CAddressDB(size_t nCacheSize, bool fMemory, bool fWipe);

    bool AddTx(const std::vector<CTransaction>& vtx, const std::vector<std::pair<uint256, CDiskTxPos> >& vpos);
    bool GetTxs(std::vector<CDiskTxPos>& Txs, const CScriptID& Address);
    bool ReadNextIn(const COutPoint& Out, uint256& Hash, unsigned int &n);

    bool WriteReindexing(bool fReindex);
    bool ReadReindexing(bool &fReindex);

    bool WriteEnable(bool fValue);
    bool ReadEnable(bool &fValue);
};

CTxOut getPrevOut(const CTxIn& In);
void getNextIn(const COutPoint& Out, uint256& Hash, unsigned int& n);
// Return transaction in tx, and if it was found inside a block, its header is placed in block
bool ReadTransaction(const CDiskTxPos &postx, CTransaction &txOut, CBlockHeader &block);

#endif // BITCOIN_TXDB_LEVELDB_H
