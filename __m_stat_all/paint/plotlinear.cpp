#include "plotlinear.h"

#include <QPainter>

PlotLinear::PlotLinear(QQuickItem * parent) :
    PlotBase(parent)
{
}

void PlotLinear::paint(QPainter * painter)
{
    painter->setBrush(Qt::green);
    painter->drawRect(0,0,width(), height());
}
