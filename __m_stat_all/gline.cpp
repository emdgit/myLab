#include "gline.h"

GLine::GLine() : _color( Qt::red ) {}

GLine::GLine(const QColor &color) : _color(color) {}

void GLine::drawGraph(QPainter * const & painter) const
{
    if ( _points.size() < 2 )
        return;

    painter->setPen( _color );

    for ( auto i(0); i < _points.size() - 1; ++i )
    {
        auto p1 = _points[i];
        auto p2 = _points[i+1];

        if ( *_xMarker < p1.x() )
            break;

        if ( *_xMarker < p2.x() )
        {
            auto y = ( ( (*_xMarker) - p1.x() ) * ( p2.y() - p1.y() ) ) / ( p2.x() - p1.x() ) + p1.y();
            p2 = { *_xMarker, y };
        }

        painter->drawLine( p1, p2 );
    }
}

void GLine::addPoint(const QPointF &p) noexcept
{
    _points << p;

    std::sort( _points.begin(), _points.end(), []( const QPointF &p1, const QPointF &p2 ){
        return p1.x() < p2.x();
    } );
}

void GLine::addPoint(const qreal &x, const qreal &y) noexcept
{
    addPoint( { x, y } );
}

void GLine::swapVectors(QVector<QPointF> &other) noexcept
{
    _points.swap( other );
}
