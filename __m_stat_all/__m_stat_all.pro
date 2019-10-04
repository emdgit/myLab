QT          +=  quick
QT          +=  sql

CONFIG      +=  c++17

DEFINES     +=  QT_DEPRECATED_WARNINGS

INCLUDEPATH +=  $$PWD/DB/
DEPENDPATH  +=  $$PWD/DB/

SOURCES     +=        \
            main.cpp   \
            chart.cpp   \
            gline.cpp    \
            graphbase.cpp \
            gpoliline.cpp  \
            templates.cpp   \
            DB/head.cpp    \
            DB/field.cpp    \
            DB/pgfunction.cpp \
            DB/typestorage.cpp \
    testmodel.cpp

RESOURCES   +=  qml.qrc

HEADERS     +=      \
            chart.h  \
            gline.h   \
            private.h  \
            gpoliline.h \
            graphbase.h  \
            templates.h   \
            DB/head.h    \
            DB/field.h    \
            DB/pgfunction.h \
            DB/typestorage.h \
    testmodel.h \
    DB/ptrset.h

