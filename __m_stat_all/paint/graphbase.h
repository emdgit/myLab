#ifndef GRAPHBASE_H
#define GRAPHBASE_H

#include <QPainter>

class GraphBase
{
public:

    GraphBase();
    virtual ~GraphBase();

    virtual void            drawGraph( QPainter * const &painter ) const = 0;

    inline void             setStart( const qreal &p ) noexcept { _startPoint = p; }
    inline void             setEnd( const qreal &p ) noexcept { _endPoint = p; }
    inline void             setMarker( qreal * const &marker ) noexcept { _xMarker = marker; }

    inline const qreal &    start() const noexcept  { return _startPoint; }
    inline const qreal &    end() const noexcept  { return _endPoint; }


protected:

    qreal                   _startPoint;
    qreal                   _endPoint;
    qreal *                 _xMarker;

};

#endif // GRAPHBASE_H
