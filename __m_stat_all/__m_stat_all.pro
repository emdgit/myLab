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
            DB/pgfunction.cpp \
            DB/typestorage.cpp \
            DB/dbconnecter.cpp \
    DB/pgworker.cpp \
    DB/pganswer.cpp

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
            DB/dbconfig.h  \
            DB/pgfunction.h \
            DB/typestorage.h \
            DB/dbconnecter.h \
    DB/pgworker.h \
    DB/pganswer.h

