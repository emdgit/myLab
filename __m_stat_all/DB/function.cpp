#include "function.h"
#include "templates.h"
#include "exception.h"

class PgFunctionPrivate
{
public:

    PgFunctionPrivate( pg::Function * const &f ) : _func(f) {}

    /*!
     * \param   Prepares the given query and binds values.
     */
    void prepare( QSqlQuery &query ) const noexcept
    {
        QString prepareScript = "SELECT ";

        prepareOutArgs( prepareScript );

        prepareScript += "FROM ";
        prepareScript += _func->_schema + ".";
        prepareScript += _func->_name;

        prepareInArgs( prepareScript );

        query.prepare( prepareScript );

        bindValues( query );
    }

    /*!
     * \brief   Sorts funtion's in arguments in correct order.
     *          Correct order is:
     *          1 - No default args with valid values first
     *          2 - Default args with valid values second
     *          3 - Default args with invalid values last
     */
    void sortInArgs() const noexcept
    {
        typedef FuncInArgument FIA;

        auto pred = [&]( const FIA &l, const FIA &r ) {
            if ( l.isDefault == r.isDefault )
                return l.isDefault ? ( l.value.isValid() > r.value.isValid() )
                                   : true;
            else
                return l.isDefault < r.isDefault;
        };

        std::sort( _func->_inArgs.begin(), _func->_inArgs.end(), pred );
    }


protected:

    void prepareOutArgs( QString &outQuery ) const noexcept
    {
        if ( _func->_outArgs.empty() )
        {
            outQuery.append( _func->name() );
            return;
        }

        for ( const auto &a : _func->_outArgs )
            outQuery += a.field.get()->name + ", ";

        outQuery.remove( outQuery.size() - 2, 1 );
    }

    void prepareInArgs( QString &outQuery ) const noexcept
    {
        outQuery += "( ";

        for ( const auto &a : _func->_inArgs )
        {
            if ( !a.value.isValid() )
                break;

            outQuery += a.field.get()->name;
            outQuery += " := ?, ";
        }

        if ( QRegExp( ",\\s$" ).indexIn( outQuery ) != -1 )
            outQuery.remove( outQuery.size() - 2, 1 );

        outQuery += " );";
    }

    void bindValues( QSqlQuery &query ) const noexcept
    {
        int i = 0;

        for ( const auto &a : _func->_inArgs )
        {
            if ( !a.value.isValid() )
                return;

            query.bindValue( i, a.value );
            ++i;
        }
    }


private:

    pg::Function *    _func;

};

pg::Function::Function()
{
    d_ptr = new PgFunctionPrivate( this );
}

pg::Function::Function(const QString &name) : _name(name)
{
    d_ptr = new PgFunctionPrivate( this );
}

pg::Function::Function(const Function & other)
{
    _name = other._name;
    _schema = other._schema;

    _outArgs = other._outArgs;
    _inArgs = other._inArgs;

    d_ptr = new PgFunctionPrivate( this );
}

void pg::Function::addOut(const FuncArgument &&in) noexcept
{
    _outArgs.push_back( std::move( in ) );
}

void pg::Function::addIn(const FuncInArgument &&out) noexcept
{
    Q_D ( PgFunction );

    _inArgs.push_back( std::move( out ) );

    d->sortInArgs();
}

bool pg::Function::isComplete() const noexcept
{
    if ( _name.isEmpty() || _schema.isEmpty() )
        return false;

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

bool pg::Function::prepare(QSqlQuery &query) const noexcept
{
    if ( !isComplete() )
        return false;

    Q_D ( const PgFunction );

    d->prepare( query );

    return true;
}

bool pg::Function::bindValue(const QString &name, const QVariant &&val) noexcept
{
    auto it = std::find_if( _inArgs.begin(), _inArgs.end(), [&] ( const auto &arg ) {
        return arg.field.get()->name == name;
    } );

    if ( it == _inArgs.end() )
        return false;

    (*it).value = std::move( val );

    return true;
}

bool pg::Function::operator<(const pg::Function &other) noexcept
{
    return _schema == other._schema ? ( _name < other._name )
                                    : ( _schema < other._schema );
}

bool pg::Function::operator==(const pg::Function &other) noexcept
{
    return _schema == other._schema ? ( _name == other._name )
                                    : false;
}

