#ifndef PLOTBASE_H
#define PLOTBASE_H

#include <QQuickPaintedItem>

#include <vector>

// Базовый класс, рисующий непосредственно
// данные графика.
// Встраивается в PlotForm. // todo make it independent
class PlotBase : public QQuickPaintedItem
{
public:
    PlotBase(QQuickItem *parent = nullptr);

    // QQuickPaintedItem interface
    virtual void paint(QPainter * painter) = 0;

    // PlotBase interface.
    /*!
     * \brief   Нарисовать подсказку. Действие при наведении
     *          мыши на график.
     */
    virtual void printHintLabel(int x, int y) const = 0;

    void addData(const QVariant &k, double v);
    void setViewRange(size_t left, size_t right);

    const std::pair<size_t, size_t> &viewRange() const;


protected:

    /*!
     * \brief Вызывается, если была изменена ширина или
     * высота Item'a в GUI
     * \default По  умолчанию ничего не происходит.
     */
    virtual void resizePlot();

    std::vector<int> yScale() const;

    std::pair<size_t, size_t>   viewRange_ = {0,0};


private:

    bool checkViewRange() const;

    std::vector<std::pair<QVariant, double>>    data_;

};

#endif // PLOTBASE_H
