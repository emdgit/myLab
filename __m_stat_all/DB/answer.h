#ifndef PGANSWER_H
#define PGANSWER_H

#include <QObject>

namespace pg {
    class Answer;
}

class pg::Answer : public QObject
{

    Q_OBJECT

public:

    Answer( QObject * parent = nullptr );
    virtual ~Answer();

};

#endif // PGANSWER_H
