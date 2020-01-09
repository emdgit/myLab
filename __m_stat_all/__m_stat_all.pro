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
            testmodel.cpp    \
            DB/Set.cpp      \
            DB/field.cpp     \
            DB/typestorage.cpp \
    DB/head.cpp \
    DB/typedfield.cpp \
    DB/connecter.cpp \
    DB/answer.cpp \
    DB/function.cpp \
    DB/worker.cpp

RESOURCES   +=  qml.qrc \
                sql.qrc

HEADERS     +=      \
            chart.h  \
            gline.h   \
            private.h  \
            gpoliline.h \
            graphbase.h  \
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
    DB/worker.h

