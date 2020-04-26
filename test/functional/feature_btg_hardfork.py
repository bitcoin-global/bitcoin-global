#!/usr/bin/env python3
# Copyright (c) 2020 The Bitcoin Global developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test cases for Bitcoin Global hard fork """

from test_framework.test_framework import BitcoinTestFramework

from decimal import Decimal
from test_framework.blocktools import create_coinbase, COIN
from test_framework.messages import CBlock
from test_framework.script import CScript, OP_NOP, OP_RETURN
from test_framework.util import assert_equal, assert_raises_rpc_error

# =========== CONSTS
BTG_HARDFORK_HEIGHT = 3000
BTG_HARDFORK_LENGTH = 50
BTG_PREMINE_REWARD  = 100
BTG_PREMINE_PUBKEYS = [
    "022df9dd6c032bb01871e0a46ccc305bd80b32daf2230fb917725539a0bbebca19",
    "03317341a33373f319a3520662faa85a0231d3259a287d06cbfa943f620c1e7471",
    "02f591d3ffbcd00a988acd1c35dd1fdba3d33833fb7a9132633a83a0e2363edc15",
    "03642391def4438c925feaa3c6f9fef65a5295a100f69afb9aa90934e2cbefc6c5",
    "02b024335bbed2f74b2133e7a515ce0089c48fafb20413de6769a2817cc2a9a289"
]

BTG_NOT_PREMINE_PUBKEYS = [
    "0316797c1aa441ef4b393a16f4bb60d0ba4347603c0fef50121eb364c62295d679",
    "0322a871b20874ee8d3cbb78188566d4da0aa9de5af9bcb865bc8546e24821c0f8"
]

class BTGForkTest(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 1
        self.setup_clean_chain = True
        self.supports_cli = True

    def mine_large_block(self, prefork=True):
        best_block = self.nodes[0].getblock(self.nodes[0].getbestblockhash())
        height = int(best_block["height"]) + 1

        # Build the coinbase transaction (with large scriptPubKey)
        coinbase_tx = create_coinbase(height)
        coinbase_tx.vin[0].nSequence = 2 ** 32 - 1
        coinbase_tx.vout[0].scriptPubKey = CScript([OP_RETURN] + [OP_NOP] * (1500000 if prefork else 950000))
        coinbase_tx.rehash()

        # Build the block
        block = CBlock()
        block.nVersion = best_block["version"]
        block.hashPrevBlock = int(best_block["hash"], 16)
        block.nTime = int(best_block["time"]) + 1
        block.nBits = int('207fffff', 16)
        block.nNonce = 0
        block.vtx = [coinbase_tx]
        block.hashMerkleRoot = block.calc_merkle_root()
        block.solve()

        # Submit to the node
        return self.nodes[0].submitblock(hexdata=block.serialize().hex())

    def mine_chain(self, height):
        self.nodes[0].generate(height)

    def skip_test_if_missing_module(self):
        self.skip_if_no_wallet()

    def create_premine_tx(self, pubkey=None):
        tmpl = self.nodes[0].getblock(self.nodes[0].getbestblockhash())
        coinbase_tx = create_coinbase(height=tmpl['height'] + 1, pubkey=pubkey)
        coinbase_tx.vin[0].nSequence = 2 ** 32 - 2
        coinbase_tx.vout[0].nValue = COIN * BTG_PREMINE_REWARD
        coinbase_tx.rehash()
        return coinbase_tx

    def create_coinbase_block(self, coinbase_txs):
        tmpl = self.nodes[0].getblock(self.nodes[0].getbestblockhash())
        block = CBlock()
        block.nVersion = 4
        block.hashPrevBlock = int(tmpl["hash"], 16)
        block.nTime = tmpl['time'] + 1
        block.nBits = int(tmpl["bits"], 16)
        block.nNonce = 0
        block.vtx = coinbase_txs
        block.hashMerkleRoot = block.calc_merkle_root()
        block.solve()
        return block

    def submitblock_for_coinbase(self, coinbase_txs):
        block = self.create_coinbase_block(coinbase_txs)
        return self.nodes[0].submitblock(hexdata=block.serialize().hex())

    def run_test(self):
        # prepare chain for BTG Hard fork tests.
        node = self.nodes[0]
        self.mine_chain(BTG_HARDFORK_HEIGHT - 2)

        # (1) BTG height reached
        self.log.info('reached BTG Hard Fork')
        mining_info = node.getmininginfo()
        assert_equal(mining_info['blocks'], BTG_HARDFORK_HEIGHT - 2)
        assert_equal(mining_info['chain'], 'regtest')

        # (2) mine large block, before hard fork
        assert_equal("bad-blk-length", self.mine_large_block(prefork=True))
        node.generate(1)

        # (3) fail mining block for various reasons
        self.log.info('test premine validation')

        # (4) generate premine block, should fail
        assert_raises_rpc_error(-1, "bad-premine-coinbase-output", node.generate, 1)

        # (5) mine premine blocks, should fail
        fail_tx1 = self.create_premine_tx(bytes.fromhex(BTG_NOT_PREMINE_PUBKEYS[0]))
        fail_tx2 = self.create_premine_tx(bytes.fromhex(BTG_NOT_PREMINE_PUBKEYS[1]))
        fail_tx3 = self.create_premine_tx(bytes.fromhex("0322a871b20874ee8d3cbb78188566d4"))

        assert_equal(self.submitblock_for_coinbase([fail_tx1, fail_tx2]), "bad-cb-multiple")
        assert_equal(self.submitblock_for_coinbase([fail_tx1]), "bad-premine-coinbase-scriptpubkey")
        assert_equal(self.submitblock_for_coinbase([fail_tx3]), "bad-premine-coinbase-scriptpubkey")

        # (6) mine all remaining premine-reserved blocks, should pass
        mining_payment = self.nodes[0].gettxoutsetinfo()["total_amount"]
        for i in range(BTG_HARDFORK_LENGTH):
            # create coinbase premine block
            coinbase_tx = self.create_premine_tx(bytes.fromhex(BTG_PREMINE_PUBKEYS[i % len(BTG_PREMINE_PUBKEYS)]))
            block = self.create_coinbase_block([coinbase_tx])
            self.nodes[0].submitblock(hexdata=block.serialize().hex())

            # verify block acceptance and reward
            premine_payment = self.nodes[0].gettxoutsetinfo()["total_amount"]
            assert (self.nodes[0].getbestblockhash() == block.hash)
            assert ((premine_payment - mining_payment) == BTG_PREMINE_REWARD)
            mining_payment = premine_payment

        # (7) postfork checks
        self.log.info('test postfork validation')

        # (8) mine large block after premine, should fail
        assert_equal("bad-blk-weight", self.mine_large_block(prefork=False))

        # (9) accept new mining blocks, should fail
        node.generate(10)
        assert_equal(self.submitblock_for_coinbase([fail_tx1, fail_tx2]), "bad-cb-multiple")

        # (10) verify current stats
        tmpl = self.nodes[0].getblock(self.nodes[0].getbestblockhash())
        assert_equal(tmpl["height"], BTG_HARDFORK_HEIGHT + BTG_HARDFORK_LENGTH + 9)

if __name__ == '__main__':
    BTGForkTest().main()
