#include "coreapi.h"

#include "connecter.h"
#include "typestorage.h"
#include "purchasegroup.h"

#include <iostream>

using namespace pg;
using namespace std;

using RE = std::runtime_error;

pg::Worker * CoreAPI::_pg_worker = Connecter::createWorker();

bool CoreAPI::addPurchase(const int & groupId, const int & userId,
                          const int & recordId, const double & summ,
                          const QDate & date) noexcept
{
    // todo
    Q_UNUSED(groupId)
    Q_UNUSED(userId)
    Q_UNUSED(recordId)
    Q_UNUSED(summ)
    Q_UNUSED(date)

    auto func = TypeStorage::func( "add_purchase", "common" );

    if ( !func ) {
        return false;
    }

    auto answer = _pg_worker->execute( *func.value() );

    if ( !answer ) {
        return false;
    }


}

std::vector<IObject *> CoreAPI::loadRootGroups(bool profit)
{
    // todo use profit in db
    Q_UNUSED(profit)

    auto func = TypeStorage::func( "get_root_groups", "common" );

    if ( !func ) {
        throw RE( "function common.get_root_groups hasn't been found" );
    }

    auto answer = _pg_worker->execute( *func.value() );

    if ( !answer ) {
        throw RE( "Error in executing common.get_root_groups" );
    }

    vector<IObject*> out( answer->rows() );

    for ( size_t i(0); i < answer->rows(); ++i ) {
        auto gr = new PurchaseGroup();

        try {
            gr->fromPgAnswer( answer, static_cast<ulong>(i) );
        }
        catch( const exception &ex ) {

            for ( size_t j(0); j < i - 1; ++j ) {
                delete out[j];
            }

            cout << ex.what() << endl;
            throw ex;
        }

        out[i] = gr;
    }

    return out;
}
