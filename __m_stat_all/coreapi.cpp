#include "coreapi.h"

#include "storage.h"
#include "purchase.h"
#include "connecter.h"
#include "hintmodel.h"
#include "typestorage.h"
#include "modelmanager.h"
#include "purchasegroup.h"
#include "purchaserecord.h"
#include "purchasegroupmodel.h"

#include <iostream>
#include <sstream>

#define DB_COMMON(func_name) #func_name, "common"
#define DB_SERVICE(func_name) #func_name, "service"

using namespace pg;
using namespace std;

using RE = std::runtime_error;

pg::Worker * CoreAPI::_pg_worker = Connecter::createWorker();

void CoreAPI::addPurchase(const QString &rec, QString summ,
                          const QString &date_str, int amount)
{
    addTransaction( rec, summ, date_str, amount, false );
}

void CoreAPI::addProfit(const QString &rec, QString summ, const QString &date_str, int amount)
{
    addTransaction( rec, summ, date_str, amount, true );
}

void CoreAPI::loadGroups(bool profit)
{
    const char * func_name = profit ? "get_root_groups_profit"
                                    : "get_root_groups_spend";

    auto func = TypeStorage::func( func_name, "common" );

    if ( !func ) {
        throw RE( "function common." + string(func_name) + " hasn't been found" );
    }

    auto answer = unique_ptr<Answer>( _pg_worker->execute( *func.value() ) );

    if ( !answer ) {
        // todo Пересмотреть логику ответа execute. Что если ответ пустой?
        return;
    }

    auto st = profit ? ST.groupsProfit()
                     : ST.groupsSpend();

    if ( !st ) {
        throw RE("Target storage wasn't set");
    }

    packGroups( answer.get(), st, {} );
}

