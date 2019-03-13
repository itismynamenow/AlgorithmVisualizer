#ifndef ALGORITHM_VISUALIZER_H
#define ALGORITHM_VISUALIZER_H
#include <QtWidgets>
class AlgorithmVisualizer: public QWidget
{
    Q_OBJECT

public:
    virtual void reset()=0;
    virtual void clear()=0;
    virtual void stopResume()=0;
    virtual QString getName()=0;
    virtual ~AlgorithmVisualizer()=default;
};
#endif // ALGORITHM_VISUALIZER_H
