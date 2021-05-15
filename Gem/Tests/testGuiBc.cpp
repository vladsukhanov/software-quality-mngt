#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets>

#include <QDebug>

#include "btclient.h"
#include "ui_btclient.h"

class TestGuiBc : public QObject
{
    Q_OBJECT

public:
    TestGuiBc();
    ~TestGuiBc();

    BtClient bc;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_bc_btns();
    void test_bc_labels();
    void test_bc_grid();

    void test_btn_clicks();

    void test_btnScan();

};

TestGuiBc::TestGuiBc() {}

TestGuiBc::~TestGuiBc() {}

void TestGuiBc::initTestCase() {}

void TestGuiBc::cleanupTestCase() {}

// Проверка инициализации кнопок BtClient
void TestGuiBc::test_bc_btns() {
    QVERIFY2(bc.ui->btnBack, "btnBack not initialized...");
    QVERIFY2(bc.ui->btnConnect, "btnConnect not initialized...");
    QVERIFY2(bc.ui->btnDisconnect, "btnDisconnect not initialized...");
    QVERIFY2(bc.ui->btnScan, "btnScan not initialized...");
}

void TestGuiBc::test_bc_labels() {
    QVERIFY2(bc.ui->labelDevices, "labelDevices not initialized...");
    QVERIFY2(bc.ui->labelInfo, "labelInfo not initialized...");
}

void TestGuiBc::test_bc_grid() {
    QVERIFY2(bc.ui->gridLayout, "gridLayout not initialized...");
    QVERIFY2(bc.ui->gridLayout_2, "gridLayout_2 not initialized...");
    QVERIFY2(bc.ui->horizontalLayout, "horizontalLayout not initialized...");
}

void TestGuiBc::test_btn_clicks() {
    QTest::mouseClick(bc.ui->btnBack, Qt::LeftButton);
    QTest::mouseClick(bc.ui->btnConnect, Qt::LeftButton);
    QTest::mouseClick(bc.ui->btnDisconnect, Qt::LeftButton);
    QTest::mouseClick(bc.ui->btnScan, Qt::LeftButton);
}

// Проверка, что после нажатия кнопки btnScan
// предыдущие значения из списка удаляться
void TestGuiBc::test_btnScan() {
    QTest::mouseClick(bc.ui->btnScan, Qt::LeftButton);

    QVERIFY(bc.ui->listDevicesWidget->count() == 0);
}

QTEST_MAIN(TestGuiBc)

#include "testGuiBc.moc"
