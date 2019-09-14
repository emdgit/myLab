#include "gline.h"

GLine::GLine() {}

GLine::GLine(const qreal & k, const qreal & d) : _k(k), _d(d) {}

GLine::GLine(const qreal & k, const qreal & d, const qreal & start, const qreal & end, qreal * const & marker)
{
    setK( k );
    setD( d );
    setStart( start );
    setEnd( end );
    _xMarker = marker;
}

void GLine::drawGraph(QPainter * const & painter) const
{
    if ( _xMarker )
        if ( *_xMarker < _startPoint )
            return;

    QPointF p1( _startPoint, _k * _startPoint + _d );

    auto end = _xMarker ? *_xMarker > _endPoint ? _endPoint
                                                : *_xMarker
                                                : _endPoint;
    QPointF p2( end, _k * end + _d );

    QLineF line( p1, p2 );

    painter->drawLine( line );
}
