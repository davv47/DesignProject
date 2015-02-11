TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS +=


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/release/ -lserial
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/debug/ -lserial
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/local/lib/ -lserial

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/include
