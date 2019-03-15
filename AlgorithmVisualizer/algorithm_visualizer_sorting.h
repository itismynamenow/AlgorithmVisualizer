#ifndef ALGORITHM_VISUALIZER_SORTING_H
#define ALGORITHM_VISUALIZER_SORTING_H
#include "ui_panel.h"
#include "algorithm_visualizer_base.h"
#include "../Algorithms/Sorting/sorts.h"

class AlgorithmVisualizerSorting: public AlgorithmVisualizerBase{

    Q_OBJECT

public:
    AlgorithmVisualizerSorting(){
        name = "Sorting";
        initUIPanel();
        initLayout();
    }
    virtual ~AlgorithmVisualizerSorting(){}
public slots:
    virtual void reset() override{
        elementsToSort.clear();
    }
    virtual void clear() override{
        reset();
    }
protected:
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setBrush(QBrush(Qt::cyan));
        if(elementsToSort.size()>0){
            int availableWidth = this->width() - margin*2;
            int meanWidth = availableWidth / elementsToSort.size();
            int availableHeight = this->height() - margin*2;
            int meanHeight = availableHeight / elementsToSort.size();
            for(int i=0;i<elementsToSort.size();i++){
                int x = i * meanWidth + margin;
                int y = (elementsToSort.size() - elementsToSort.at(i)) * meanHeight + margin;
                int height = elementsToSort.at(i) * meanHeight;
                painter.drawRect(x,y,meanWidth,height);
            }
        }
        stopper.stopped = false;
    }
    void initUIPanel(){
        uiPanel.addLineOfWidgets({&randomButton, &randomLabel, &randomLineEdit});
        connect(&randomButton, SIGNAL(clicked()), this, SLOT(setRandomData()));
        randomLineEdit.setValidator(&randomInputValidator);
    }
    void initLayout(){
        mainLayout.addWidget(&uiPanel,0,0,1,1, Qt::AlignLeft | Qt::AlignTop);
        setLayout(&mainLayout);
    }

protected slots:
    void setRandomData(){
        int elementsToGenerate = randomLineEdit.text().toInt();
        elementsToSort.resize(elementsToGenerate);
        std::iota (std::begin(elementsToSort), std::end(elementsToSort), 1);
        random_shuffle(elementsToSort.begin(), elementsToSort.end());
        insertionSort.setStopper(&stopper);
        std::thread ([this]{insertionSort.sort(elementsToSort.begin(), elementsToSort.end());}).detach();
//        auto t = new std::thread ([this]{insertionSort.sort(elementsToSort.begin(), elementsToSort.end());});
//        t->detach();
    }

protected:
    UIPanel uiPanel;
    QPushButton randomButton{"Random"};
    QLabel randomLabel{"in range from 1 to "};
    QLineEdit randomLineEdit{"55"};
    QIntValidator randomInputValidator{1, 99};
    std::vector<int> elementsToSort;
    int margin = 50;
    Stopper stopper;
    InsertionSort<typename std::vector<int>::iterator> insertionSort;

};

#endif // ALGORITHM_VISUALIZER_SORTING_H
