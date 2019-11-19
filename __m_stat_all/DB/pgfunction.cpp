#include "pgfunction.h"
#include "templates.h"
#include "exception.h"

class PgFunctionPrivate
{
public:

    PgFunctionPrivate( PgFunction * const &f ) : _func(f) {}

    void prepare( QSqlQuery &query ) const
    {
        QString prepareScript = "SELECT ";
    }

    void sortInArgs() const noexcept
    {
        typedef FuncInArgument FIA;

        auto pred = [&]( const FIA &l, const FIA &r ) {
            if ( l.isDefault != r.isDefault )
                return l.isDefault ? false : true;
        };
    }

private:

    PgFunction *    _func;

};

PgFunction::PgFunction()
{
    d_ptr = new PgFunctionPrivate( this );
}

PgFunction::PgFunction(const QString &name) : _name(name) {}

void PgFunction::addOut(const FuncArgument &&in) noexcept
{
    _outArgs.push_back( std::move( in ) );
}

void PgFunction::addIn(const FuncInArgument &&out) noexcept
{
    _inArgs.push_back( std::move( out ) );
}

bool PgFunction::isComplete() const noexcept
{
    auto isCompleteArg = [&]( const auto &arg ) -> bool {

        if constexpr ( !std::is_base_of_v< decltype ( arg ), FuncArgument > )
                return false;

        if ( !arg.field.get() )
            return  false;

        if constexpr ( std::is_same_v< FuncInArgument, decltype ( arg ) > )
        {
            auto _arg = static_cast< FuncInArgument >( arg );

            if ( _arg.isDefault )
                return true;

            return _arg.value.isValid();
        }

        return false;
    };

    return std::is_partitioned( _inArgs.begin(), _inArgs.end(), isCompleteArg ) &&
           std::is_partitioned( _outArgs.begin(), _outArgs.end(), isCompleteArg );
}

bool PgFunction::prepare(QSqlQuery &query) const noexcept
{
    if ( !isComplete() )
        return false;

    Q_D ( const PgFunction );

    using namespace Owl::Exception;

    try {
        d->prepare( query );
    }
    catch ( Exception< FP > &ex ) {
        qDebug() << ex.what();
        return false;
    }

    return true;
}

bool PgFunction::bindValue(const QString &name, const QVariant &&val) noexcept
{
    auto it = std::find_if( _inArgs.begin(), _inArgs.end(), [&] ( const auto &arg ) {
        return arg.field.get()->name == name;
    } );

    if ( it == _inArgs.end() )
        return false;

    (*it).value = std::move( val );

    return true;
}

bool operator<(const PgFunction &l, const PgFunction &r) noexcept
{
    return l._schema == r._schema ? ( l._name < r._name )
                                  : ( l._schema < r._schema );
}

bool operator==(const PgFunction &l, const PgFunction &r) noexcept
{
    return l._schema == r._schema ? ( l._name == r._name )
                                  : false;
}

