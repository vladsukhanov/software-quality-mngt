#include "btclient.h"
#include "ui_btclient.h"

#include <QDebug>
#include <QtEndian>
#include <QTimer>

#include "mainwindow.h"

BtClient::BtClient(QWidget *parent) : QDialog(parent), ui(new Ui::BtClient) {
    ui->setupUi(this);

    qDebug() << "BtClient();";

    // Check if Bluetooth is available on this device
    if (_localBtDevice.isValid()) {
        qDebug() << "localDevice is valid";

        // Turn Bluetooth on
        _localBtDevice.powerOn();

        // Read local device name
        _localBtDeviceName = _localBtDevice.name();
        _localBtDeviceAddress = _localBtDevice.address();
        qDebug() << "localDeviceName: " << _localBtDeviceName << "(" << _localBtDeviceAddress << ")";

        // Make it visible to others
        _localBtDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        // Get connected devices
        QList<QBluetoothAddress> remotes;
        remotes = _localBtDevice.connectedDevices();

        qDebug() << "remotes: " << remotes;
    }

    ui->btnConnect->setEnabled(false);
    ui->btnDisconnect->setEnabled(false);
}

BtClient::~BtClient() {
    delete ui;

    delete _pDiscoveryAgent;
    delete _pController;
    delete _pChosenDevice;

    _listDevices.clear();
    qDeleteAll(_listDevices);

    qDebug() << "~BtClient();";
}

void BtClient::discoveryBtDevice() {
    // Create a discovery agent and connect to its signals
    _pDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    _pDiscoveryAgent->setLowEnergyDiscoveryTimeout(LE_TIMER);

    // Device discovered
    connect(_pDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, [this](const QBluetoothDeviceInfo &device) {
        qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
        ui->listDevicesWidget->addItem(device.name() /*+ " ( \"" + device.address().toString() + "\" )"*/);

        _listDevices.append(new DeviceInfo(device));
    });

    // Discovery device errors
    connect(_pDiscoveryAgent,
            static_cast<void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error),
            this, [this](QBluetoothDeviceDiscoveryAgent::Error error) {
        if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
            qDebug() << "The Bluetooth adaptor is powered off.";
        else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
            qDebug() << "Writing or reading from the device resulted in an error.";
        else
            qDebug() << "An unknown error has occurred.";
    });

    // Discovery device finished
    connect(_pDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, [this]() {
        ui->btnScan->setEnabled(true);

        qDebug() << "Scan finished.";
    });

    // Discovery device canceled
    connect(_pDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, [this]() {
        ui->btnScan->setEnabled(true);

        qDebug() << "Scan finished.";
    });

    // Start a discovery
    _pDiscoveryAgent->start();
}

void BtClient::clearLabelInfo() {
    ui->labelInfo->setText("");
}

void BtClient::on_btnScan_clicked() {
    ui->listDevicesWidget->clear();

    _listDevices.clear();

    ui->btnConnect->setEnabled(false);
    ui->btnScan->setEnabled(false);

//    ui->statusBar->showMessage("Scanning...", LE_TIMER);
    ui->labelInfo->setText("Scanning...");
    QTimer::singleShot(5000, this, SLOT(clearLabelInfo()));

    qDebug() << "Scanning...";

//    _discoveryAgent->stop();
    discoveryBtDevice();
}

void BtClient::on_btnConnect_clicked() {
    connectBtDevice();
}

void BtClient::on_btnDisconnect_clicked() {
    disconnectBtDevice();
}

// Отключение от Bt устройства
void BtClient::disconnectBtDevice() {
    if (_pController->state() != QLowEnergyController::UnconnectedState)
        _pController->disconnectFromDevice();

    qDebug() << "Bluetooth device disconnected.";
}

