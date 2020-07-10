// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2020 The Bitcoin Global developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>
#include <script/standard.h>

#include <assert.h>
#include <pubkey.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
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
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Exception = uint256S("0x00000000000002dc756eebf4f49723ed8d30cc28a5f108eb94b1ba88ac4f9c22");
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.BIP65Height = 388381; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 363725; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.CSVHeight = 419328; // 000000000000000004a1b34462cb8aeebd5799177f7a29cf28f2d1961716b5b5
        consensus.SegwitHeight = 481824; // 0000000000000000001c8018d9cb3b742ef25114f27563e3fc4a1902167f9893
        consensus.MinBIP9WarningHeight = 483840; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Bitcoin Global hard fork.
        consensus.BTGHeight = 638592;
        consensus.BTGPremineWindow = 100;
        consensus.BTGPremineEnforceWhitelist = true;
        consensus.BTGPremineReward = 1000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000e1ab5ec9348e9f4b8eb8154");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000000000000f2adce67e49b0b6bdeb9de8b7c3d7e93b21e7fc1e819d"); // 623950

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x3b;
        pchMessageStart[1] = 0xf8;
        pchMessageStart[2] = 0xac;
        pchMessageStart[3] = 0x8f;
        nDefaultPort = 8222;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 320;
        m_assumed_chain_state_size = 4;

        genesis = CreateGenesisBlock(1231006505, 2083236893, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"));
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("seed.bitcoin-global.dev");
        vSeeds.emplace_back("dnsseed.bitcoin-global.io");
        vSeeds.emplace_back("dnsseed.bitcoin-global.co");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);  // prefix: G
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,23);  // prefix: A
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "glob";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;

        checkpointData = {
            {
                { 11111, uint256S("0x0000000069e244f73d78e8fd29ba2fd2ed618bd6fa2ee92559f542fdb26e7c1d")},
                { 33333, uint256S("0x000000002dd5588a74784eaa7ab0507a18ad16a236e7b1ce69f00d7ddfb5d0a6")},
                { 74000, uint256S("0x0000000000573993a3c9e41ce34471c079dcf5f52a0e824a81e7f953b8661a20")},
                {105000, uint256S("0x00000000000291ce28027faea320c8d2b054b2e0fe44a773f3eefb151d6bdc97")},
                {134444, uint256S("0x00000000000005b12ffd4cd315cd34ffd4a594f430ac814c91184a0d42d2b0fe")},
                {168000, uint256S("0x000000000000099e61ea72015e79632f216fe6cb33d7899acb35b75c8303b763")},
                {193000, uint256S("0x000000000000059f452a5f7340de6682a977387c17010ff6e6c3bd83ca8b1317")},
                {210000, uint256S("0x000000000000048b95347e83192f69cf0366076336c639f9b7228e9ba171342e")},
                {216116, uint256S("0x00000000000001b4f4b433e81ee46494af945cf96014816a4e2370f11b23df4e")},
                {225430, uint256S("0x00000000000001c108384350f74090433e7fcf79a606b8e797f065b130575932")},
                {250000, uint256S("0x000000000000003887df1f29024b06fc2200b55f8af8f35453d7be294df2d214")},
                {279000, uint256S("0x0000000000000001ae8c72a0b0c301f67e3afca10e819efa9041e458e9bd7e40")},
                {295000, uint256S("0x00000000000000004d9b4ef50f0f9d686fd69db2e03af35a100370c64632a983")},
            }
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 4096 0000000000000000000f2adce67e49b0b6bdeb9de8b7c3d7e93b21e7fc1e819d
            /* nTime    */ 1585764811,
            /* nTxCount */ 517186863,
            /* dTxRate  */ 3.305709665792344,
        };

        // Premine PubKeys for coin distribution.
        vPreminePubkeys = {
            "028abd95108065315eb0d5a4ad75c1afe31039afd5e260f177a134bbe352347375", "0243d1f1298b08f83463f45a8a53a099d20bd0bf0dfc0e4f4168da44e573f4088e", "036d152f919bdcd877b912e773851def55f8266106e8bdf31537e093efcf5425c3", "0381cb19542c5c7b95f2b01f1504bbe4cafdf40cce32f7e5d7bb0b3525b35f3561", "02fcb0c546338f217d01e10700e99d4777659a2bb8d949161fbbdbc5a6802c87b2", "0251ff7846708dc5b4d6f9b1996882649f8fde8c75b22e99e0f06b57580a2eaa2a", "024910058d1db2c60a8d97719067cfc449d762729da30149456b2491c5f4cc9422", "033125aab0d71e8dca2a4f8ea184118ae9d39993c4889998f6aa4c23a7b556b31f", "02ecfebfae8cfaf52a865ef891b62237b98794a053c1a1c9613516c327ba5605ca", "033d929cff7d694778c9184c11a2bc82e6332143f03b27d326cbd59be0e82475dc", "03f49d91d89b43376844efb777512d0c09eb3626d073929c22596f190f585c8a31", "02bcf19f37fbbf2009d591c458fe7fb7a9846137b77b0876530c32ae319de64b88", "037b29498ffa9af5a9c15e1d50a5e9a131e5da8f9206b58d09fc90781b7858d511", "03ba58e4ab1be9dda480c0212bf5f2d4c328b21c788e2378316297ab5723b323b6", "031b1f1f28a37bffa7cd3c1771e4ee80b1ba79bf9cf711bca395baaf3ce9332437", "03bf22d33caf671e6957c2aadf1a81312ec0fdd3f5cc57b1ee1b1ca1c20ea6d657", "0311c10d2fc27951fbd2a1d90ebf637b6a0caf4de4cba249fa67a8f31bec717af1", "02c212b7871242340c692925b20572b35f24852f5c3a9c4481e1988cb81c074a55", "03c8ddc5c513bcf68fdc2c60d9b3193b05570772988665c5f119c0bce0be63cd38", "03383888c922af7dfb20af0663c77ac38ff887688b6dab8b3e3e8f6f082e464d49"
            "02ef242a6f2244658aa0e82cb929f8257e083843f048a4597844dedeef766e3bde", "028f277dc05af3f54151d7c6183c8cc02873fa970b0c46b01cfcfbe92c8213fdd2", "029e47d940ab6be06ef3f20199912395a8501f0d310c1a47cd4036790b949447e4", "03fd453aad7dcae8e38811850bb1c1cf478c553d88c5ff732e45074a2a06db4084", "02738941d5a5d0de0cf38de75b9153418c0dc4f67a1b8111233c844cc4e3117b6c", "02c35feba5eac52c2771e81d84bca029aac48de5136386a24f231839f61ede365e", "02405eaa7ae4c3e027bb99083fbd36571ba6a9cf262d683a56750ccb4796af02bd", "039eba24a33a40bf816a7baeffd4fda767c996c66914ccbd1edc054a9578aa6ec6", "036353accb471bf041c0b4fbb5ec2ba8620ee59a8f77a9e191625c62e663619589", "03d91b0cd8bd1650d2773576cbd8957bda5dd43b2190787d216c00f811922d94ef", "0381fe1bbab45a76c683fcd6913e56e62bab8b996b958739fc2ccdc967f4923a45", "03572652b55661318feaa2926f9bad5dc83d1f4b1d695e1d44ddb52af9f2a7949e", "0280cd063d33c25174fa5e8d56b5c5368403fca2a6c130b158899466c99e8d501c", "021e29a66e58d1e020eeef8318b1cf04a8784faca45b298113398706fda66e9a09", "02b2fa47c6e509c1eabc4b74cb0d8e366c6943c619c42b0125ed76ee4de4b8f555", "03d147a35f12bd03174693b52f2ec4bf91bfd6e5a4b76353fe1209bd8dc6c18687", "03b3e560af2590418a8ae84a2e699fc7d1bf4f45275bd951a123c8196819b69e96", "020457dd9249b4648e5e21e234592b83b27d95c0797b14994eca8cbeb5b8dee6b6", "02dc995337c1b6fd1ef8ef80cdfb179145bb923cf1640c1bb99e7ffec5ee7960c8", "037693493f2edb38b0a317159a8dd0326b34db8cd0e08ef4cfb84c8dc2714e521c"
            "02990ecf6d5753df5607076351d0d4d9fc205cf47762ca213616f62318db8207a1", "0280db8dc9387fc231559093db088e57c1a3a8418058ae462f430d4b3ee9780e61", "034551be04f031042e6fbf4ac4264863c6840dce5c097b3580760aec1eac77f66f", "02c3c43089d02fce24038f15f0e3bc32be0356756ce208777e0492ddd7803ad4bc", "02ffae6076f39fa2a640cef469f8db992b5a009999a98424bbc5539bc9defb69d7", "02c271bf33dace298904f64a58d74793b25b2ffa232e6c861a5b252d0bbf8b6fe7", "021fecfea903e49e6f48334797c86a56f6e044a32b011e4e41f46ff9667eaed9f1", "0304a3f540288e32670884dd4fdc3e7e7721d97d658ae95ba0ba5238db2b09d311", "0292cbf4fd4fab8be2142801dc7882b40534cc8d0cc512a76c977735a19a0bfc28", "02eb2576927ead7877ac6383d33ca70ae9997112ce1da4170baff132bf70d85f45", "02d79cbe60a73689e09bcdbee2763cb5f90220c0cd6f30f17e3912ad1a25a4aafd", "03100e89525898fcf0ba5cf466faebe774d2efabe2c6dbb055e7c7068168398510", "0378d4542e712a74d4557295c7ef5db80bf9d1c64fd61bb96e32fff847189233c9", "0381a539f9998f6a4cd7654939b3ceca7e284a049d09cdd0a924904b533d370c83", "02fd68227c6efabe769f22ab77ce9187b35ec52231d035b626b05f11639b270b81", "03f6d3403a0e0bd073123ef424a745a5f6517a17ba0f2883ba5290023423780cce", "03f332071803d15c38714ba105de01dbfa7351c80de952206ece4470e1c44143ff", "0349df5703d06c402cb5c16ada38f9064ff21b32f6549538372f2ddc752fa5f0f2", "02036ac926e01e926633a5c67943294de0e1400fcad76c31e07128187f9e74584b", "02714b838178a521a75b682f05de1bd75adc6722931dc1ba1216c9145d2cba158a"
            "029d18aa7764adc57bb87220a46b0f5794a13384c782bbae0330a9a101fa0815e8", "03c0d9fa8036a2fe796f0c48d6d8b6786859fc0109d7883ac973b6f91c6ec3536c", "03aaaf2371f037edc3afeda1d49613e8e91621630246ef872d9b46db67237aeed4", "022e1b1e0f96787a99a00f430a05a6350092d53b0f5e7e911bb8449370bd01a30c", "02e4bf20e1ff643645f1e7496f309f8cbb0cbf6aeb788d213eb34c41e37db6dcfb", "02479c9579d7affe72bdfa465a26f9350c75977d9db9770688949f390ae411f43a", "03713984aaefa28d745a4cfdd29b61c1917d24ffe411b7de331716cece89e8f4b4", "022a93f0c7c3d0183817bfcc1ec6a39ca012fafba169273428ebcaa7204b212b77", "03ea925c21971c961dcbfa852e456afa68d3408094e0885f227f663a040e4a8aa4", "02b7ba86e01af55739c5f4384e66161463bab827a94454d469f4f94cb91fa0390c", "035284162fd681e8128b2e1d4ea8a8be044db4f90dcc15f52ffd30197211291d85", "03a7487bfc36c8e6cb5e3dc616ac75a8ccaf66459d8ac7402e0b5f9d745d33e068", "032c753ff6e63271256f9b6704a44e3ff697de7060e0d8819bf0120e1c6cb1d2dc", "03f3bb73e3bc0264348615bf614d35f908c333dcfa519de0d5d7038b5753dee8df", "02d9f0234822268672df178f527b9a7031f7c6e98b7eb3f4991595d0edfaa8ad16", "02fa465fa8fe3aafeaf116d537439e458096e6e262f74c301f51aa4a48024fb976", "038cc3f504646bf9f7b6c1cd0890b0d42e0cd2d8334b1aee9af60469440f1621fe", "03c9bce245db14bcfd8b467c0fe3975a7c4f66be6a5c29e90177fd7dd5047c2a62", "02af647043663859831f0fee45f080115ad008554cb7afb41c50ef8fa5e3025e0c", "02d558c1c04d4c18f62b7620c29ffdfe0b6d85171442bd3e617384b9f402dffba2"
            "03023e0b7663815ce50d175b2a75950aeed7fc297e9ca3bf1a96d0bd1ca3480238", "0307c75b200d6d2120c61789167069683b881f09352f350e48328778963b465bb7", "025ba2c202ea111e715d08eda7a26c1d20279e61bcf05e91ba3513b7fcbb907941", "026b498a6957cb0ddd7379e9c4b4bf32517ebe033a4886719114be418930cc2d2b", "032cbc6d571c5d6fc6818ae2bfafebfe951a439a3bdac473e1d6c675b9aba98404", "02162129972a97194451eaddb503044f618c32817a55557e9b0a78416ed60c5441", "03f1fc845bc29294b5b2754834328d8e37c8770cab575bd710143c4ea08d116bfb", "02dc00685bf9de890662fe413b034abbb38f11f9fbc80f1c73bfa0435e06688c5b", "02ae78a6ca2e25bb0704357a680d0b335009f09219ad7638ea4a323d6c4f10afb4", "02e637b2ed10ec60edad483b34f0c1148dd16b59d0de5ec7f9a6657b937f4a66b5", "034dae5e0a6c87dafbb35f1be5f1f96f32bf0ebdc6912085e2fcfd2d1162de125d", "030a9a59131bb9f4c6495f0b613882c2cbcef32dd452a7f5e38c00c57e50a2a98f", "02d39949eb53baf273644e7aeca784926c6b40ff94a3cd4191fae468ec463a4a85", "02ded4e4b934140a6ede7e2bd509c57e502a0a6820ffdd96eb7e018e2b140211e6", "02f2cd991b4a447f47abfef0a7ec330354b18660635ffa4b0474ba12a98ac12a73", "02c6f694e4e7c920bd1510d0029e5e61fba7a15371496357f53ea9e34a34612612", "02ffe95074550c8a594909a2c8f393758c988220f951d584dd66fe5f3e9a9013dd", "03a4db2e42139749228a42b96c198be34e64895130d25ee53773f9b573ff70913d", "03bf33290cd659fc9d1069136af51bb4611fd8abbc7c552b9e25af8411979f64a4", "02286c2b4834bd976650b6d8757e2b7355dfe3247f737783176940bb376f6f4088"
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP16Exception = uint256S("0x00000000dd30457c001f4095d208cc1296b0eed002427aa599874af7a432b105");
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8");
        consensus.BIP65Height = 581885; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 330776; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.CSVHeight = 770112; // 00000000025e930139bac5c6c31a403776da130831ab85be56578f3fa75369bb
        consensus.SegwitHeight = 834624; // 00000000002b980fcd729daaa248fd9316a5200e9b367f4ff2c42453e84201ca
        consensus.MinBIP9WarningHeight = 836640; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
    
        // Bitcoin Global hard fork on test chain.
        consensus.BTGHeight = 1780318;
        consensus.BTGPremineWindow = 50;
        consensus.BTGPremineEnforceWhitelist = false;
        consensus.BTGPremineReward = 1000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000001495c1d5a01e2af8a23");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000000000056c49030c174179b52a928c870e6e8a822c75973b7970cfbd01"); // 1692000

        pchMessageStart[0] = 0x50;
        pchMessageStart[1] = 0xc9;
        pchMessageStart[2] = 0x6a;
        pchMessageStart[3] = 0x24;
        nDefaultPort = 18222;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 40;
        m_assumed_chain_state_size = 2;

        genesis = CreateGenesisBlock(1296688602, 414098458, 0x1d00ffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943"));
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("test-dnsseed.bitcoin-global.io");
        vSeeds.emplace_back("test-dnsseed.bitcoin-global.co");
        vSeeds.emplace_back("globt-dnsseed.bitcoin-global.co");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "globt";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;


        checkpointData = {
            {
                {546, uint256S("000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70")},
            }
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 4096 00000000000000b7ab6ce61eb6d571003fbe5fe892da4c9b740c49a07542462d
            /* nTime    */ 1569741320,
            /* nTxCount */ 52318009,
            /* dTxRate  */ 0.1517002392872353,
        };

        // Premine PubKeys for coin distribution.
        vPreminePubkeys = {
            "03dd8efa43ceac0b50bc667f8b84bed3de78cd2d4581ed6a871affef08d8626e6b", "029607c91b0019e6297102611399e172a89bfe674fabcc38c14aa972e9caa9755a", "02375c001becef3c3023d49d59481cef64f2e1ef32c5e7500d84e062a6a78d9797", "036d3e61a820c1a14355252626ea114747178d676332913d60598c06d5207f3813", "03b71f54b4436a18d751b49ad39b16159807424c4b6582631dbf9777d7a2121f25", "03fd99f34f98ddfcddf71a30afb63603fdab2ff7b5317de6272685842a288062e4", "027cdef4a0c32b72672677feea80e249e7510b1bf64a287058081cc2083f3e874c", "02779d8380f45f19ef513deb86727140b653914f45bcd66cde5785d94e98a4ea62", "0379ea002727afe109178e7b185023345154bdc6f94a9e24e6b4515a33ef94b70d", "03df9590af95d33e21dbca20546d1859a9298c91eaa3c75e0bc5dd98d6fac975cb", "032b19cf42fdd2d27a3ce65bb066a27e59e46ef4be0117d63d127f23554f68b343", "03dfcc37bd4c6f3069ea47ae4d6612d19a467db0eba03e5cff0ee5c1db15168a71", "037d8f4a91613ddc33b5f3916105edc80e9e2e12d512d8def4a172a71f8be72c7f", "030a0d33b36068b1030cb153d24ad6c67f476c8f800f4bdfa917df71aeb59c66e1", "030b968e01f9fd45cf16c7dd12319388c8a3b09489df16a60067bd64fd50009d3a", "02dd535da5dd420b72282391b06ef0ee4ffd1a42e3d88dfd1abe90fb8e3d1512f6", "03825497ccd03962518012ac9ef3d8671be54795280a3c62a0ead1b4274d2ee1dc", "02ea9b0a745ddb9b8421f32ac42082aa588ab8517f069a350be96811f025c19b97", "03f6ef8955b2dfcff2b805c7a0e22636ac563bc6e5efb9e088ae522d77ec64f5dc", "02143729b21ba324b4e43b66dd61ade28ece4eb3ac4bf9e2a7873ca4c700f542a4"
            "023ca8f6bb28e659ce8aed5ab53e0575e555226b1222aefadb7bc4698a13bd8521", "03a247f7ed8527e5f09ae354f2b32f7807e2fbb337cdd7a112e9721049b8d63fa5", "02f71b6380ecbdd37486f7e177946bd8d839994280a62bc9bfdb598db551cece23", "028b444477ee0267a8a0e5885fd4c5b30419378f687090641da79540f995b09608", "023e3fe17ad4031382c689e40130ef0f9013fa17697d3277bb9db48cd55d4fd624", "03b0fff0864ebb846fcb54f492135359fb6f11b488a00c1188058899bce2517727", "02a8da0db27d5c6d52d517ecca5288c8e6a402a2d13c4a5165bb7062344f960b8c", "026002560e65cdd8e2c8d0aa52329994cb655263902c811a7a54671d1ceffc9ff4", "0262db182cefe4807a7b3aa95df9eb21d5d4347661c8874c17e08c3ac838c5b67a", "0264a4c8aed7141f9ca346772c9f65a0efa615bb5e4690c26812e26d6b4b70e90a", "02b3c7a24514e5e8468691e19d6c1b9532e2517272059f8165a0d7c98e08fd1bf4", "02bfba321d353dff4fe8bb1717043afc2fcecae24ebdc17ad375f013339666fe5c", "026a972f47cd3724ad8de5db6b28605a2d9cd0720921eee6fdad2152d40ef2c4b7", "0222476d4a7e8019ca744b393acae0909d2e41b1bdc1d11247c89862c66eeb603e", "02e6fd364985ede80a64b35d9002ecd9a91af2c7f7cff6030f117d827b6be6db5f", "02758a7463e3defa18c7974b4d138afda7bff729df33384a8d6d21e844aac27409", "029bced43755766117d8ef8ab16a29eac9389ff51b28cbdd4b37448c69e74f68a6", "03b0a27ebd4dcf7b9a1c624c87340eb5f08bd834254f2a7d45af29b60ceeaa9564", "0387770f38a7441d6bfd25f60828750b3137a8d28f855b7e645975ce5601c550ea", "030d1569dfc1a11b8f95b9b64c4bd30055bcb0c32b9fbe8aed284fb8a51375bf42"
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in functional tests)
        consensus.CSVHeight = 432; // CSV activated on regtest (Used in rpc activation tests)
        consensus.SegwitHeight = 0; // SEGWIT is always activated on regtest unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Bitcoin Global hard fork on reg chain.
        consensus.BTGHeight = 3000;
        consensus.BTGPremineWindow = 50;
        consensus.BTGPremineEnforceWhitelist = true;
        consensus.BTGPremineReward = 100;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xb6;
        pchMessageStart[1] = 0x0c;
        pchMessageStart[2] = 0x4d;
        pchMessageStart[3] = 0x86;
        nDefaultPort = 68222;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateActivationParametersFromArgs(args);

        genesis = CreateGenesisBlock(1296688602, 2, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;

        checkpointData = {
            {
                {0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "globr";

        // Premine PubKeys for coin distribution.
        vPreminePubkeys = {
            "022df9dd6c032bb01871e0a46ccc305bd80b32daf2230fb917725539a0bbebca19", "03317341a33373f319a3520662faa85a0231d3259a287d06cbfa943f620c1e7471", "02f591d3ffbcd00a988acd1c35dd1fdba3d33833fb7a9132633a83a0e2363edc15", "03642391def4438c925feaa3c6f9fef65a5295a100f69afb9aa90934e2cbefc6c5", "02b024335bbed2f74b2133e7a515ce0089c48fafb20413de6769a2817cc2a9a289"
        };
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (gArgs.IsArgSet("-segwitheight")) {
        int64_t height = gArgs.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

bool CChainParams::IsPremineAddressScript(const CScript& scriptPubKey, uint32_t height) const {
    assert((uint32_t)consensus.BTGHeight <= height &&
           height < (uint32_t)(consensus.BTGHeight + consensus.BTGPremineWindow));
    // assert(vPreminePubkeys.size() == 100);

    int block = height - consensus.BTGHeight;
    CPubKey vPreminePubkey = CPubKey(ParseHex(vPreminePubkeys[block % vPreminePubkeys.size()]));  // Round robin.
    CScript target_scriptPubKey = GetScriptForRawPubKey(vPreminePubkey);
    return target_scriptPubKey == scriptPubKey;
}
