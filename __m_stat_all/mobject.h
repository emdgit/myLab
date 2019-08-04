#ifndef MOBJECT_H
#define MOBJECT_H

#include <QObject>
#include <QtCore>

class MObject : public QObject
{

    Q_OBJECT

    Q_PROPERTY( QString randomIcon  READ randomIcon USER true )

public:
    explicit                MObject(QObject *parent = nullptr);
    ~                       MObject();

    Q_INVOKABLE void        qmlAbout();
    Q_INVOKABLE void        qmlQuit();
    Q_INVOKABLE void        playSound();

    const QString           randomIcon() const;

};

#endif // MOBJECT_H
