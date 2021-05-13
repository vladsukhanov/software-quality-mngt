#include <QtTest>
#include <QCoreApplication>

#include <QDebug>

#include "butterworthfilter.h"

class CheckTest : public QObject
{
    Q_OBJECT

public:
    CheckTest();
    ~CheckTest();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_butterworth();
    void test_poles_and_zeros();

};

CheckTest::CheckTest() {}

CheckTest::~CheckTest() {}

void CheckTest::initTestCase() {}

void CheckTest::cleanupTestCase() {}

void CheckTest::test_butterworth() {
    qDebug() << "Test ButterWorth initializing...";
    ButterworthFilter bf;
    QCOMPARE(bf.filter(14.8686313166), 72);
}

void CheckTest::test_poles_and_zeros() {
    qDebug() << "Test Poles&Zeros initializing...";
    QCOMPARE(3, NPOLES);
    QCOMPARE(3, NZEROS);
}

QTEST_MAIN(CheckTest)

#include "tst_checktest.moc"
