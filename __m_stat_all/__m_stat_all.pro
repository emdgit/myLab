QT          +=  sql
QT          +=  quick
QT          +=  charts
QT          +=  widgets

CONFIG      +=  c++17

DEFINES     +=  QT_DEPRECATED_WARNINGS

INCLUDEPATH +=  $$PWD/DB/
DEPENDPATH  +=  $$PWD/DB/
INCLUDEPATH +=  $$PWD/paint/
DEPENDPATH  +=  $$PWD/paint/
INCLUDEPATH +=  $$PWD/core/
DEPENDPATH  +=  $$PWD/core/

SOURCES     +=           \
            core/chartmetaobject.cpp \
            core/recordbygroupmodel.cpp \
            main.cpp      \
            coreapi.cpp    \
            templates.cpp   \
            modelmanager.cpp \
            DB/Set.cpp \
            DB/head.cpp \
            DB/field.cpp \
            DB/worker.cpp \
            DB/answer.cpp  \
            DB/function.cpp \
            DB/connecter.cpp \
            DB/typedfield.cpp \
            DB/typestorage.cpp \
            paint/chart.cpp   \
            paint/gline.cpp    \
            paint/gpoliline.cpp \
            paint/graphbase.cpp  \
            core/pnode.cpp  \
            core/period.cpp  \
            core/iobject.cpp  \
            core/storage.cpp   \
            core/purchase.cpp   \
            core/hintmodel.cpp   \
            core/chartmanager.cpp \
            core/periodmodel.cpp   \
            core/signalmanager.cpp  \
            core/purchasemodel.cpp   \
            core/purchasegroup.cpp    \
            core/purchaserecord.cpp    \
            core/chartdatastorage.cpp   \
            core/purchasegroupmodel.cpp  \
            core/hierarchicalstorage.cpp  \
            core/purchasemodeldaily.cpp

RESOURCES   +=  qml.qrc \
                sql.qrc  \
                conf.qrc

HEADERS     +=        \
            Error.h \
            core/chartmetaobject.h \
            core/recordbygroupmodel.h \
            private.h  \
            coreapi.h   \
            templates.h  \
            exception.h   \
            modelmanager.h \
            DB/Set.h \
            DB/head.h \
            DB/field.h \
            DB/ptrset.h \
            DB/config.h  \
            DB/worker.h   \
            DB/answer.h    \
            DB/function.h   \
            DB/connecter.h   \
            DB/typedfield.h   \
            DB/typestorage.h   \
            paint/chart.h   \
            paint/gline.h    \
            paint/gpoliline.h \
            paint/graphbase.h  \
            core/log.h  \
            core/pnode.h \
            core/period.h \
            core/storage.h \
            core/iobject.h  \
            core/purchase.h  \
            core/singleton.h  \
            core/hintmodel.h   \
            core/periodmodel.h  \
            core/chartmanager.h  \
            core/purchasemodel.h  \
            core/purchasegroup.h   \
            core/signalmanager.h    \
            core/purchaserecord.h    \
            core/chartdatastorage.h   \
            core/storagedefinition.h   \
            core/purchasegroupmodel.h   \
            core/purchasemodeldaily.h    \
            core/hierarchicalstorage.h

