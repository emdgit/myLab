QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    chart.cpp \
    graphbase.cpp \
    gline.cpp \
    gpoliline.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

QML_DESIGNER_IMPORT_PATH =

HEADERS += \
    chart.h \
    graphbase.h \
    gline.h \
    gpoliline.h \
    private.h

