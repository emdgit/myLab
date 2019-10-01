#include "testmodel.h"

TestModel::TestModel(QObject *parent) :QObject (parent)
{

}

QStringList TestModel::data() const
{
    return QStringList() << "orange" << "skyblue";
}
