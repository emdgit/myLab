#ifndef GLINE_H
#define GLINE_H

#include "graphbase.h"

class GLine : public GraphBase
{
public:
    GLine();
    GLine( const qreal &k, const qreal &d );
    GLine( const qreal &k, const qreal &d,
           const qreal &start, const qreal &end,
           qreal * const &marker );

    void                    drawGraph(QPainter * const & painter) const override;

    inline void             setK( const qreal &k ) noexcept { _k = k; }
    inline void             setD( const qreal &d ) noexcept { _d = d; }

    inline const qreal &    kCoef() const noexcept  { return _k; }
    inline const qreal &    dCoef() const noexcept  { return _d; }


private:

    /*!
     * \brief   f(x) = kx + d
     */
    qreal                   _k, _d;

};

#endif // GLINE_H
