#pragma once

#include <QQuickPaintedItem>

#include <vector>

;   // haha, QtCreator's bug detected. =)
#pragma pack(push, 1)

// Базовый класс, рисующий непосредственно
// данные графика.
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
     * \brief   Вызывается, если была изменена ширина или
     *          высота Item'a в GUI
     * \default По  умолчанию ничего не происходит.
     */
    virtual void resizePlot();

    /// Вернуть отображаемые на шкале Y значения
    std::vector<int> yScale() const;

    /// Точки центрирования данных графика.
    std::vector<QPoint> anchors_;

    /// Диапазон отображаемых значений. Индексы массива данных
    std::pair<size_t, size_t>   viewRange_ = {0,0};


private:

    /// Проверить валидность данных во viewRange_
    bool checkViewRange() const;

    /// Данные графика. (Подпись по оси Х / значение)
    std::vector<std::pair<QVariant, double>>    data_;

    /// Идентификатор графика. Получается при создании.
    int plotId_;
};

#pragma pack(pop)
