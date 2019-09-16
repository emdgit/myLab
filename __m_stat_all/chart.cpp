#include "chart.h"

#include <QPen>
#include <QPainter>

#include <random>
#include <ctime>

static constexpr int    _linesCount = 2;

static constexpr qreal  _dxOrdinate = 0.15;    //  Procent value
static constexpr qreal  _dyAbscissa = 0.15;    //  Procent value

class ChartPrivate
{
public:
    ChartPrivate ( Chart * const &chart ) :_chart(chart) {}

    void initPolyLine() const noexcept
    {
        _chart->_graph = new GPoliLine();

        for ( int i(0); i < _linesCount; ++i )
        {
            auto line = new GLine();
            line->setMarker( &_chart->_marker );
            _chart->_graph->addLine( line );
        }
    }

    void resetLines() const noexcept
    {
        for ( size_t i(0); i < _chart->_graph->size(); ++i )
        {
            auto line = (*_chart->_graph)[ static_cast<int>( i )];

            auto w = _chart->width() * ( 1 - _dxOrdinate );
            auto h = _chart->height() * ( 1 - _dyAbscissa );

            std::default_random_engine engine( static_cast<unsigned int>( time(nullptr) ) );
            std::uniform_real_distribution<qreal> distrX( _chart->width() - w, _chart->width() );
            std::uniform_real_distribution<qreal> distrY( _chart->height() - h, _chart->height() );
            std::uniform_int_distribution<size_t> distrC( 3, 5 );

            auto count = distrC( engine );

            auto minXDistance = ( w / static_cast<qreal>( count ) ) / 5;

            std::vector<qreal> xVal;
            xVal.assign( count, 0 );

            auto genX = [&]() {
                for ( size_t i(0); i < count; ++i )
                    xVal[i] = distrX( engine );

                std::sort( xVal.begin(), xVal.end() );
            };

            auto checkX = [&]() -> bool {
                    for ( size_t i(0); i < count - 1; ++i )
                    if ( xVal[i+1] - xVal[i] < minXDistance )
                    return false;
            return true;
            };

            genX();

            while ( !checkX() )
            genX();

            QVector< QPointF > pVec;

            for ( size_t i(0); i < count; ++i )
            {
                pVec << QPointF( xVal[i], distrY( engine ) );
            }

            line->swapVectors( pVec );
        }
    }

private:

    Chart *     _chart;

};


Chart::Chart(QQuickItem * parent) : QQuickPaintedItem (parent)
{
    setWidth( 100 );
    setHeight( 100 );
    _marker = 100;

    m_ptr = new ChartP( this );

    QQ( Chart );

    m->initPolyLine();
    m->resetLines();
}

Chart::~Chart() {}

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

void Chart::makeNewGraphs() const noexcept
{
    QQ ( const Chart );

    m->resetLines();
}