// Подключение к Bt устройству
void BtClient::connectBtDevice() {
    // Создание соединения
    _pController = QLowEnergyController::createCentral(_pChosenDevice->getDevice(), this);

    connect(_pController, &QLowEnergyController::serviceDiscovered,
            this, &BtClient::serviceDiscovered);

    connect(_pController, &QLowEnergyController::discoveryFinished,
            this, &BtClient::serviceScanDone);

    connect(_pController, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error),
            this, [this](QLowEnergyController::Error error) {
        Q_UNUSED(error);

        ui->btnConnect->setEnabled(true);
//        ui->statusBar->showMessage("Cannot connect to remote device.");
        ui->labelInfo->setText("Cannot connect to remote device.");
        QTimer::singleShot(5000, this, SLOT(clearLabelInfo()));

        qDebug() << "Cannot connect to remote device.";
    });

    connect(_pController, &QLowEnergyController::connected, this, [this]() {
        qDebug() << "Controller connected. Search services...";
        _pController->discoverServices();

        ui->btnDisconnect->setEnabled(true);
        ui->btnConnect->setEnabled(false);

//        ui->statusBar->showMessage("Connected: " + _pChosenDevice->getName());
        ui->labelInfo->setText("Connected: " + _pChosenDevice->getName());

        connectedState = true;
        emit connectionStatusChanged(connectedState, _pChosenDevice->getName());
    });

    connect(_pController, &QLowEnergyController::disconnected, this, [this]() {
        qDebug() << "LowEnergy controller disconnected.";


        ui->btnConnect->setEnabled(true);
        ui->btnDisconnect->setEnabled(false);

//        ui->statusBar->showMessage("");
        ui->labelInfo->setText("");

        connectedState = false;
        emit connectionStatusChanged(connectedState, _pChosenDevice->getName());
    });

    _pController->connectToDevice();

    ui->btnConnect->setEnabled(false);
//    ui->statusBar->showMessage("Connecting...");
    ui->labelInfo->setText("Connecting...");
    QTimer::singleShot(5000, this, SLOT(clearLabelInfo()));

    qDebug() << "Trying to connect to Bt device...";
}

void BtClient::serviceDiscovered(const QBluetoothUuid &serviceUuid) {
    if (serviceUuid == QBluetoothUuid(QBluetoothUuid::BloodPressure))
        foundBloodPressureServiceState = true;
}

void BtClient::serviceScanDone() {
    if (foundBloodPressureServiceState)
        _pBloodPressureService = _pController->createServiceObject(QBluetoothUuid(QBluetoothUuid::BloodPressure), this);

    if (_pBloodPressureService) {
        connect(_pBloodPressureService, &QLowEnergyService::stateChanged, this, &BtClient::serviceStateChanged);
        connect(_pBloodPressureService, &QLowEnergyService::characteristicChanged, this, &BtClient::getPressureValue);
        connect(_pBloodPressureService, &QLowEnergyService::descriptorWritten, this, &BtClient::confirmedDescriptorWrite);
        _pBloodPressureService->discoverDetails();
    }
}

void BtClient::on_listDevicesWidget_itemSelectionChanged() {
    _pChosenDevice = _listDevices.at(ui->listDevicesWidget->currentRow());

    if (!ui->btnDisconnect->isEnabled())
        ui->btnConnect->setEnabled(true);

    qDebug() << "Device name: " << _pChosenDevice->getName() << " Device address: " << _pChosenDevice->getAddress();
}

void BtClient::serviceStateChanged(QLowEnergyService::ServiceState s) {
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug() << "Discovering services...";
        break;
    case QLowEnergyService::ServiceDiscovered: {
        qDebug() << "Service discovered.";

        const QLowEnergyCharacteristic hrChar = _pBloodPressureService->characteristic(QBluetoothUuid(QBluetoothUuid::IntermediateCuffPressure));
        if (!hrChar.isValid()) {
            qDebug() << "Characteristic not valid.";
            break;
        }

        qDebug() << hrChar.name() << "FOUND";

        _descriptor = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (_descriptor.isValid()) {
            _pBloodPressureService->writeDescriptor(_descriptor, QByteArray::fromHex("0100"));

            qDebug() << "Descriptor is valid.";
        }
        break;
    }
    default:
        break;
    }
}

void BtClient::getPressureValue(const QLowEnergyCharacteristic &c, const QByteArray &value) {
    // ignore any other characteristic change -> shouldn't really happen though
    if (c.uuid() != QBluetoothUuid(QBluetoothUuid::IntermediateCuffPressure)) {
        qDebug() << "UUID doesn't match IntermediateCuffPressure";

        return;
    }

//    int32_t myInt1 = (uchar)value[0] + ((uchar)value[1] << 8) + ((uchar)value[2] << 16) + ((uchar)value[3] << 24);
//    emit updatedData(myInt1);

    emit dataAvailable(value);
}

void BtClient::confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value) {
    if (d.isValid() && d == _descriptor && value == QByteArray::fromHex("0000")) {
        // disabled notifications -> assume disconnect intent
        _pController->disconnectFromDevice();
        delete _pBloodPressureService;
        _pBloodPressureService = nullptr;
    }
    qDebug() << "Descriptor written.";
}

void BtClient::on_btnBack_clicked() {
    this->done(1);
}
