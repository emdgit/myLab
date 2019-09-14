#include "gpoliline.h"

GPoliLine::GPoliLine()
{

}

void GPoliLine::drawGraph(QPainter * const & painter) const
{
    for ( const auto &_l : _lines )
        _l->drawGraph( painter );
}

void GPoliLine::addLine( const qreal & k, const qreal & d,
                         const qreal & start, const qreal & end, qreal * const & marker) noexcept
{
    auto l = new GLine( k, d, start, end, marker );
    addLine( l );
}

void GPoliLine::addLine(GLine * const & line) noexcept
{
    _lines.push_back( line );
}
