// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2020 The Bitcoin Global developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_TEST_COMPATTESTS_H
#define BITCOIN_QT_TEST_COMPATTESTS_H

#include <QObject>
#include <QTest>

class CompatTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void bswapTests();
};

#endif // BITCOIN_QT_TEST_COMPATTESTS_H
