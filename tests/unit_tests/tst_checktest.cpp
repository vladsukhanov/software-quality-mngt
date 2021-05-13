#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

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
    void test_case1();
    void test_case2();

};

CheckTest::CheckTest()
{

}

CheckTest::~CheckTest()
{

}

void CheckTest::initTestCase()
{

}

void CheckTest::cleanupTestCase()
{

}

void CheckTest::test_case1()
{
//    ButterworthFilter bf;
//    QCOMPARE(bf.filter(15), 72);

//    QCOMPARE(5, NZEROS);
}

void CheckTest::test_case2() {
    QCOMPARE(3, NPOLES);
}

QTEST_MAIN(CheckTest)

#include "tst_checktest.moc"
