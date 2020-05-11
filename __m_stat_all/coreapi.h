#ifndef COREAPI_H
#define COREAPI_H

#include <QObject>
#include <QDate>

namespace pg {
    class Worker;
}

class CoreAPI : public QObject
{

    Q_OBJECT

public:

    Q_INVOKABLE
    static bool addPurchase( const int &groupId, const int &userId,
                             const int &recordId, const double &summ,
                             const QDate &date ) noexcept;

private:

    static pg::Worker * _pg_worker;


};

#endif // COREAPI_H
