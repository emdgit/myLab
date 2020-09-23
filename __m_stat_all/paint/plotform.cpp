#include "plotform.h"
#include "plotbase.h"

#include <QPainter>

static constexpr qreal plot_x = 50;
static constexpr qreal plot_y = 50;

#include "plotlinear.h"
PlotForm::PlotForm(QQuickItem * parent) :
    QQuickPaintedItem(parent)
{
    setSize(QSizeF(500, 200));
    plot_ = new PlotLinear(this);
    resizePlot();

    connect(this, &PlotForm::widthChanged, [&] {
        resizePlot();
    });

    connect(this, &PlotForm::heightChanged, [&] {
        resizePlot();
    });
}

void PlotForm::paint(QPainter * painter)
{
    painter->setBrush(Qt::red);
    painter->drawRect(0,0, width(), height());
    plot_->update();
}

PlotBase *PlotForm::plot() const
{
    return plot_;
}

void PlotForm::setPlot(PlotBase * plot)
{
    if (plot == plot_) {
        return;
    }

    plot_ = plot;

    emit plotChanged(plot_);
}

void PlotForm::resizePlot() const
{
    plot_->setX(plot_x);
    plot_->setY(plot_y);
    plot_->setSize(QSizeF(width() - plot_x * 2,
                          height() - plot_y * 2));
}

