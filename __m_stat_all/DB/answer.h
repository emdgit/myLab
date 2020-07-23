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

    inline size_t columns() const noexcept { return _answerMap.size(); }
    /*!
     * \brief   Количество записей в ответе
     * \throw   Исключение, если объект не валиден
     */
    size_t      rows() const;

    AnswerValue field( const size_t &row, const size_t &column ) const;

    /*!
     * Если ответ это 1 кортеж с одним значением, то попытаться привести его
     * к переданному типу.
     * \tparam T Тип, к которому требуется привести полученый ответ.
     * \return std::optional<T> - получилось или нет.
     */
    template < class T >
    bool tryConvert(T &target) const noexcept
    {
        if ( _answerMap.size() != 1 ) {
            return false;
        }

        if ( _answerMap[0].values.size() != 1 ) {
            return false;
        }

        try {
            target = qvariant_cast<T>( _answerMap[0].values[0] );
            return true;
        } catch (...) {
            return false;
        }
    }

    Answer &    operator=( const Answer & ) = delete;
    Answer &    operator=( Answer && ) = default;


private:

    std::vector<AnswerItem> _answerMap;

};

#endif // PGANSWER_H
