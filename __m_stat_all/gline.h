#ifndef GLINE_H
#define GLINE_H

#include "graphbase.h"

class GLine : public GraphBase
{
public:
    GLine();
    GLine( const QColor &color );

    void                    drawGraph(QPainter * const & painter) const override;
    void                    addPoint( const QPointF &p ) noexcept;
    void                    addPoint( const qreal &x, const qreal &y ) noexcept;
    void                    swapVectors(QVector<QPointF> &other ) noexcept;
    void                    setColor( const QColor &color ) noexcept;


private:

    QVector< QPointF >      _points;

    QColor                  _color;

};

#endif // GLINE_H