void CoreAPI::loadRecords(bool profit)
{
    using record_vec = vector<PurchaseRecord*>;
    pair<QString, record_vec*> work_pair;

    if ( profit ) {
        work_pair.first = "get_all_records_profit";
        work_pair.second = ST.recordsProfit();
    } else {
        work_pair.first = "get_all_records_spend";
        work_pair.second = ST.recordsSpend();
    }

    auto func = TypeStorage::func( work_pair.first, "common" );

    if ( !func ) {
        throw RE( "function common." +
                  work_pair.first.toStdString() +
                  " hasn't been found" );
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

    auto records = profit ? ST.recordsProfit()
                          : ST.recordsSpend();

    _modelManager->hintModel()->setRecords( records );
}

void CoreAPI::addRecord(int groupId, const QString &recordName, bool profit)
{
    auto func = TypeStorage::func( DB_COMMON(add_record) );

    if ( !func ) {
        throw RE( "Function 'common.add_record' hasn't been found" );
    }

    (*func)->bindValue( "group_id", groupId );
    (*func)->bindValue( "name", recordName );

    auto answer = unique_ptr<Answer>( _pg_worker->execute( **func ) );

    if (!answer) {
        throw RE( "Cannot execute function common.add_record" );
    }

    int res;

    if ( !answer->tryConvert(res) ) {
        throw RE( "Unexpected function response: CoreAPI::addRecord(): "
                  "'common.add_record'" );
    }

    if ( res < 0 ) {
        return;
    }

    loadRecords( profit );
}

void CoreAPI::addPurchaseGroup(const QString &name, int parentGroupId, bool profit)
{
    auto func = TypeStorage::func( DB_COMMON(add_group) );

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

    int res;

    if ( !answer->tryConvert(res) ) {
        throw RE( "Unexpected function response: CoreAPI::addPurchaseGroup(): "
                  "'common.add_group'" );
    }

    if ( res < 0 ) {
        throw RE( "Error in psql function: 'common.add_group'" );
    }

    auto st = profit ? ST.groupsProfit()
                     : ST.groupsSpend();

    st->clear();

    loadGroups( profit );

    if ( profit ) {
        _modelManager->profitModel()->reloadData();
    } else {
        _modelManager->spendModel()->reloadData();
    }
}

void CoreAPI::setCurrentUser(int uid, int guid)
{
    _current_user = uid;

    if ( _current_group == guid || guid <= 0 ) {
        return;
    }

    _current_group = guid;
    reloadStartPoint();
}

bool CoreAPI::hasStartPoint()
{
    return _start_point.isValid();
}

void CoreAPI::setStartPoint( const QDate &date )
{
    if ( !_current_group || !date.isValid() ) {
        return;
    }

    auto func = TypeStorage::func(DB_COMMON(set_start_point));
    (*func)->bindValue("group_id", _current_group);
    (*func)->bindValue("start_point", date);

    _pg_worker->execute(**func);
    reloadStartPoint();
}

bool CoreAPI::checkDateFormat(const QString &date_str)
{
    QDate d;

    return dateFromStr( date_str, d );
}

double CoreAPI::currentConsuption()
{
    return currentPeriodSumm(false);
}

double CoreAPI::currentProfit()
{
    return currentPeriodSumm(true);
}

void CoreAPI::loadPurchases(const QString & dateFrom, const QString & dateTo)
{
    QDate from, to;

    if (!dateFromStr(dateFrom,from) || !dateFromStr(dateTo, to)) {
        throw RE("CoreAPI::loadPurchases() Incorrect date string.");
    }

    loadPurchases(from, to);
}

void CoreAPI::loadPurchases()
{
    auto period = currentPeriod();
    loadPurchases(period.first, period.second);
}

void CoreAPI::setModelManager(ModelManager *mm)
{
    _modelManager = mm;
}

void CoreAPI::initProfitGroupCallback() noexcept
{
    ST.groupsProfit()->setInsertHandler( [&](PNodeIndex index) -> void {
        try {
            loadGroupsByParent( index, ST.groupsProfit() );
        } catch ( const exception &ex ) {
            cout << ex.what() << endl;
        }
    } );
}

void CoreAPI::initSpendGroupCallback() noexcept
{
    ST.groupsSpend()->setInsertHandler( [&](PNodeIndex index) -> void {
        try {
            loadGroupsByParent( index, ST.groupsSpend() );
        } catch ( const exception &ex ) {
            cout << ex.what() << endl;
        }
    } );

}

void CoreAPI::loadGroupsByParent(PNodeIndex parent, PGStorage *st)
{
    auto func = TypeStorage::func( DB_COMMON(get_groups) );

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

void CoreAPI::reloadStartPoint()
{
    auto func = TypeStorage::func(DB_COMMON(get_start_point));
    (*func)->bindValue("group_id", _current_group);

    auto answer = _pg_worker->execute( **func );

    if ( !answer ) {
        return;
    }

    QDate date;

    if ( !answer->tryConvert(date) ) {
        return;
    }

    _start_point = date;
}

bool CoreAPI::dateFromStr(const QString &date_str, QDate &date)
{
    if ( QDate::fromString(date_str, "dd.MM.yyyy").isValid() ) {
        date = QDate::fromString(date_str, "dd.MM.yyyy");
        return true;
    }
    if (QDate::fromString(date_str, "dd-MM-yyyy").isValid()) {
        date = QDate::fromString(date_str, "dd-MM-yyyy");
        return true;
    }
    if ( QDate::fromString(date_str, "dd:MM:yyyy").isValid() ) {
        date = QDate::fromString(date_str, "dd:MM:yyyy");
        return true;
    }

    return false;
}

void CoreAPI::addTransaction( const QString &rec, QString summ, const
                              QString &date_str, int amount, bool profit )
{
    QDate date;

    if ( summ.contains(",") ) {
        for ( int i(0); i < summ.size(); ++i ) {
            if ( summ[i] == "," ) {
                summ.replace(i, 1, ".");
                break;
            }
        }
    }

    auto dsumm = summ.toDouble();

    if ( !dateFromStr(date_str,date) ) {
        throw runtime_error("CoreAPI::addTransaction() Invalid date string!");
    }

    auto st = profit ? ST.recordsProfit()
                     : ST.recordsSpend();

    auto rec_it = std::find_if(st->begin(), st->end(),
                               [&rec]( const auto &record ){
        auto name_qstr = QString::fromStdString( record->name() );
        return rec.compare(name_qstr, Qt::CaseInsensitive) == 0;
    });

    if ( rec_it == st->end() ) {
        throw runtime_error("CoreAPI::addTransaction() Cannot find given record");
    }

    for ( int i(0); i < amount; ++i ) {
        auto func = TypeStorage::func(DB_COMMON(add_purchase));
        (*func)->bindValue("u_group_id", _current_group);
        (*func)->bindValue("user_id", _current_user);
        (*func)->bindValue("record_id", (*rec_it)->id());
        (*func)->bindValue("p_summ", dsumm);
        (*func)->bindValue("p_date", date);
        (*func)->bindValue("creation_date", QDate::currentDate());

        auto answer = _pg_worker->execute(**func);

        if (!answer) {
            throw runtime_error("CoreAPI::addTransaction() Cannot add purchase"); // todo
        }
    }
}

pair<QDate, QDate> CoreAPI::currentPeriod()
{
    if (!_start_point.isValid()) {
        throw runtime_error("CoreAPI::currentPeriod(): Invalid start point");
    }

    auto today = QDate::currentDate();
    QDate from(today.year(), today.month(), _start_point.day());
    QDate to(from);

    if (from.day() > today.day()) {
        from = from.addMonths(-1);
        to = to.addDays(-1);
    } else {
        to = to.addMonths(1).addDays(-1);
    }

    return make_pair(from, to);
}

vector<int> CoreAPI::periodRootGroups(CoreAPI::date_pair dpair, bool profit)
{
    if (!dpair.first.isValid() || !dpair.second.isValid()) {
        throw runtime_error("CoreAPI::periodRootGroups() Invalid date given.");
    }

    if (dpair.first.daysTo(dpair.second) < 0) {
        std::swap(dpair.first, dpair.second);
    }

    auto func = TypeStorage::func(DB_COMMON(period_root_groups));
    (*func)->bindValue("date_from", dpair.first);
    (*func)->bindValue("date_to", dpair.second);
    (*func)->bindValue("profit", profit);

    auto answer = _pg_worker->execute(**func);

    if (!answer) {
        throw runtime_error("CoreAPI::periodRootGroups() Cannot exec query");
    }

    if (answer->columns() != 1) {
        if (answer->columns() == 0) {
            return {};
        }
        throw runtime_error("CoreAPI::periodRootGroups() Unexpected answer.");
    }

    const auto rows = answer->rows();

    if (!rows) {
        return {};
    }

    vector<int> res(rows);

    for ( size_t i(0); i < rows; ++i ) {
        res[i] = answer->field(i,0).value.toInt();
    }

    return res;
}

double CoreAPI::currentPeriodSumm(bool profit)
{
    double summ(0.0);
    auto period = currentPeriod();

    vector<int> g_ids = periodRootGroups(period, profit);

    for ( const auto &id : g_ids ) {
        auto func = TypeStorage::func(DB_COMMON(summ_purchases));
        (*func)->bindValue("date_from", period.first);
        (*func)->bindValue("date_to", period.second);
        (*func)->bindValue("group_id", id);

        auto answer = _pg_worker->execute(**func);

        if (!answer) {
            stringstream ss;
            ss << "CoreAPI::currentConsuption() Cannot execute "
               << (*func)->schema().toStdString() << "."
               << (*func)->name().toStdString() << " with params: "
               << "date_from = "
               << period.first.toString().toStdString()
               << ", date_to = "
               << period.second.toString().toStdString()
               << ", group_id = "
               << id;

            throw runtime_error(ss.str());
        }

        double res;
        if (!answer->tryConvert(res)) {
            throw runtime_error("CoreAPI::currentConsuption() Cannot convert answer");
        }

        summ += res;
    }

    return summ;
}

void CoreAPI::loadPurchases(const QDate &from, const QDate &to)
{
    auto func = TypeStorage::func(DB_COMMON(get_purchases));

    (*func)->bindValue("date_from", from);
    (*func)->bindValue("date_to", to);
    (*func)->bindValue("profit", false);

    auto answer = _pg_worker->execute(**func);

    if (!answer) {
        throw RE("CoreAPI::loadPurchases() Query exec error");
    }

    auto st = ST.purchasesSpend();
    st->clear();

    const auto size = answer->rows();

    for (size_t i(0); i < size; ++i) {
        auto pr = new Purchase();

        try {
            pr->fromPgAnswer(answer, i);
        } catch (const exception &ex) {
            cout << ex.what() << endl;
            delete pr;
            throw;
        }

        st->push_back(pr);
    }

    _modelManager->purchaseModel()->reloadData();
}

