#include "plotbase.h"

using namespace std;

PlotBase::PlotBase(QQuickItem * parent) :
    QQuickPaintedItem(parent)
{
    connect(this, &PlotBase::widthChanged, [&] {
        resizePlot();
    });

    connect(this, &PlotBase::heightChanged, [&] {
        resizePlot();
    });
}

void PlotBase::addData(const QVariant & k, double v)
{
    data_.push_back(make_pair(k, v));
}

void PlotBase::setViewRange(size_t left, size_t right)
{
    viewRange_.first = left;
    viewRange_.second = right;
}

const std::pair<size_t, size_t> &PlotBase::viewRange() const
{
    return viewRange_;
}

void PlotBase::resizePlot()
{
    // update viewRange
    // repaint
    qt_noop();
}

std::vector<int> PlotBase::yScale() const
{
    if (data_.empty()) {
        return {0};
    }

    if (!checkViewRange()) {
        throw runtime_error("Invalid View Range");
    }

    auto min = data_[viewRange_.first].second;
    auto max = data_[viewRange_.first].second;

    for (size_t i(viewRange_.first + 1); i <= viewRange_.second; ++i) {
        if (data_[i].second < min) {
            min = data_[i].second;
        }
        if (data_[i].second > max) {
            max = data_[i].second;
        }
    }

    auto dv = static_cast<int>((max - min) / 4);

    vector<int> out{4};

    for (size_t i(0); i < 4; ++i) {
        int x = static_cast<int>(min) +
                static_cast<int>(i) *
                static_cast<int>(dv);
        out[i] = x;
    }

    return out;
}

bool PlotBase::checkViewRange() const
{
    if (viewRange_.first >= data_.size() ||
        viewRange_.second >= data_.size() ||
        viewRange_.first > viewRange_.second) {
        return false;
    }
    return true;
}
