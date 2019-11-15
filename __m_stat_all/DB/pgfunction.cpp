#include "pgfunction.h"

PgFunction::PgFunction()
{

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

