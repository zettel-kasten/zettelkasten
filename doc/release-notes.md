0.9.20.2 changes PART 2 (New Wallet UI Design)
=============

-) No protocol changes, so this upgrade is not mandatory,

Simply a new wallet UI design with a few fancy features, like a mempool visualizer.

This will help with the testing of the upcoming tx-appendix feature.


0.9.20.2 changes
=============

-) Introduction of a "Full Node Reward System" that lets users defined and broadcast a donation-address, that is visible and shared on the entire network and tied to its specific IP.

Use zettelkasten.conf file to add following line:

donation=X

(where X is your ZTTL-Address)
e.g.:
donation=ZbT9debLiUeur2JVVBrowHM4DcZueTTaUY

You can then see your donation-address when typing getinfo, and see everyone's donation-address (if defined) with getpeerinfo.


0.9.20.1 changes
=============

-) new Burgerhash V6 POW algorithm:
SDKPGABSPCSSWSSBP-ACE-LIGHT (SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSaladSenoritaBonitaPepita-ACE-LIGHT)
-) new protocol version: 70024

Introducing the new "Algo Composer Engine".
Starting with block #555555, every 360 blocks (ca 6 hours) a new instruction is added to the ACE-LIGHT-part of the POW-Code.
(POW is now soft-forking 4 times a day sotospeak.)


0.9.19.1 changes
=============

-) introducing new Burgerhash V5 algorithm starting with block >= 400k
SDKPGABSPCSSWSSBP (SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSaladSenoritaBonitaPepita)

-) new protocol version 70023


0.9.18.2 changes
=============

This version fixes the sudden forking issue we had for a while.
All credit to elbandi


0.9.18.1 changes
=============

-) introducing new Burgerhash V4 algorithm starting with block >= 300k
SDKPGABSPCSSWS (SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSalad)

-) new protocol version 70022

-) new rpccommand "getwordsalad"


0.9.17.1 changes
=============
-) introducing new Burgerhash V3 algorithm starting with block >= 200k
SDKPGABSPC (SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheese)

-) new protocol version 70021


0.9.16.2 changes
=============

- BurgerHashV2-Release v0.9.16.2

- (fixed resync problem)


0.8.6.2 changes
=============

- Windows only: Fixes issue where network connectivity can fail.

- Cleanup of SSE2 scrypt detection.

- Minor fixes:
  - s/Bitcoin/BitCoin/ in the Coin Control example
  - Fix custom build on MacOS X 10.9
  - Fix QT5 custom build
  - Update Debian build instructions
  - Update Homebrew build 

0.8.6.1 changes
=============

- Coin Control - experts only GUI selection of inputs before you send a transaction

- Disable Wallet - reduces memory requirements, helpful for miner or relay nodes

- 20x reduction in default mintxfee.

- Up to 50% faster PoW validation, faster sync and reindexing.

- Peers older than protocol version 70002 are disconnected.  0.8.3.7 is the oldest compatible client.

- Internal miner added back to BitCoin.  setgenerate now works, although it is generally a bad idea as it is significantly slower than external CPU miners.

- New RPC commands: getbestblockhash and verifychain

- Improve fairness of the high priority transaction space per block

- OSX block chain database corruption fixes
  - Update leveldb to 1.13
  - Use fcntl with `F_FULLSYNC` instead of fsync on OSX
  - Use native Darwin memory barriers
  - Replace use of mmap in leveldb for improved reliability (only on OSX)

- Fix nodes forwarding transactions with empty vins and getting banned

- Network code performance and robustness improvements

- Additional debug.log logging for diagnosis of network problems, log timestamps by default

- Fix rare GUI crash on send

0.8.5.1 changes
===============

Workaround negative version numbers serialization bug.

Fix out-of-bounds check (BitCoin currently does not use this codepath, but we apply this
patch just to match Bitcoin 0.8.5.)

0.8.4.1 changes
===============

CVE-2013-5700 Bloom: filter crash issue - BitCoin 0.8.3.7 disabled bloom by default so was
unaffected by this issue, but we include their patches anyway just in case folks want to 
enable bloomfilter=1.

CVE-2013-4165: RPC password timing guess vulnerability

CVE-2013-4627: Better fix for the fill-memory-with-orphaned-tx attack

Fix multi-block reorg transaction resurrection.

Fix non-standard disconnected transactions causing mempool orphans.  This bug could cause 
nodes running with the -debug flag to crash, although it was lot less likely on BitCoin
as we disabled IsDust() in 0.8.3.x.

Mac OSX: use 'FD_FULLSYNC' with LevelDB, which will (hopefully!) prevent the database 
corruption issues have experienced on OSX.

Add height parameter to getnetworkhashps.

Fix Norwegian and Swedish translations.

Minor efficiency improvement in block peer request handling.


0.8.3.7 changes
===============

Fix CVE-2013-4627 denial of service, a memory exhaustion attack that could crash low-memory nodes.

Fix a regression that caused excessive writing of the peers.dat file.

Add option for bloom filtering.

Fix Hebrew translation.
