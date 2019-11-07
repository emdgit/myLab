#ifndef GPOLILINE_H
#define GPOLILINE_H

#include "gline.h"

class GPoliLine : public GraphBase
{
public:
    GPoliLine();

    void                    drawGraph(QPainter * const & painter) const override;
    void                    addLine( GLine * const &line ) noexcept;

    GLine *                 operator[]( const int &i ) noexcept;

    inline size_t           size() const noexcept { return _lines.size(); }


private:

    std::vector<GLine*>     _lines;


};

#endif // GPOLILINE_H
