#ifndef SERIALCLIENT_H
#define SERIALCLIENT_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui { class SerialClient; }

class SerialClient : public QDialog {
    Q_OBJECT

public:
    explicit SerialClient(QWidget *parent = nullptr);
    ~SerialClient();

    QByteArray readData();
    void writeData(QByteArray);

    bool connectedState = false;

    friend class TestGuiSc;

signals:
    void dataAvailable(QByteArray data);
    void connectionStatusChanged(bool state, QString portName);

private slots:
    void on_listPortsWidget_itemSelectionChanged();
    void on_btnConnect_clicked();
    void on_btnScan_clicked();
    void on_btnDisconnect_clicked();
    void on_btnBack_clicked();

private:
    Ui::SerialClient *ui;

    QSerialPort *serial = nullptr;
    QSerialPortInfo *info = nullptr;

    QVector<QSerialPortInfo> _listPorts;
};

#endif // SERIALCLIENT_H
