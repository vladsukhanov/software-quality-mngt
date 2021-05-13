QT += testlib
#QT += gui core bluetooth serialport widgets printsupport
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_checktest.cpp

INCLUDEPATH += ../../Gem

#SOURCES += ../../Gem/btclient.cpp
SOURCES += ../../Gem/butterworthfilter.cpp
#SOURCES += ../../Gem/deviceinfo.cpp
#SOURCES += ../../Gem/graphwindow.cpp
#SOURCES += ../../Gem/mainwindow.cpp
#SOURCES += ../../Gem/qcustomplot.cpp
#SOURCES += ../../Gem/serialclient.cpp

#FORMS += ../../Gem/btclient.ui
#FORMS += ../../Gem/graphwindow.ui
#FORMS += ../../Gem/mainwindow.ui
#FORMS += ../../Gem/serialclient.ui
