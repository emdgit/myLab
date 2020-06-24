#ifndef LOG_H
#define LOG_H

#define ENABLE_LOG
    #ifndef QDEBUG_H
    #include <QDebug>
    #endif

#define LOG_FUNC_INFO qDebug() << Q_FUNC_INFO;

#endif // LOG_H
