#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <QObject>
#include <QStringList>

class TestModel : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QStringList data READ data CONSTANT)

public:

    TestModel(QObject *parent = nullptr);

    QStringList data() const;

};



#endif // TESTMODEL_H
