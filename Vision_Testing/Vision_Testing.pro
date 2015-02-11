TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    morphology.h \
    morphology.h \
    objRecognition.h \
    geometry.h

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

LIBS += -lopencv_nonfree
