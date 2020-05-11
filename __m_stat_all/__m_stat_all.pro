QT          +=  quick
QT          +=  sql

CONFIG      +=  c++17

QMAKE_CXXFLAGS += -std=c++17

DEFINES     +=  QT_DEPRECATED_WARNINGS

INCLUDEPATH +=  $$PWD/DB/
DEPENDPATH  +=  $$PWD/DB/
INCLUDEPATH +=  $$PWD/paint/
DEPENDPATH  +=  $$PWD/paint/

SOURCES     +=        \
            main.cpp   \
            templates.cpp   \
            testmodel.cpp    \
            DB/Set.cpp \
            DB/head.cpp \
            DB/field.cpp \
            DB/answer.cpp \
            DB/worker.cpp  \
            DB/function.cpp \
            DB/connecter.cpp \
            DB/typedfield.cpp \
            DB/typestorage.cpp \
    paint/chart.cpp \
    paint/gline.cpp \
    paint/gpoliline.cpp \
    paint/graphbase.cpp

RESOURCES   +=  qml.qrc \
                sql.qrc

HEADERS     +=      \
            private.h  \
            templates.h   \
            testmodel.h    \
            exception.h     \
            DB/Set.h    \
            DB/field.h   \
            DB/ptrset.h   \
            DB/typestorage.h \
    DB/head.h \
    DB/typedfield.h \
    DB/config.h \
    DB/connecter.h \
    DB/answer.h \
    DB/function.h \
    DB/worker.h \
    paint/chart.h \
    paint/gline.h \
    paint/gpoliline.h \
    paint/graphbase.h

