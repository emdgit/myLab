#ifndef PLOTHISTO_H
#define PLOTHISTO_H

#include "plotbase.h"

// Реализует график-гистограмму
/*!
 * \       0
 * \ 0     0
 * \ 0  0  0  0
 * \ 0  0  0  0  0
 * \ 0  0  0  0  0
 * x-----------------
 */
class PlotHisto : public PlotBase
{
public:
    PlotHisto(QQuickItem * parent = nullptr);

    // QQuickPaintedItem interface
    void paint(QPainter * painter) override;

    // PlotBase interface
    void printHintLabel(int x, int y) const override;


protected:

    void paintYScale(QPainter * painter) const;
    void paintHisto(QPainter * painter) const;


private:

    /// Ширина, отведенная наотрисовку шкалы Y
    static constexpr int yScaleWidth_ = 50;

    /// Высота области значения в шкале Y
    static constexpr int yScaleLabelHeight_ = 25;

    /// Ширина столбика гистограммы
    static constexpr int histoWidth_ = 25;

    /// Интервал между столбиками гистограммы
    static constexpr int histoMargin_ = 25;

    /// Общий отступ от границ
    static constexpr int margin_ = 5;

};

#endif // PLOTHISTO_H
