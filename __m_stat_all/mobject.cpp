#include "mobject.h"

#include <QGuiApplication>

MObject::MObject(QObject *parent) : QObject(parent)
{

}

MObject::~MObject()
{
    qDebug( "~MObject()" );
}

void MObject::qmlAbout()
{
    qDebug( "ABOUT.." );
}

void MObject::qmlQuit()
{
    qDebug( "Quit.. =(" );
    qApp->quit();
}

void MObject::playSound()
{

}

const QString MObject::randomIcon() const
{
    QDir d( "debug/Images/" );
    auto fList = d.entryInfoList( QDir::Files );
    auto size = fList.size();

    if ( !size )
        return "";

    qsrand( static_cast<uint>( QTime::currentTime().msec() ) );

    auto i = qrand() % size;

    return fList[ i ].absoluteFilePath();
}
