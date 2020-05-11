#include "coreapi.h"

#include "connecter.h"
#include "typestorage.h"

using namespace pg;

pg::Worker * CoreAPI::_pg_worker = Connecter::createWorker();

bool CoreAPI::addPurchase(const int & groupId, const int & userId,
                          const int & recordId, const double & summ,
                          const QDate & date) noexcept
{
    auto func = TypeStorage::func( "add_purchase", "common" );

    if ( !func ) {
        return false;
    }

    auto answer = _pg_worker->execute( *func.value() );

    if ( !answer ) {
        return false;
    }


}
