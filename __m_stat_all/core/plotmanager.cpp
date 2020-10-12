#include "plotmanager.h"
#include "plotbase.h"

using namespace std;

void PlotManager::registerPlot(PlotBase * plot)
{
    ++id_counter_;

    plot->setPlotId(id_counter_);

    plots_.insert(make_pair(id_counter_, plot));
}
