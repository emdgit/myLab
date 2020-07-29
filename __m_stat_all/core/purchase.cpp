#include "templates.h"
#include "purchase.h"
#include "answer.h"

using namespace std;

static constexpr int _fieldCount = 3;

Purchase::Purchase()
{

}

void Purchase::fromPgAnswer(pg::Answer * answer, unsigned long i)
{
    const auto size = answer->columns();

    if ( size != _fieldCount ) {
        throw runtime_error( "Purchase::fromPgAnswer: expected 3 fields" );
    }

    QString name;

    fromVariant(name, answer->field(i, 0).value);
    fromVariant(_summ, answer->field(i, 1).value);
    fromVariant(_last_date, answer->field(i, 2).value);

    _name = name.toStdString();
}

const QDate &Purchase::lastDate() const
{
    return _last_date;
}

const string &Purchase::name() const
{
    return _name;
}

double Purchase::summ() const noexcept
{
    return _summ;
}
