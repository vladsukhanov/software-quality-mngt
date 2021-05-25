#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDialog>
#include <QTimer>

#include "qcustomplot.h"
#include "butterworthfilter.h"

namespace Ui { class GraphWindow; }

class GraphWindow : public QDialog {
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    explicit GraphWindow(int8_t graphNumber);
    ~GraphWindow();

public slots:
    void getDataAvailable(QByteArray data);

private slots:
    // Функция для RealTimePlot
    void realtimeDataSlot();

    double filterAds1256Data(double data);

    void on_btnCancel_clicked();
    void on_btnFull_clicked();
    void on_btnSliding_clicked();
    void on_btnInteraction_clicked();

private:
    Ui::GraphWindow *ui;

    // Фамилия пациента
    QString _patientSurname;

    // Переменная для построения графика
    QTimer _dataTimer;

    // Вид графика - скользящее окно / статический
    int8_t _plotViewPos;

    // Вид графика: 1 - P(t); 2 - deltaP(t); 3 - deltaP(P);
    int8_t _graphNumber;

    ButterworthFilter filter;

    double local_ads1256Data1, local_ads1256Data2, local_ads1256Data3, local_ads1256Data4;
    double local_ads1292rData1, local_ads1292rData2, local_ads1292rData3, local_ads1292rData4;
    double local_filteredAds1256Data1, local_filteredAds1256Data2, local_filteredAds1256Data3, local_filteredAds1256Data4;

    // Инициализация QCustomPlot
    void setupPlot(QCustomPlot *customPlot);
};

#endif // GRAPHWINDOW_H
