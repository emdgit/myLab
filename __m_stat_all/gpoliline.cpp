#include "gpoliline.h"

GPoliLine::GPoliLine() {}

void GPoliLine::drawGraph(QPainter * const & painter) const
{
    for ( const auto &_l : _lines )
        _l->drawGraph( painter );
}

void GPoliLine::addLine(GLine * const &line) noexcept
{
    _lines.push_back( line );
}

GLine *GPoliLine::operator[](const int &i) noexcept
{
    return _lines[ static_cast<size_t>( i ) ];
}


