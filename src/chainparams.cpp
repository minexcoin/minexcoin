// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"
#include <assert.h>
#include <mutex>
#include <cmath>
#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint256 nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint256 nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 20/07/2017 Minexcoin";
    const CScript genesisOutputScript = CScript() << ParseHex("035640ff41f4ed5a927db8ed19bc1d691596356baaf5100f0a3e3479c5d8e6b5a6") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000; //Don't used in Minexcoin
        consensus.MIP1Height = 24000;
        consensus.MIP2Height = -1;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2 * 24 * 60 * 60; // two days
        consensus.nPowTargetSpacing = 3 * 60; // three minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nMinerConfirmationWindow = consensus.nPowTargetTimespan / consensus.nPowTargetSpacing;
        consensus.nRuleChangeActivationThreshold = std::ceil(consensus.nMinerConfirmationWindow * 0.95); // 95% of nMinerConfirmationWindow
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000001f4652da8c6b4");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000001d26827155dedd004020740a0a73aee7a6694c0d4ff6d6ef8c9bb819"); //109800

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x4b;
        pchMessageStart[1] = 0x4a;
        pchMessageStart[2] = 0x4c;
        pchMessageStart[3] = 0x5d;
        nDefaultPort = 8335;
        nPruneAfterHeight = 100000;
        const size_t N = 96, K = 5;
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(1500508801, uint256S("0x0000000000000000000000000000000000000000003f94d1ad391682fe038bfd"), 
            0x207fffff, 1, 50 * COIN);
        genesis.nSolution = ParseHex("01dae2d39764d434a275f65c09fc9300450660c8a85d48f00e31a61235ccfc795fd1198c4893743b3c0c11dacc0b80e71aac60307e499aaa74b997d60e4edc12a3d3c240");

        /*printf("Searching for genesis block...\n");
        // This will figure out a valid hash and Nonce if you're
        // creating a different genesis block:
        arith_uint256 hashTarget = hashTarget.SetCompact(genesis.nBits);
        printf("hashTarget = %s\n", hashTarget.ToString().c_str());
        arith_uint256 thash;

        while(true)
        {
            crypto_generichash_blake2b_state state;
            std::mutex m_cs;
            bool cancelSolver = false;
            std::string solver = GetArg("-equihashsolver", "default");
            EhInitialiseState(nEquihashN, nEquihashK, state);

            // I = the block header minus nonce and solution.
            CEquihashInput I{genesis};
            CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
            ss << I;

            // H(I||...
            crypto_generichash_blake2b_update(&state, (unsigned char*)&ss[0], ss.size());

            // H(I||V||...
            crypto_generichash_blake2b_state curr_state;
            curr_state = state;
            crypto_generichash_blake2b_update(&curr_state,
                                        genesis.nNonce.begin(),
                                        genesis.nNonce.size());
            std::function<bool(std::vector<unsigned char>)> validBlock =
                [&hashTarget, &m_cs, &cancelSolver, this]
                    (std::vector<unsigned char> soln) {
                        // Write the solution to the hash and compute the result.
                        // printf("- Checking solution against target\n");
                        genesis.nSolution = soln;

                        if (UintToArith256(genesis.GetHash()) > hashTarget) {
                            return false;
                        }

                        if (!CheckEquihashSolution(&genesis, *this)) {
                            return false;
                        }

                        // Found a solution
                        // Ignore chain updates caused by us
                        std::lock_guard<std::mutex> lock{m_cs};
                        cancelSolver = false;
                        return true;
            };
            std::function<bool(EhSolverCancelCheck)> cancelled = [&m_cs, &cancelSolver](EhSolverCancelCheck pos) {
                std::lock_guard<std::mutex> lock{m_cs};
                return cancelSolver;
            };
            if (solver == "tromp") {
                // Create solver and initialize it.
                equi eq(1);
                eq.setstate(&curr_state);

                // Intialization done, start algo driver.
                eq.digit0(0);
                eq.xfull = eq.bfull = eq.hfull = 0;
                eq.showbsizes(0);
                for (u32 r = 1; r < WK; r++) {
                    (r&1) ? eq.digitodd(r, 0) : eq.digiteven(r, 0);
                    eq.xfull = eq.bfull = eq.hfull = 0;
                    eq.showbsizes(r);
                }
                eq.digitK(0);

                // Convert solution indices to byte array (decompress) and pass it to validBlock method.
                bool ready = false;
                for (size_t s = 0; s < eq.nsols; s++) {
                    // printf("\rChecking solution %d", int(s+1));
                    std::vector<eh_index> index_vector(PROOFSIZE);
                    for (size_t i = 0; i < PROOFSIZE; i++) {
                        index_vector[i] = eq.sols[s][i];
                    }
                    std::vector<unsigned char> sol_char = GetMinimalFromIndices(index_vector, DIGITBITS);

                    if (validBlock(sol_char)) {
                        // If we find a POW solution, do not try other solutions
                        // because they become invalid as we created a new block in blockchain.
                        ready = true;
                        break;
                    }
                }
                if (ready) break;
            } else {
                try {
                    // If we find a valid block, we rebuild
                    bool found = EhOptimisedSolve(nEquihashN, nEquihashK, curr_state, validBlock, cancelled);
                    if (found) {
                        break;
                    }
                } catch (EhSolverCancelledException&) {
                    printf("Equihash solver cancelled\n");
                    std::lock_guard<std::mutex> lock{m_cs};
                    cancelSolver = false;
                }
            }

            genesis.nNonce = ArithToUint256(UintToArith256(genesis.nNonce) + 1);
        }

        printf("block.nTime = %u \n", genesis.nTime);
        printf("block.nNonce = %s \n", genesis.nNonce.ToString().c_str());
        printf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        printf("block.hashMerkleRoot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("block.nSolution = %s\n", HexStr(genesis.nSolution.begin(), genesis.nSolution.end()).c_str());//*/

        consensus.hashGenesisBlock = genesis.GetHash();
        /*CBlockHeader genesisHead= genesis.GetBlockHeader();
	
        printf("%s",genesisHead.hashMerkleRoot.ToString().c_str());*/
        assert(consensus.hashGenesisBlock == uint256S("490a36d9451a55ed197e34aca7414b35d775baa4a8e896f1c577f65ce2d214cb"));
        assert(genesis.hashMerkleRoot == uint256S("0x0516e9e037b01d085c49c4957801c909432cdbfc1facc0b0ff25de0d7bd2b8a8"));
        
        vSeeds.push_back(CDNSSeedData("138.197.73.48", "138.197.73.48"));
        vSeeds.push_back(CDNSSeedData("138.197.73.123", "138.197.73.123"));
        vSeeds.push_back(CDNSSeedData("159.203.70.193", "159.203.70.193"));
        vSeeds.push_back(CDNSSeedData("88.198.33.35", "88.198.33.35"));
        vSeeds.push_back(CDNSSeedData("95.85.35.152", "95.85.35.152"));
        vSeeds.push_back(CDNSSeedData("78.46.93.126", "78.46.93.126"));
        vSeeds.push_back(CDNSSeedData("91.233.111.28", "91.233.111.28"));
         // Note that of those with the service bits flag, most only support a subset of possible options
        // vSeeds.push_back(CDNSSeedData("bitcoin.sipa.be", "seed.bitcoin.sipa.be", true)); // Pieter Wuille, only supports x1, x5, x9, and xd
        // vSeeds.push_back(CDNSSeedData("bluematt.me", "dnsseed.bluematt.me", true)); // Matt Corallo, only supports x9
        // vSeeds.push_back(CDNSSeedData("dashjr.org", "dnsseed.bitcoin.dashjr.org")); // Luke Dashjr
        // vSeeds.push_back(CDNSSeedData("bitcoinstats.com", "seed.bitcoinstats.com", true)); // Christian Decker, supports x1 - xf
        // vSeeds.push_back(CDNSSeedData("bitcoin.jonasschnelli.ch", "seed.bitcoin.jonasschnelli.ch", true)); // Jonas Schnelli, only supports x1, x5, x9, and xd

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,75);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (110000, uint256S("0x000000b76be158ed71ca2b3c035b0d041285e3f7961d8fd05c15d266ded3284a"))
        };

        chainTxData = ChainTxData{
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000; //Don't used in Minexcoin
        consensus.MIP1Height = 2;
        consensus.MIP2Height = 0;
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        consensus.BIP65Height = 0; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 0; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2 * 24 * 60 * 60; // two days
        consensus.nPowTargetSpacing = 3 * 60; // three minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nMinerConfirmationWindow = consensus.nPowTargetTimespan / consensus.nPowTargetSpacing;
        consensus.nRuleChangeActivationThreshold = std::ceil(consensus.nMinerConfirmationWindow * 0.75); // 75% of nMinerConfirmationWindow
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000001");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000000000128796ee387cf110ccb9d2f36cffaf7f73079c995377c65ac0dcc"); //1079274

        pchMessageStart[0] = 0x54;
        pchMessageStart[1] = 0x4a;
        pchMessageStart[2] = 0x4c;
        pchMessageStart[3] = 0x54;
        nDefaultPort = 8336;
        nPruneAfterHeight = 1000;
        const size_t N = 96, K = 5;
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(1520955809, uint256S("0x0000000000000000000000000000000000000000003f94d1ad391682fe038bfd"),
            0x207fffff, 1, 50 * COIN);

        genesis.nSolution = ParseHex("00f9494486bb0e1d03951320ab82f9eead1559a4e408a02f97467a0ce3aeaec3ff701c8db53120df398e53727e36a0e189c3ec409b28c79bf75bc2c4181a4b395b8d3d2e");
        
        /* printf("Searching for genesis block...\n");
        // This will figure out a valid hash and Nonce if you're
        // creating a different genesis block:
        arith_uint256 hashTarget = hashTarget.SetCompact(genesis.nBits);
        printf("hashTarget = %s\n", hashTarget.ToString().c_str());
        arith_uint256 thash;

        while(true)
        {
            crypto_generichash_blake2b_state state;
            std::mutex m_cs;
            bool cancelSolver = false;
            std::string solver = GetArg("-equihashsolver", "default");
            EhInitialiseState(nEquihashN, nEquihashK, state);

            // I = the block header minus nonce and solution.
            CEquihashInput I{genesis};
            CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
            ss << I;

            // H(I||...
            crypto_generichash_blake2b_update(&state, (unsigned char*)&ss[0], ss.size());

            // H(I||V||...
            crypto_generichash_blake2b_state curr_state;
            curr_state = state;
            crypto_generichash_blake2b_update(&curr_state,
                                        genesis.nNonce.begin(),
                                        genesis.nNonce.size());
            std::function<bool(std::vector<unsigned char>)> validBlock =
                [&hashTarget, &m_cs, &cancelSolver, this]
                    (std::vector<unsigned char> soln) {
                        // Write the solution to the hash and compute the result.
                        // printf("- Checking solution against target\n");
                        genesis.nSolution = soln;

                        if (UintToArith256(genesis.GetHash()) > hashTarget) {
                            return false;
                        }

                        if (!CheckEquihashSolution(&genesis, *this)) {
                            return false;
                        }

                        // Found a solution
                        // Ignore chain updates caused by us
                        std::lock_guard<std::mutex> lock{m_cs};
                        cancelSolver = false;
                        return true;
            };
            std::function<bool(EhSolverCancelCheck)> cancelled = [&m_cs, &cancelSolver](EhSolverCancelCheck pos) {
                std::lock_guard<std::mutex> lock{m_cs};
                return cancelSolver;
            };
            if (solver == "tromp") {
                // Create solver and initialize it.
                equi eq(1);
                eq.setstate(&curr_state);

                // Intialization done, start algo driver.
                eq.digit0(0);
                eq.xfull = eq.bfull = eq.hfull = 0;
                eq.showbsizes(0);
                for (u32 r = 1; r < WK; r++) {
                    (r&1) ? eq.digitodd(r, 0) : eq.digiteven(r, 0);
                    eq.xfull = eq.bfull = eq.hfull = 0;
                    eq.showbsizes(r);
                }
                eq.digitK(0);

                // Convert solution indices to byte array (decompress) and pass it to validBlock method.
                bool ready = false;
                for (size_t s = 0; s < eq.nsols; s++) {
                    // printf("\rChecking solution %d", int(s+1));
                    std::vector<eh_index> index_vector(PROOFSIZE);
                    for (size_t i = 0; i < PROOFSIZE; i++) {
                        index_vector[i] = eq.sols[s][i];
                    }
                    std::vector<unsigned char> sol_char = GetMinimalFromIndices(index_vector, DIGITBITS);

                    if (validBlock(sol_char)) {
                        // If we find a POW solution, do not try other solutions
                        // because they become invalid as we created a new block in blockchain.
                        ready = true;
                        break;
                    }
                }
                if (ready) break;
            } else {
                try {
                    // If we find a valid block, we rebuild
                    bool found = EhOptimisedSolve(nEquihashN, nEquihashK, curr_state, validBlock, cancelled);
                    if (found) {
                        break;
                    }
                } catch (EhSolverCancelledException&) {
                    printf("Equihash solver cancelled\n");
                    std::lock_guard<std::mutex> lock{m_cs};
                    cancelSolver = false;
                }
            }

            genesis.nNonce = ArithToUint256(UintToArith256(genesis.nNonce) + 1);
        }

        printf("block.nTime = %u \n", genesis.nTime);
        printf("block.nNonce = %s \n", genesis.nNonce.ToString().c_str());
        printf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        printf("block.hashMerkleRoot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("block.nSolution = %s\n", HexStr(genesis.nSolution.begin(), genesis.nSolution.end()).c_str());// */

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("6ef2e897f7acb347086f9860b2ad401f133fe1b103f77de771aac7b5e88cfe70"));
        assert(genesis.hashMerkleRoot == uint256S("0x0516e9e037b01d085c49c4957801c909432cdbfc1facc0b0ff25de0d7bd2b8a8"));
        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("138.197.90.246", "138.197.90.246"));
        vSeeds.push_back(CDNSSeedData("139.59.154.41", "139.59.154.41"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
                boost::assign::map_list_of
                ( 0, uint256S("0x"))
        };

        chainTxData = ChainTxData{
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 210000; //Don't used in Minexcoin
        consensus.MIP1Height = 2;
        consensus.MIP2Height = -1;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

	pchMessageStart[0] = 0x4b;
        pchMessageStart[1] = 0x4a;
        pchMessageStart[2] = 0x4c;
        pchMessageStart[3] = 0x5d;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;
        const size_t N = 96, K = 5;
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(1500508801, uint256S("0x0000000000000000000000000000000000000000003f94d1ad391682fe038bfd"), 0x207fffff, 1, 50 * COIN);
        genesis.nSolution = ParseHex("01dae2d39764d434a275f65c09fc9300450660c8a85d48f00e31a61235ccfc795fd1198c4893743b3c0c11dacc0b80e71aac60307e499aaa74b997d60e4edc12a3d3c240");
        consensus.hashGenesisBlock = genesis.GetHash();

//    

        assert(consensus.hashGenesisBlock == uint256S("490a36d9451a55ed197e34aca7414b35d775baa4a8e896f1c577f65ce2d214cb"));
        assert(genesis.hashMerkleRoot == uint256S("0x0516e9e037b01d085c49c4957801c909432cdbfc1facc0b0ff25de0d7bd2b8a8"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x001"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
