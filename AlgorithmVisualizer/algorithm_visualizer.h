#ifndef ALGORITHM_VISUALIZER_H
#define ALGORITHM_VISUALIZER_H
#include <QtWidgets>
class AlgorithmVisualizer: public QWidget
{
    Q_OBJECT

public:
    virtual QString getName()=0;
    virtual ~AlgorithmVisualizer()=default;
public slots:

    virtual void reset()=0;
    virtual void clear()=0;
    virtual void stopResume()=0;

};
#endif // ALGORITHM_VISUALIZER_H
