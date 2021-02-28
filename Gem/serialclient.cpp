#include "serialclient.h"
#include "ui_serialclient.h"

#include <QDebug>

SerialClient::SerialClient(QWidget *parent) : QDialog(parent), ui(new Ui::SerialClient) {
    ui->setupUi(this);

    qDebug() << "SerialClient();";

    info = new QSerialPortInfo();

    ui->btnConnect->setEnabled(false);
    ui->btnDisconnect->setEnabled(false);
}

SerialClient::~SerialClient() {
    delete ui;

    serial->close();
    delete serial;
    delete info;

    qDebug() << "~SerialClient();";
}

QByteArray SerialClient::readData() {
    return serial->readAll();
}

void SerialClient::writeData(QByteArray str) {
    serial->write(str);
}

void SerialClient::on_listPortsWidget_itemSelectionChanged() {
    if (!ui->btnDisconnect->isEnabled())
        ui->btnConnect->setEnabled(true);

    qDebug() << _listPorts.at(ui->listPortsWidget->currentRow()).portName();
}

void SerialClient::on_btnBack_clicked() {
    this->done(1);
}

void SerialClient::on_btnConnect_clicked() {
    QString portName = _listPorts.at(ui->listPortsWidget->currentRow()).portName();

    serial = new QSerialPort(this);
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);

    if (serial->isOpen() && serial->isReadable()) {
        ui->labelInfo->setText("Connected to " + portName);

        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);

        connectedState = true;
        emit connectionStatusChanged(connectedState, portName);
    }
    else {
        ui->labelInfo->setText("Can not open " + portName);

        ui->btnConnect->setEnabled(true);
        ui->btnDisconnect->setEnabled(false);

        connectedState = false;
        emit connectionStatusChanged(connectedState, portName);
    }

    connect(serial, &QSerialPort::readyRead, this, [this]() {
        emit dataAvailable(readData());
    });
}

void SerialClient::on_btnScan_clicked() {
    ui->listPortsWidget->clear();
    _listPorts.clear();

    _listPorts = info->availablePorts().toVector();

    for (int i = 0; i < _listPorts.count(); i++) {
        ui->listPortsWidget->addItem(_listPorts[i].portName());

        qDebug() << "Name : " << _listPorts[i].portName();
        qDebug() << "Description : " << _listPorts[i].description();
        qDebug() << "Manufacturer: " << _listPorts[i].manufacturer();
    }

    ui->btnConnect->setEnabled(false);
}

void SerialClient::on_btnDisconnect_clicked() {
    serial->close();

    ui->btnConnect->setEnabled(true);
    ui->btnDisconnect->setEnabled(false);

    connectedState = false;
    emit connectionStatusChanged(connectedState, "Nope");

    ui->labelInfo->setText("");
}
