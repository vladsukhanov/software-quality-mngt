#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

#include "btclient.h"
#include "graphwindow.h"
#include "serialclient.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Подключение по COM-порту / Bluetooth
    SerialClient *serialClient = nullptr;
    BtClient *btClient = nullptr;

    // Графики
    GraphWindow *graph1256 = nullptr;
    GraphWindow *graph1292r = nullptr;

public slots:
    void parseBtConnectionState(bool state, QString deviceName);
    void parseSerialConnectionState(bool state, QString portName);

private slots:
    void on_btnInitSerial_clicked();
    void on_btnInitBt_clicked();
    void on_btnStartSerial_clicked();
    void on_btnStartBt_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
