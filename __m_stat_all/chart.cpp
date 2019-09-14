#include "chart.h"

#include <QPen>
#include <QPainter>

#include "gpoliline.h"

static constexpr qreal  _dxOrdinate = 0.15;    //  Procent value
static constexpr qreal  _dyAbscissa = 0.15;    //  Procent value

Chart::Chart(QQuickItem * parent) : QQuickPaintedItem (parent)
{
    setWidth( 100 );
    setHeight( 100 );
    _marker = 100;

    auto poly = new GPoliLine();
    poly->addLine( 1.5, 10, 10, 50, &_marker );
    poly->addLine( 1.5, 15, 10, 80, &_marker );

    _graph = poly;
}

void Chart::paint(QPainter * painter)
{
    QPen pen( _color, _width );
    painter->setPen( pen );

    if ( this->smooth() )
        painter->setRenderHint( QPainter::Antialiasing, true );

    painter->setTransform( QTransform(1,0,0,-1,0,height()) );
    painter->save();

    QPen penG;
    penG.setColor( QColor( "#183A8F" ) );
    penG.setWidth( 3 );

    painter->setPen( penG );

    auto ordinateX = width() * _dxOrdinate;
    auto abscissaY = height() * _dyAbscissa;

    QLineF ordinate( ordinateX, abscissaY, ordinateX, height() - abscissaY );

    painter->drawLine( ordinate );

    painter->restore();
    _graph->drawGraph( painter );
}

void Chart::setMarker(const qreal & m) noexcept
{
    _marker = m;
    update();
}
