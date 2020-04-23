#ifndef CHART_H
#define CHART_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QQmlInfo>

#include "graphbase.h"
#include "private.h"
#include "gpoliline.h"

class ChartPrivate;
typedef ChartPrivate        ChartP;

class Chart : public QQuickPaintedItem
{

    Q_OBJECT

    Q_PROPERTY(QColor color READ color  WRITE setColor  NOTIFY colorChanged     USER true)
    Q_PROPERTY(qreal pWidth READ pWidth WRITE setPWidth NOTIFY widthChanged     USER true)
    Q_PROPERTY(qreal marker READ marker WRITE setMarker                         USER true)

public:
    Chart( QQuickItem *parent = nullptr );
    ~Chart() override;

// QQuickPaintedItem interface
    void                    paint(QPainter * painter) override;

    inline void             setColor( const QColor &cl ) noexcept   { _color = cl; }
    inline void             setPWidth( const qreal &w ) noexcept    { _width = w;  }
    void                    setMarker(  const qreal &m) noexcept;

    inline const QColor &   color() const noexcept                   { return _color;  }
    inline const qreal &    pWidth() const noexcept                  { return _width;  }
    inline const qreal &    marker() const noexcept                  { return _marker; }

    Q_INVOKABLE void        makeNewGraphs() const noexcept;


signals:

    void                    colorChanged();
    void                    widthChanged();


private:

    ChartP *                m_ptr;
    DECLARE_PRIVATE(Chart)

    GPoliLine *             _graph;

    QColor                  _color;

    qreal                   _width;
    qreal                   _marker;

};

QML_DECLARE_TYPE(Chart)

#endif // CHART_H
