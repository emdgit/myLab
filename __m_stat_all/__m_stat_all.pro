QT          +=  quick
QT          +=  sql

CONFIG      +=  c++11

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
            DB/data.cpp   \
            DB/head.cpp    \
            DB/field.cpp    \
            DB/entity.cpp    \
            DB/pgfunction.cpp \
            DB/typestorage.cpp

RESOURCES   +=  qml.qrc

HEADERS     +=      \
            chart.h  \
            gline.h   \
            private.h  \
            gpoliline.h \
            graphbase.h  \
            templates.h   \
            DB/data.h   \
            DB/head.h    \
            DB/field.h    \
            DB/entity.h    \
            DB/pgfunction.h \
            DB/typestorage.h

