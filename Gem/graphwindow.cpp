#include "graphwindow.h"
#include "ui_graphwindow.h"

#include "butterworthfilter.h"

// Классы фильтров
ButterworthFilter filter1;
ButterworthFilter filter2;
ButterworthFilter filter3;
ButterworthFilter filter4;

GraphWindow::GraphWindow(QWidget *parent) : QDialog(parent), ui(new Ui::GraphWindow) {
    ui->setupUi(this);

    qDebug() << "default GraphWindow();";
}

GraphWindow::GraphWindow(int8_t graphNumber) : ui(new Ui::GraphWindow){
    ui->setupUi(this);

    qDebug() << "GraphWindow(graphNumber);";

    _graphNumber = graphNumber;

    switch (_graphNumber) {
    // Функция давления от времени
    case 1:
        setGeometry(100, 130, 1200, 800);
        break;
    // Осцилляции от времени
    case 2:
        setGeometry(100, 130, 1200, 800);
        break;
    // Осцилляции от давления
    case 3:
        setGeometry(100, 130, 1200, 800);
        break;
    default:
        break;
    }

    setupPlot(ui->customPlot);
    setWindowTitle("Patient: " + _patientSurname);
    ui->customPlot->replot();
}

GraphWindow::~GraphWindow() {
    delete ui;

    qDebug() << "~GraphWindow();";
}

