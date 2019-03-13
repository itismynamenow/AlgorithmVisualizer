#ifndef ALGORITHM_VISUALIZER_BASE_H
#define ALGORITHM_VISUALIZER_BASE_H
#include <QtWidgets>

#include "algorithm_visualizer.h"

class AlgorithmVisualizerBase: public AlgorithmVisualizer
{
    Q_OBJECT

public:
    AlgorithmVisualizerBase(){
        timer = new QTimer(this);
        timer->start(updateTimeMs);
    }
    virtual void reset() override=0;
    virtual void clear() override=0;
    virtual void stopResume() override=0;
    virtual QString getName() override{
        return name;
    }
    virtual ~AlgorithmVisualizerBase() override{
        delete  timer;
    }
protected:
    QString name{"Default Name"};
    //Update timer
    QTimer* timer;
    int updateTimeMs = 30;
};
#endif // ALGORITHM_VISUALIZER_BASE_H
