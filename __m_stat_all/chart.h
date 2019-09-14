#ifndef CHART_H
#define CHART_H

#include <QQuickPaintedItem>
#include <QColor>

#include "graphbase.h"
#include "private.h"

class Chart : public QQuickPaintedItem
{

    Q_OBJECT

    Q_PROPERTY(QColor color READ color  WRITE setColor  NOTIFY colorChanged     USER true)
    Q_PROPERTY(qreal pWidth READ pWidth WRITE setPWidth NOTIFY widthChanged     USER true)
    Q_PROPERTY(qreal marker READ marker WRITE setMarker                         USER true)

public:
    Chart( QQuickItem *parent = nullptr );

// QQuickPaintedItem interface
    void                    paint(QPainter * painter) override;

    inline void             setColor( const QColor &cl ) noexcept   { _color = cl; }
    inline void             setPWidth( const qreal &w ) noexcept    { _width = w;  }
    void                    setMarker(  const qreal &m) noexcept;

    inline const QColor &   color() const noexcept                   { return _color;  }
    inline const qreal &    pWidth() const noexcept                  { return _width;  }
    inline const qreal &    marker() const noexcept                  { return _marker; }


signals:

    void                    colorChanged();
    void                    widthChanged();


private:

    GraphBase *             _graph;

    QColor                  _color;

    qreal                   _width;
    qreal                   _marker;

};

QML_DECLARE_TYPE(Chart)

#endif // CHART_H
