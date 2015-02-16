TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    morphology.h \
    morphology.h \
    objRecognition.h \
    geometry.h \
    geometry.h \
    morphology.h \
    objRecognition.h \
    serialCom.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/release/ -lserial
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../usr/local/lib/debug/ -lserial
else:unix: LIBS += -L$$PWD/../../../../../../../../usr/local/lib/ -lserial

INCLUDEPATH += $$PWD/../../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../../usr/local/include


INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib

LIBS += -lopencv_core

LIBS += -lopencv_imgproc

LIBS += -lopencv_highgui

LIBS += -lopencv_ml

LIBS += -lopencv_video

LIBS += -lopencv_features2d

LIBS += -lopencv_calib3d

LIBS += -lopencv_objdetect

LIBS += -lopencv_contrib

LIBS += -lopencv_legacy

LIBS += -lopencv_flann
