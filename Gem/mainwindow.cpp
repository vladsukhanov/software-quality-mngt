#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "btclient.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    qDebug() << "MainWindow();";

    // Инициализация класса графика
    // 1 - P(t); 2 - deltaP(t); 3 - deltaP(P);
    graph1256 = new GraphWindow(1);
//    graph1292r = new GraphWindow(1);

    // Инициализация класса Bluetooth
    btClient = new BtClient(this);
    connect(btClient, &BtClient::connectionStatusChanged, this, &MainWindow::parseBtConnectionState);

    // Инициалиазция класса Serial
    serialClient = new SerialClient(this);
    connect(serialClient, &SerialClient::connectionStatusChanged, this, &MainWindow::parseSerialConnectionState);

    connect(serialClient, &SerialClient::dataAvailable, graph1256, &GraphWindow::getDataAvailable);
//    connect(serialClient, &SerialClient::dataAvailable, graph1292r, &GraphWindow::getDataAvailable);

    connect(btClient, &BtClient::dataAvailable, graph1256, &GraphWindow::getDataAvailable);
//    connect(btClient, &BtClient::dataAvailable, graph1292r, &GraphWindow::getDataAvailable);

    ui->btnStartSerial->setEnabled(false);
    ui->btnStartBt->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;

    delete serialClient;
    delete btClient;

    delete graph1256;
    delete graph1292r;

    qDebug() << "~MainWindow();";
}

void MainWindow::parseBtConnectionState(bool state, const QString &deviceName) {
    if (state) {
        ui->btnStartBt->setEnabled(true);
        ui->labelBt->setText("Connected to: " + deviceName);
    }
    else {
        ui->btnStartBt->setEnabled(false);
        ui->labelBt->setText("");
    }
}

void MainWindow::parseSerialConnectionState(bool state, const QString &portName) {
    if (state) {
        ui->btnStartSerial->setEnabled(true);
        ui->labelSerial->setText("Connected to: " + portName);
    }
    else {
        ui->btnStartSerial->setEnabled(false);
        ui->labelSerial->setText("");
    }
}

void MainWindow::on_btnInitSerial_clicked() {
    this->hide();
    serialClient->exec();
    this->show();
}

void MainWindow::on_btnInitBt_clicked() {
    this->hide();
    btClient->exec();
    this->show();
}

void MainWindow::on_btnStartSerial_clicked() {
    this->hide();
    graph1256->exec();
//    graph1292r->exec();
    this->show();
}

void MainWindow::on_btnStartBt_clicked() {
    this->hide();
    graph1256->exec();
//    graph1292r->exec();
    this->show();
}
