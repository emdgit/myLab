#ifndef PLOTBASE_H
#define PLOTBASE_H

#include <QQuickPaintedItem>

// Базовый класс, рисующий непосредственно
// данные графика. Встраивается в PlotForm.
class PlotBase : public QQuickPaintedItem
{
public:
    PlotBase(QQuickItem *parent = nullptr);

    // QQuickPaintedItem interface
    virtual void paint(QPainter * painter) = 0;
};

#endif // PLOTBASE_H
