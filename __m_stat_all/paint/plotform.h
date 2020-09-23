#ifndef PLOTFORM_H
#define PLOTFORM_H

#include <QQuickPaintedItem>

class PlotBase;

/// Форма, содержащая на себе график
class PlotForm : public QQuickPaintedItem
{

    Q_OBJECT

    Q_PROPERTY(PlotBase* plot
               READ plot
               WRITE setPlot
               NOTIFY plotChanged)

public:
    PlotForm(QQuickItem *parent = nullptr);

    // QQuickPaintedItem interface
    void paint(QPainter * painter) override;

    PlotBase *  plot() const;


public slots:

    void        setPlot(PlotBase *plot);


signals:

    void        plotChanged(PlotBase *newPlot);


protected:

    void        resizePlot() const;


private:

    PlotBase *  plot_ = nullptr;

};

#endif // PLOTFORM_H