void GraphWindow::setupPlot(QCustomPlot *customPlot) {
    // Начальные настройки
    _patientSurname = "Sukhanov";
    _plotViewPos = 2;

    ui->labelGraphView->setText("SLIDING");

    // Переменные для сброса таймера
//    myTimer.restart();
//    lastPointKey = 0;

    // include this section to fully disable antialiasing for higher performance:
    /*
    customPlot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->legend->setFont(font);
    */
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    ui->customPlot->graph(0)->setName("Right hand");
    ui->customPlot->graph(1)->setName("Left hand");

    ui->customPlot->legend->setVisible(true);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);

    switch (_graphNumber) {
    // Функция давления от времени
    case 1:
        timeTicker->setTimeFormat("%h:%m:%s");
        customPlot->xAxis->setTicker(timeTicker);
        customPlot->xAxis->setLabel("Time [sec]");
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->yAxis->setRange(-1, 30);
        customPlot->yAxis->setLabel("Pressure [mm Hg]");
        break;
    // Осцилляции от времени
    case 2:
        timeTicker->setTimeFormat("%h:%m:%s");
        customPlot->xAxis->setTicker(timeTicker);
        customPlot->xAxis->setLabel("Time [sec]");
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->yAxis->setRange(-20, 20);
        customPlot->yAxis->setLabel("Oscillation [mm Hg]");
        break;
    // Осцилляции от давления
    case 3:
        customPlot->xAxis->setLabel("Pressure [mm Hg]");
        customPlot->axisRect()->setupFullAxesBox();
        customPlot->yAxis->setRange(-20, 20);
        customPlot->yAxis->setLabel("Oscillation [mm Hg]");
        break;
    default:
        break;
    }

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    connect(&_dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    // Одно и то же
//    connect(&_dataTimer, &QTimer::timeout, this, &GraphWindow::realtimeDataSlot);

    _dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void GraphWindow::realtimeDataSlot() {
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double timeKey = time.elapsed() / 1000.0;

    // add data to lines:
//    ui->customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
//    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));

    // Проверка типа графика
    switch (_graphNumber) {
    // Функция давления от времени
    case 1:
//        ui->customPlot->graph(0)->addData(timeKey, local_ads1256Data1);
//        ui->customPlot->graph(1)->addData(timeKey, local_ads1256Data2);

//        ui->graphValueY1->setText(QString::number(local_ads1256Data1));
//        ui->graphValueY2->setText(QString::number(local_ads1256Data2));
//        ui->graphValueY3->setText(QString::number(local_ads1256Data3));
//        ui->graphValueY4->setText(QString::number(local_ads1256Data4));


        ui->customPlot->graph(0)->addData(timeKey, local_ads1292rData1);
        ui->customPlot->graph(1)->addData(timeKey, local_ads1292rData2);

        ui->graphValueY1->setText(QString::number(local_ads1292rData1));
        ui->graphValueY2->setText(QString::number(local_ads1292rData2));
        ui->graphValueY3->setText(QString::number(local_ads1292rData3));
        ui->graphValueY4->setText(QString::number(local_ads1292rData4));

        break;
    // Осцилляции от времени
    case 2:
        ui->customPlot->graph(0)->addData(timeKey, local_filteredAds1256Data1);
        ui->customPlot->graph(1)->addData(timeKey, local_filteredAds1256Data2);

        ui->graphValueY1->setText(QString::number(local_filteredAds1256Data1));
        ui->graphValueY2->setText(QString::number(local_filteredAds1256Data2));
        ui->graphValueY3->setText(QString::number(local_filteredAds1256Data3));
        ui->graphValueY4->setText(QString::number(local_filteredAds1256Data4));

        break;
    // Осцилляции от давления
    case 3:
        ui->customPlot->graph(0)->addData(local_ads1256Data1, local_filteredAds1256Data1);
        ui->customPlot->graph(1)->addData(local_ads1256Data2, local_filteredAds1256Data2);

        ui->graphValueX1->setText(QString::number(local_ads1256Data1));
        ui->graphValueX2->setText(QString::number(local_ads1256Data2));
        ui->graphValueX3->setText(QString::number(local_ads1256Data3));
        ui->graphValueX4->setText(QString::number(local_ads1256Data4));

        ui->graphValueY1->setText(QString::number(local_filteredAds1256Data1));
        ui->graphValueY2->setText(QString::number(local_filteredAds1256Data2));
        ui->graphValueY3->setText(QString::number(local_filteredAds1256Data3));
        ui->graphValueY4->setText(QString::number(local_filteredAds1256Data4));

        break;
    default:
        break;
    }

    // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis(true);
    ui->customPlot->graph(1)->rescaleValueAxis(true);

    // Проверка вида графика
    if (_plotViewPos == 1) {
        ui->customPlot->graph(0)->rescaleKeyAxis(true);
        ui->customPlot->graph(1)->rescaleKeyAxis(true);
    }

    if (_plotViewPos == 2) ui->customPlot->xAxis->setRange(timeKey, 3, Qt::AlignRight);

    ui->customPlot->replot();

    // Flush the graph
//    ui->customPlot->graph(0)->data()->clear();
//    ui->customPlot->graph(1)->data()->clear();

    // rescale key (horizontal) axis to fit the current data:
//    ui->customPlot->graph(0)->rescaleKeyAxis(true);
    //    ui->customPlot->graph(1)->rescaleKeyAxis(true);
}

void GraphWindow::filterAds1256Data() {
    local_filteredAds1256Data1 = filter1.filter(local_ads1256Data1);
    local_filteredAds1256Data2 = filter2.filter(local_ads1256Data2);
    local_filteredAds1256Data3 = filter3.filter(local_ads1256Data3);
    local_filteredAds1256Data4 = filter4.filter(local_ads1256Data4);
}

void GraphWindow::getDataAvailable(QByteArray data) {
    static int channelNumber = 0;

//    qDebug() << data;

    channelNumber = data.left(1).toInt(); // номер канала
    data.remove(0, 1);  // удаление номера канала из пакета данных
//    data.chop(2); // удаление \r\n

    switch (channelNumber) {
        // ADS1256
      case 1:
        local_ads1256Data1 = data.toDouble();
        filterAds1256Data();
        break;
      case 2:
        local_ads1256Data2 = data.toDouble();
        filterAds1256Data();
        break;
      case 3:
        local_ads1256Data3 = data.toDouble();
        filterAds1256Data();
        break;
      case 4:
        local_ads1256Data4 = data.toDouble();
        filterAds1256Data();
        break;
        // ADS1292R
      case 5:
        local_ads1292rData1 = data.toDouble();
        local_ads1292rData1 /= 1e6;

        if (local_ads1292rData1 < 0.1) local_ads1292rData1 *= 1000;

//        qDebug() << local_ads1292rData1;
        break;
      case 6:
        local_ads1292rData2 = data.toDouble();
        local_ads1292rData2 /= 1e6;

        if (local_ads1292rData2 < 0.1) local_ads1292rData2 *= 1000;

//        qDebug() << local_ads1292rData2;
        break;
      case 7:
        local_ads1292rData3 = data.toDouble();
        break;
      case 8:
        local_ads1292rData4 = data.toDouble();
        break;
      default:
        break;
    }
}

void GraphWindow::on_btnCancel_clicked() {
    qDebug() << "btnCancel was clicked";

    this->done(1);
//    this->reject();
//    this->accept();
}

void GraphWindow::on_btnFull_clicked() {
    qDebug() << "btnFull was clicked";
    _plotViewPos = 1;

//    _parentWindow->serialClient->writeData("Full");

    ui->customPlot->xAxis->setRange(-1, 40);

    // Запрет масштабирования и перетаскивания
    ui->customPlot->setInteraction(QCP::iRangeZoom, false);
    ui->customPlot->setInteraction(QCP::iRangeDrag, false);

    ui->labelGraphView->setText("FULL");
}

void GraphWindow::on_btnSliding_clicked() {
    qDebug() << "btnSliding was clicked";
    _plotViewPos = 2;

//    _parentWindow->serialClient->writeData("Sliding");

    // Запрет масштабирования и перетаскивания
    ui->customPlot->setInteraction(QCP::iRangeZoom, false);
    ui->customPlot->setInteraction(QCP::iRangeDrag, false);

    ui->labelGraphView->setText("SLIDING");
}

void GraphWindow::on_btnInteraction_clicked() {
    qDebug() << "btnInteraction was clicked";
    _plotViewPos = 3;

//    _parentWindow->serialClient->writeData("Interaction");

    // Разрешение масштабирования и перетаскивания
    ui->customPlot->setInteraction(QCP::iRangeZoom, true);
    ui->customPlot->setInteraction(QCP::iRangeDrag, true);

    ui->labelGraphView->setText("INTERACTION");
}
