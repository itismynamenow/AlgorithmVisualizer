#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <memory>
#include <iostream>

#include <QtWidgets>

#include "algorithm_visualizer_dummy.h"
#include "ui_panel.h"
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr){
        init();
        initAlgorithmVisulizer();
        initUIPanel();
        initConnects();
        setLayout(&mainLayout);
    }
    ~MainWindow(){}
    void addAlgorithmVisualizer(AlgorithmVisualizer* algorithmVisualizer){
        algorithmVisualizers.push_back(algorithmVisualizer);
        dropdown.addItem(algorithmVisualizer->getName());
    }
protected:
    void init(){
        mainLayout.setSpacing(0);
        mainLayout.setMargin(0);
    }
    void initAlgorithmVisulizer(){
        defaultAlgorithmVisualizer = std::unique_ptr<AlgorithmVisualizer>(new AlgorithmVisualizerDummy());
        addAlgorithmVisualizer(defaultAlgorithmVisualizer.get());
        setCurrAlgorithmVisualiser(defaultAlgorithmVisualizer.get());
    }
    void initUIPanel(){
        uiPanel.addLineOfWidgets({&dropdown,&resetButton,&clearButton,&stopResumeButton});
        mainLayout.addWidget(&uiPanel,1,0,1,1,Qt::AlignHCenter | Qt::AlignBottom);
    }
    void initConnects(){
        connect(&dropdown,SIGNAL(currentIndexChanged(int)),this,SLOT(onDropdownChange()));
        connect(&resetButton,SIGNAL(pressed()),this,SLOT(onResetButtonPress()));
        connect(&clearButton,SIGNAL(pressed()),this,SLOT(onClearButtonPress()));
        connect(&stopResumeButton,SIGNAL(pressed()),this,SLOT(onStopResumeButtonPress()));
    }
protected slots:
    void setCurrAlgorithmVisualiser(AlgorithmVisualizer* algorithmVisualizer){
        if(algorithmVisualizer != currAlgorithmVisualizer){
            mainLayout.removeWidget(currAlgorithmVisualizer);
            currAlgorithmVisualizer = algorithmVisualizer;
            currAlgorithmVisualizer->setParent(this);
            mainLayout.addWidget(currAlgorithmVisualizer,0,0,2,1);
            currAlgorithmVisualizer->stackUnder(&uiPanel);
        }
    }
    void onDropdownChange(){
        setCurrAlgorithmVisualiser(algorithmVisualizers.at(dropdown.currentIndex()));
    }
    void onResetButtonPress(){
        currAlgorithmVisualizer->reset();
    }
    void onClearButtonPress(){
        currAlgorithmVisualizer->clear();
    }
    void onStopResumeButtonPress(){
        currAlgorithmVisualizer->stopResume();
    }

protected:

    AlgorithmVisualizer* currAlgorithmVisualizer = nullptr;
    std::unique_ptr<AlgorithmVisualizer> defaultAlgorithmVisualizer;
    std::vector<AlgorithmVisualizer*> algorithmVisualizers;

    // Bottom UI Panel
    QPushButton resetButton{"Reset"};
    QPushButton clearButton{"Clear"};
    QPushButton stopResumeButton{"Stop/Resume"};
    QComboBox dropdown;
    UIPanel uiPanel;

    QGridLayout mainLayout;

};
#endif // MAIN_WINDOW_H
