#ifndef DUMMY_ALGORITHM_VISUALIZER_H
#define DUMMY_ALGORITHM_VISUALIZER_H
#include <iostream>

#include <QtWidgets>

#include "algorithm_visualizer_base.h"
#include "ui_panel.h"

class AlgorithmVisualizerDummy: public AlgorithmVisualizerBase
{
    Q_OBJECT

public:
    AlgorithmVisualizerDummy(){
        name = "Placeholder";
        initUIPanel();
        initLayout();
        this->setMinimumSize(800, 800);
        connect(this->timer,SIGNAL(timeout()),this,SLOT(update()));
    }
    void paintEvent(QPaintEvent *e) override{
        QPainter painter(this);
        painter.setBrush(QBrush(Qt::cyan));
        painter.drawRect(0,0,800,800);
    }
    virtual QString getName() override{return QString("PlaceholderAlgorithm");}
public slots:
    virtual void reset() override{std::cout<<"Reset"<<std::endl;}
    virtual void clear() override{std::cout<<"Clear"<<std::endl;}
    virtual void stopResume() override{std::cout<<"StopResume"<<std::endl;}
protected:
    void initUIPanel(){
        uiPanel.addLineOfWidgets({&someButton,&someLabel,&someLineEdit});
        uiPanel.addLineOfWidgets({&someButton1,&someLabel1,&someLineEdit1});
    }
    void initLayout(){
        mainLayout.addWidget(&uiPanel,0,0,1,1, Qt::AlignLeft | Qt::AlignTop);
        setLayout(&mainLayout);
    }
protected:
    UIPanel uiPanel;
    QGridLayout mainLayout;
    QPushButton someButton{"Some button"};
    QPushButton someButton1{"Some button1"};
    QLineEdit someLineEdit{"Some line edit"};
    QLineEdit someLineEdit1{"Some line edit1"};
    QLabel someLabel{"Some label"};
    QLabel someLabel1{"Some label1"};
};
#endif // DUMMY_ALGORITHM_VISUALIZER_H
