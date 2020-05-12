#include "answer.h"
#include "typestorage.h"

pg::Answer::~Answer() {}

bool pg::Answer::isValid() const noexcept
{
    if ( _answerMap.empty() )
        return false;

    auto s = _answerMap[0].values.size();

    if ( !s )
        return false;

    for ( size_t i(1); i < _answerMap.size(); ++i )
        if ( _answerMap[i].values.size() != s )
            return false;

    return true;
}

bool pg::Answer::insertValue(const QString & field, const QVariant & val) noexcept
{
    auto fieldOpt = TypeStorage::field( field );

    if ( !fieldOpt )
        return false;

    auto t = QMetaType::type( val.typeName() );

    auto it = std::find_if( _answerMap.begin(), _answerMap.end(),
                            [&]( const AnswerItem &item ) {
        auto ptr = item.tField.field.lock();
        return ptr.get()->name == field &&
                item.tField.type == t;
    });

    if ( it == _answerMap.end() )
    {
        TypedField fld( fieldOpt.value(), static_cast<QMetaType::Type>( t ) );
        _answerMap.emplace_back( fld );
        _answerMap.back().values.push_back( val );
    }
    else
        (*it).values.push_back( val );

    return true;
}

pg::Answer::AnswerValue pg::Answer::field(const size_t & row, const size_t & column) const
{
    try {
        auto &item = _answerMap[column];
        return {item.tField, item.values[row]};
    }
    catch(...) {
        throw;
    }
}

pg::Answer::Answer(const int & len) noexcept
{
    _answerMap.reserve( static_cast<size_t>( len ) );
}
