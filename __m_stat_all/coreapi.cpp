#include "coreapi.h"

#include "connecter.h"
#include "typestorage.h"
#include "modelmanager.h"
#include "purchasegroup.h"
#include "purchaserecord.h"

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

void CoreAPI::loadGroups(bool profit)
{
    auto func = profit ? TypeStorage::func( "get_root_groups_profit", "common" )
                       : TypeStorage::func( "get_root_groups_spend", "common" );

    if ( !func ) {
        throw RE( "function common.get_root_groups hasn't been found" );
    }

    auto answer = unique_ptr<Answer>( _pg_worker->execute( *func.value() ) );

    if ( !answer ) {
        // todo Пересмотреть логику ответа execute. Что если ответ пустой?
        return;
    }

    auto st = profit ? &_p_g_storage_profit
                     : &_p_g_storage_spend;

    if ( !st ) {
        throw RE("Target storage wasn't set");
    }

    packGroups( answer.get(), *st, {} );
}

void CoreAPI::loadRecords(bool profit)
{
    using record_vec = vector<PurchaseRecord*>;
    pair<QString, record_vec*> work_pair;

    if ( profit ) {
        work_pair.first = "get_all_records_profit";
        work_pair.second = &_records_profit;
    } else {
        work_pair.first = "get_all_records_spend";
        work_pair.second = &_records_spend;
    }

    auto func = TypeStorage::func( work_pair.first, "common" );

    if ( !func ) {
        throw RE( "function common." +
                  work_pair.first.toStdString() +
                  "get_root_groups hasn't been found" );
    }

    auto answer = unique_ptr<Answer>( _pg_worker->execute( *func.value() ) );

    if ( !answer ) {
        // todo Пересмотреть логику ответа execute. Что если ответ пустой?
        return;
    }

    auto vec = work_pair.second;

    if ( !vec->empty() ) {
        for ( auto &p : *vec ) {
            delete p;
        }

        vec->clear();
    }

    for ( size_t i(0); i < answer->rows(); ++i ) {
        auto record = new PurchaseRecord;

        try {
            record->fromPgAnswer( answer.get(), i );
        } catch ( const exception &ex ) {
            cout << ex.what() << endl;
            delete record;
            continue;
        }

        vec->push_back(record);
    }
}

void CoreAPI::switchHintModel(bool profit)
{
    if ( !_modelManager ) {
        throw runtime_error("Attempt to switch model in NULL model manager");
    }

    auto records = profit ? &_records_profit
                          : &_records_spend;

    _modelManager->hintModel()->setRecords( records );
}

void CoreAPI::addRecord(int groupId, const QString &recordName, bool profit)
{
    auto func = TypeStorage::func( "add_record", "common" );

    if ( !func ) {
        throw RE( "Function 'common.add_record' hasn't been found" );
    }

    (*func)->bindValue( "group_id", groupId );
    (*func)->bindValue( "name", recordName );

    auto answer = unique_ptr<Answer>( _pg_worker->execute( **func ) );

    if (!answer) {
        throw RE( "Cannot execute function common.add_record" );
    }

    auto res = answer->tryConvert<int>();

    if ( !res ) {
        throw RE( "Unexpected function response: CoreAPI::addRecord(): "
                  "'common.add_record'" );
    }

    if ( *res < 0 ) {
        return;
    }

    loadRecords( profit );
}

void CoreAPI::addPurchaseGroup(const QString &name, int parentGroupId, bool profit)
{
    auto func = TypeStorage::func( "add_group", "common" );

    if ( !func ) {
        throw RE( "Function 'common.add_group' hasn't been found" );
    }

    (*func)->bindValue( "name", name );
    (*func)->bindValue( "profit", profit );

    if ( parentGroupId ) {
        (*func)->bindValue( "parent_id", parentGroupId );
    }

    auto answer = unique_ptr<Answer>( _pg_worker->execute( **func ) );

    if (!answer) {
        throw RE( "Cannot execute function common.add_group" );
    }

    auto res = answer->tryConvert<int>();

    if ( !res ) {
        throw RE( "Unexpected function response: CoreAPI::addPurchaseGroup(): "
                  "'common.add_group'" );
    }

    if ( *res < 0 ) {
        throw RE( "Error in psql function: 'common.add_group'" );
    }

    auto st = profit ? _p_g_storage_profit
                     : _p_g_storage_spend;

    st->clear();

    loadGroups( profit );

    if ( profit ) {
        _modelManager->profitModel()->reloadData();
    } else {
        _modelManager->spendModel()->reloadData();
    }
}

void CoreAPI::setModelManager(ModelManager *mm)
{
    _modelManager = mm;
}

void CoreAPI::setProfitGroupSt(PGStorage * st) noexcept
{
    _p_g_storage_profit = st;

    st->setInsertHandler( [&](PNodeIndex index) -> void {
        try {
            loadGroupsByParent( index, _p_g_storage_profit );
        } catch ( const exception &ex ) {
            cout << ex.what() << endl;
        }
    } );
}

void CoreAPI::setSpendGroupSt(PGStorage * st) noexcept
{
    _p_g_storage_spend = st;

    st->setInsertHandler( [&](PNodeIndex index) -> void {
        try {
            loadGroupsByParent( index, _p_g_storage_spend );
        } catch ( const exception &ex ) {
            cout << ex.what() << endl;
        }
    } );
}

void CoreAPI::loadGroupsByParent(PNodeIndex parent, PGStorage *st)
{
    auto func = TypeStorage::func( "get_groups", "common" );

    if ( !func ) {
        throw RE( "function common.get_groups hasn't been found" );
    }

    auto id = st->node(parent)->_data->id();
    func.value()->bindValue( "group_parent_id", id );

    auto answer = unique_ptr<Answer>( _pg_worker->execute( *func.value() ) );

    if ( !answer ) {
        // todo Пересмотреть логику ответа execute. Что если ответ пустой?
        return;
    }

    packGroups( answer.get(), st, parent );
}

void CoreAPI::packGroups(Answer *answer, PGStorage *st, PNodeIndex parent)
{
    for ( size_t i(0); i < answer->rows(); ++i ) {
        auto gr = new PurchaseGroup();

        try {
            gr->fromPgAnswer( answer, static_cast<ulong>(i) );
        }
        catch( const exception &ex ) {

            // todo clear st

            cout << ex.what() << endl;
            throw;
        }

        st->insert( parent, gr );
    }
}

