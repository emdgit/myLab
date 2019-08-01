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
