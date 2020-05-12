#ifndef PGANSWER_H
#define PGANSWER_H

#include "typedfield.h"

namespace pg {
    class Answer;
}

class pg::Answer
{

    struct AnswerItem {
        AnswerItem ( const pg::TypedField &tf ) :
            tField( tf ) {}
        pg::TypedField tField;
        std::vector<QVariant> values;
    };

    struct AnswerValue {
        pg::TypedField tfield;
        QVariant value;
    };

public:

    Answer() = delete;
    Answer( const Answer & ) = delete;

    Answer( Answer && ) = default;

    Answer( const int &len ) noexcept;

    virtual ~Answer();

    bool        isValid() const noexcept;
    bool        insertValue( const QString &field,
                             const QVariant &val ) noexcept;

    inline size_t size() const noexcept { return _answerMap.size(); }

    AnswerValue field( const size_t &row, const size_t &column ) const;

    Answer &    operator=( const Answer & ) = delete;
    Answer &    operator=( Answer && ) = default;


private:

    std::vector<AnswerItem> _answerMap;

};

#endif // PGANSWER_H
