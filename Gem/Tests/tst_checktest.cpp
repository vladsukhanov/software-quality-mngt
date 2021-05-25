//#include <QtTest>
//#include <QCoreApplication>

//#include <QDebug>

//#include "butterworthfilter.h"
//#include "deviceinfo.h"
//#include "btclient.h"

//class CheckTest : public QObject
//{
//    Q_OBJECT

//public:
//    CheckTest();
//    ~CheckTest();

//private slots:
//    void initTestCase();
//    void cleanupTestCase();

//    void test_butterworth();
//    void test_poles_and_zeros();

//    void test_deviceInfo();

//    void test_timer();

//    void test_ble_local_device();

//};

//CheckTest::CheckTest() {}

//CheckTest::~CheckTest() {}

//void CheckTest::initTestCase() {}

//void CheckTest::cleanupTestCase() {}

//// Проверка на правильность выполнения функции фильтрации фильтра
//void CheckTest::test_butterworth() {
//    ButterworthFilter bf;
//    double result = bf.filter(15);
//    double delta = 0.0001;
//    double expected = 14.8686;

//    qDebug() << "Result value = " << result;

//    QVERIFY(expected + delta > result && expected - delta < result);
//}

//// Проверка на соответсвие Poles и Zeros
//void CheckTest::test_poles_and_zeros() {
//    QCOMPARE(3, NPOLES);
//    QCOMPARE(3, NZEROS);
//}

//// Проверка на создание BLE устройства
//void CheckTest::test_deviceInfo() {
//    QBluetoothDeviceInfo device;
//    DeviceInfo di(device);

//    // Проверка на создание дефолтного устройства BLE
//    QCOMPARE("00:00:00:00:00:00", di.getAddress());
//    QCOMPARE("", di.getName());
//}

//void CheckTest::test_timer() {
//    QCOMPARE(LE_TIMER, 5000);
//}

//// Проверка локального устройства BLE на соответствие
//void CheckTest::test_ble_local_device() {
//    QBluetoothLocalDevice _localBtDevice;

//    QVERIFY(_localBtDevice.isValid());

//    QString _localBtDeviceName = _localBtDevice.name();
//    QBluetoothAddress _localBtDeviceAddress = _localBtDevice.address();

//    QCOMPARE("REDMIBOOK-14-VS", _localBtDeviceName);
//    QCOMPARE("E4:AA:EA:98:77:38", _localBtDeviceAddress.toString());
//}

//QTEST_MAIN(CheckTest)

//#include "tst_checktest.moc"
