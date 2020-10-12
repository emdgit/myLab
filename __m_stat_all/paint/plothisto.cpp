#include "plothisto.h"

#include <QPainter>

#define INT(expr) static_cast<int>(expr)

PlotHisto::PlotHisto(QQuickItem * parent) :
    PlotBase (parent)
{

}

void PlotHisto::paint(QPainter * painter)
{
    // Paint Y-scale
    auto y_scale = yScale();    // min -> max
    auto y_scale_h = INT(height() - 2 * margin_);
    auto y_scale_label_h = INT(y_scale_h / INT(y_scale.size()));
    auto y_scale_label_w = yScaleWidth_ - 2 * margin_;

    QSize label_size;
    QFont label_font;

    label_size.setWidth(y_scale_label_w);
    label_size.setHeight(y_scale_label_h);

    label_font.setFamily("Franklin Gothic Medium Cond");
    label_font.setPixelSize(18);

    painter->save();

    for (size_t i(0); i < y_scale.size(); ++i) {
        QPoint label_point;
        QRect label_rect;

        int y = y_scale_h - margin_ - (y_scale_label_h * INT(i + 1)) +
                INT(y_scale_label_h / 2) - yScaleLabelHeight_;

        label_point.setX(margin_);
        label_point.setY(y);

        label_rect.setTopLeft(label_point);
        label_rect.setSize(label_size);

        painter->setFont(label_font);
        painter->drawText(label_rect,
                          Qt::AlignCenter,
                          QString::number(y_scale[i]));
    }

    painter->restore();

    // Paint X-scale

    // Paint histograms

}

void PlotHisto::printHintLabel(int x, int y) const
{
    (void)x;
    (void)y;
}
