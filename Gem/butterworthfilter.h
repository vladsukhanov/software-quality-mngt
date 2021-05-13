#ifndef BUTTERWORTHFILTER_H
#define BUTTERWORTHFILTER_H

// Настройки фильтра
#define NZEROS 3
#define NPOLES 3
#define GAIN   1.008835291e+00

class ButterworthFilter{
public:
    ButterworthFilter();
    ~ButterworthFilter();

    // Функция фильтрации полученных данных
    double filter(double);

private:
    // Переменные для функции filter()
    double _xv[NZEROS+1] = {}, _yv[NPOLES+1] = {};
    double _channelData = 0;
};

#endif // BUTTERWORTHFILTER_H
