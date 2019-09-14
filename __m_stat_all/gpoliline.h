#ifndef GPOLILINE_H
#define GPOLILINE_H

#include "gline.h"

class GPoliLine : public GraphBase
{
public:
    GPoliLine();

    void                    drawGraph(QPainter * const & painter) const override;
    void                    addLine( const qreal &k, const qreal &d,
                                     const qreal &start, const qreal &end,
                                     qreal * const &marker ) noexcept;
    void                    addLine( GLine * const &line ) noexcept;


private:

    std::list<GLine*>       _lines;


};

#endif // GPOLILINE_H
