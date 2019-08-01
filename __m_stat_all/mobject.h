#ifndef MOBJECT_H
#define MOBJECT_H

#include <QObject>

class MObject : public QObject
{

    Q_OBJECT

public:
    explicit                MObject(QObject *parent = nullptr);
    ~                       MObject();

    Q_INVOKABLE void        qmlAbout();
    Q_INVOKABLE void        qmlQuit();


};

#endif // MOBJECT_H
