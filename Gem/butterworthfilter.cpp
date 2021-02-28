#include "butterworthfilter.h"

ButterworthFilter::ButterworthFilter() {}
ButterworthFilter::~ButterworthFilter() {}

double ButterworthFilter::filter(double dataValue) {
    _xv[0] = _xv[1]; _xv[1] = _xv[2]; _xv[2] = _xv[3];
    _xv[3] = dataValue / GAIN;
    _yv[0] = _yv[1]; _yv[1] = _yv[2]; _yv[2] = _yv[3];
    _yv[3] = (_xv[3] - _xv[0]) + 3 * (_xv[1] - _xv[2])
               + (0.9825608772 * _yv[0]) + (-2.9649686898 * _yv[1])
               + (2.9824071379 * _yv[2]);
    _channelData = _yv[3];
    return _channelData;
}
