#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets>

#include <QDebug>

#include "serialclient.h"
#include "ui_serialclient.h"

class TestGuiSc : public QObject
{
    Q_OBJECT

public:
    TestGuiSc();
    ~TestGuiSc();

    SerialClient sc;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_bc_btns();
    void test_bc_labels();
    void test_bc_grid();

    void test_btn_clicks();

    void test_btnScan();

};

TestGuiSc::TestGuiSc() {}

TestGuiSc::~TestGuiSc() {}

void TestGuiSc::initTestCase() {}

void TestGuiSc::cleanupTestCase() {}

// Проверка инициализации кнопок BtClient
void TestGuiSc::test_bc_btns() {
    QVERIFY2(sc.ui->btnBack, "btnBack not initialized...");
    QVERIFY2(sc.ui->btnConnect, "btnConnect not initialized...");
    QVERIFY2(sc.ui->btnDisconnect, "btnDisconnect not initialized...");
    QVERIFY2(sc.ui->btnScan, "btnScan not initialized...");
}

void TestGuiSc::test_bc_labels() {
    QVERIFY2(sc.ui->labelPorts, "labelPorts not initialized...");
    QVERIFY2(sc.ui->labelInfo, "labelInfo not initialized...");
}

void TestGuiSc::test_bc_grid() {
    QVERIFY2(sc.ui->horizontalLayout, "horizontalLayout not initialized...");
    QVERIFY2(sc.ui->horizontalLayout_2, "horizontalLayout_2 not initialized...");
}

void TestGuiSc::test_btn_clicks() {
    QTest::mouseClick(sc.ui->btnBack, Qt::LeftButton);
    QTest::mouseClick(sc.ui->btnConnect, Qt::LeftButton);
    QTest::mouseClick(sc.ui->btnDisconnect, Qt::LeftButton);
    QTest::mouseClick(sc.ui->btnScan, Qt::LeftButton);
}

// Проверка, что после нажатия кнопки btnScan
// предыдущие значения из списка удаляться
void TestGuiSc::test_btnScan() {
    QTest::mouseClick(sc.ui->btnScan, Qt::LeftButton);

    QVERIFY(sc.ui->listPortsWidget->count() == 0);
}

QTEST_MAIN(TestGuiSc)

#include "testGuiSc.moc"
