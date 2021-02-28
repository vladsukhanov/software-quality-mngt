#ifndef BTCLIENT_H
#define BTCLIENT_H

#include <QDialog>

#include "deviceinfo.h"

// Информация о собственном устройстве
#include <QBluetoothLocalDevice>

// Информация о устройствах поблизости
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>

// Классы для BLE
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyDescriptor>

#include <QListWidget>

namespace Ui { class BtClient; }

class BtClient : public QDialog {
    Q_OBJECT

public:
    explicit BtClient(QWidget *parent = nullptr);
    ~BtClient();

    bool connectedState = false;

    void discoveryBtDevice();

signals:
    void connectionStatusChanged(bool state, QString deviceName);
    void dataAvailable(QByteArray data);

public slots:
    void clearLabelInfo();

private slots:
    // Widgets
    void on_btnBack_clicked();
    void on_btnScan_clicked();
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_listDevicesWidget_itemSelectionChanged();

    // QLowEnergyController
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void disconnectBtDevice();
    void connectBtDevice();
    void serviceScanDone();

    // QLowEnergyService
    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void getPressureValue(const QLowEnergyCharacteristic &c,
                              const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d,
                              const QByteArray &value);

private:
    Ui::BtClient *ui;

    QBluetoothLocalDevice _localBtDevice;
    QString _localBtDeviceName;
    QBluetoothAddress _localBtDeviceAddress;

    QBluetoothDeviceDiscoveryAgent *_pDiscoveryAgent = nullptr;

    QVector<DeviceInfo *> _listDevices;

    DeviceInfo *_pChosenDevice = nullptr;

    QLowEnergyController *_pController = nullptr;
    QLowEnergyService *_pBloodPressureService = nullptr;
    QLowEnergyDescriptor _descriptor;

    bool foundBloodPressureServiceState = false;
};

#endif // BTCLIENT_H
