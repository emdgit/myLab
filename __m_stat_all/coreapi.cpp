#include "coreapi.h"

#include "connecter.h"
#include "typestorage.h"
#include "purchasegroup.h"

#include <iostream>

using namespace pg;
using namespace std;

using RE = std::runtime_error;

pg::Worker * CoreAPI::_pg_worker = Connecter::createWorker();

PGStorage * CoreAPI::_p_g_storage_spend = nullptr;
PGStorage * CoreAPI::_p_g_storage_profit = nullptr;

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

void CoreAPI::loadRootGroups(bool profit)
{
    auto func = profit ? TypeStorage::func( "get_root_groups_profit", "common" )
                       : TypeStorage::func( "get_root_groups_spend", "common" );

    if ( !func ) {
        throw RE( "function common.get_root_groups hasn't been found" );
    }

    auto answer = _pg_worker->execute( *func.value() );

    if ( !answer ) {
        // todo Пересмотреть логику ответа execute. Что если ответ пустой?
        return;
    }

    auto st = profit ? &_p_g_storage_profit
                     : &_p_g_storage_spend;

    if ( !st ) {
        throw RE("Target storage wasn't set");
    }

    for ( size_t i(0); i < answer->rows(); ++i ) {
        auto gr = new PurchaseGroup();

        try {
            gr->fromPgAnswer( answer, static_cast<ulong>(i) );
        }
        catch( const exception &ex ) {

            // todo clear st

            cout << ex.what() << endl;
            throw ex;
        }

        (*st)->insert( {}, gr );
    }
}

void CoreAPI::setProfitGroupSt(PGStorage * st) noexcept
{
    _p_g_storage_profit = st;
}

void CoreAPI::setSpendGroupSt(PGStorage * st) noexcept
{
    _p_g_storage_spend = st;
}
