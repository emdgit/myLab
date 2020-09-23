#ifndef PLOTLINEAR_H
#define PLOTLINEAR_H

#include "plotbase.h"

// Реализует график из линий
/*
 * \       0--0
 * \     /     \
 * \ 0--0       0
 * x---------------
*/
class PlotLinear : public PlotBase
{
public:
    PlotLinear(QQuickItem * parent = nullptr);

    // QQuickPaintedItem interface
    void paint(QPainter * painter) override;
};

#endif // PLOTLINEAR_H
