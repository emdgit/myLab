#ifndef PGANSWER_H
#define PGANSWER_H

#include <QObject>

class PGAnswer : public QObject
{

    Q_OBJECT

public:

    PGAnswer( QObject * parent = nullptr );
    virtual ~PGAnswer();

};

#endif // PGANSWER_H
