#pragma once

#include <QQuickPaintedItem>

#include <vector>

;   // haha, QtCreator's bug detected. =)
#pragma pack(push, 1)

// Базовый класс, рисующий непосредственно
// данные графика.
// Встраивается в PlotForm. // todo make it independent
class PlotBase : public QQuickPaintedItem
{

    Q_OBJECT

    Q_PROPERTY(int plotId
               READ plotId
               WRITE setPlotId
               NOTIFY plotIdChanged)

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

    int plotId() const;


public slots:

    void setPlotId(int plotId);


signals:

    void plotIdChanged(int plotId);


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

    int plotId_;
};

#pragma pack(pop)
