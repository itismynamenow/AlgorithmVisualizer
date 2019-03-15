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
        this->setMinimumSize(800, 800);
        connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    }
    virtual QString getName() override{
        return name;
    }
    virtual ~AlgorithmVisualizerBase() override{
        delete  timer;
    }
public slots:
    virtual void reset() override=0;
    virtual void clear() override=0;
    virtual void stopResume() override{
        if(timer->isActive()){
            timer->stop();
        }else{
            timer->start(updateTimeMs);
        }
    }

protected:
    QString name{"Default Name"};
    //Update timer
    QTimer* timer;
    int updateTimeMs = 30;    
    QGridLayout mainLayout;
};
#endif // ALGORITHM_VISUALIZER_BASE_H
