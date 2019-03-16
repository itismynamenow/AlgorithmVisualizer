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
        initSorts();
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
        uiPanel.addLineOfWidgets({&randomButton, &randomLabel, &randomLineEdit, &dropdown});
        connect(&randomButton, SIGNAL(clicked()), this, SLOT(setRandomData()));
        dropdown.addItem("Insertion sort");
        dropdown.addItem("Selection sort");
        dropdown.addItem("Bubble sort");
        dropdown.addItem("Merge sort");
        dropdown.addItem("Quick sort");
        randomLineEdit.setValidator(&randomInputValidator);
    }
    void initLayout(){
        mainLayout.addWidget(&uiPanel,0,0,1,1, Qt::AlignLeft | Qt::AlignTop);
        setLayout(&mainLayout);
    }
    void initSorts(){
        sorts.at(0) = &insertionSort;
        sorts.at(1) = &selectionSort;
        sorts.at(2) = &bubbleSort;
        sorts.at(3) = &mergeSort;
        sorts.at(4) = &quickSort;
    }

protected slots:
    void setRandomData(){
        int elementsToGenerate = randomLineEdit.text().toInt();
        elementsToSort.resize(elementsToGenerate);
        std::iota (std::begin(elementsToSort), std::end(elementsToSort), 1);
        random_shuffle(elementsToSort.begin(), elementsToSort.end());
        sorts.at(dropdown.currentIndex())->setStopper(&stopper);
        std::thread ([this]{(*sorts.at(dropdown.currentIndex())).sort(elementsToSort.begin(), elementsToSort.end());}).detach();
    }

protected:
    UIPanel uiPanel;
    QPushButton randomButton{"Random"};
    QLabel randomLabel{"in range from 1 to "};
    QLineEdit randomLineEdit{"55"};
    QComboBox dropdown;
    QIntValidator randomInputValidator{1, 99};
    std::vector<int> elementsToSort;
    int margin = 50;
    Stopper stopper;
    InsertionSort<typename std::vector<int>::iterator> insertionSort;
    SelectionSort<typename std::vector<int>::iterator> selectionSort;
    BubbleSort<typename std::vector<int>::iterator> bubbleSort;
    MergeSort<typename std::vector<int>::iterator> mergeSort;
    QuickSort<typename std::vector<int>::iterator> quickSort;
    std::array<SortingAlgorithm<typename std::vector<int>::iterator>*,5> sorts;

};

#endif // ALGORITHM_VISUALIZER_SORTING_H
