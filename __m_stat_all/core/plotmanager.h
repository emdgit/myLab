#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H

#include <unordered_map>

class PlotBase;

class PlotManager
{
public:
    PlotManager() = delete;

    static void registerPlot(PlotBase * plot);


private:

    static inline std::unordered_map<int, PlotBase*> plots_;

    static inline int id_counter_ = 0;

};

#endif // PLOTMANAGER_H
