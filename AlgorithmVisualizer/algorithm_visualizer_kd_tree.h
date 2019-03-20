#ifndef ALGORITHM_VISUALIZER_KD_TREE_H
#define ALGORITHM_VISUALIZER_KD_TREE_H
#include "ui_panel.h"
#include "algorithm_visualizer_base.h"

#include "../Algorithms/KDTree/KDTree/kd_tree.h"

class AlgorithmVisualizerKDTree: public AlgorithmVisualizerBase{

    Q_OBJECT

public:
    AlgorithmVisualizerKDTree(){
        name = "KD tree";
        initUIPanel();
        initLayout();
        kdTree = new KDTreeFirstImpl();
        visualisationHelper = new KDTreeFirstImplVisualizationHelper(dynamic_cast<KDTreeFirstImpl*>(kdTree));
    }
    virtual ~AlgorithmVisualizerKDTree(){}
    void setPoints(std::vector<QPoint> points){
        reset();
        this->points = points;
        sendPointsToKDTree();
    }
public slots:
    virtual void reset() override{
        points.clear();
        elements.clear();
        kdTree->clear();
    }
    virtual void clear() override{
        reset();
    }
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override{
        bool callUpdate = false;
        std::cout<<mouseEvent->pos().x()<<" "<<mouseEvent->pos().y()<<std::endl;
        if(mouseEvent->buttons() == Qt::RightButton){
            if(timer->isActive()){
                timer->stop();
            }else{
                timer->start(updateTimeMs);
            }
            callUpdate = true;
        }else if(mouseEvent->buttons() == Qt::LeftButton){
            points.push_back(mouseEvent->pos());
            this->sendPointsToKDTree();
            callUpdate = true;
        }
    }
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);

        auto lineData = visualisationHelper->getSeparatingLines();
        for(auto &pair: lineData){
            auto a = pair.first;
            int lineWidth = 6 - pair.second;
            if(lineWidth < 2) lineWidth = 1;
            painter.setPen(QPen(Qt::black,lineWidth));
            painter.drawLine(a.at(0),a.at(1),a.at(2),a.at(3));
        }
        painter.setPen(QPen(Qt::black,1));
        for(QPoint point: points){
            painter.setBrush(QBrush(Qt::yellow));
            painter.drawEllipse(point.x()-pointSize/2,point.y()-pointSize/2,pointSize,pointSize);
        }
    }
    void sendPointsToKDTree(){
        for(const auto &point: points){
            elements.push_back(KDTreeElement(point.x(),point.y()));
        }
        std::vector<KDTreeElement*> elementsPtrs;
        for(auto &element:elements){
            elementsPtrs.push_back(&element);
        }
        kdTree->setNewElements(elementsPtrs);
        this->update();
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
    virtual void setRandomData(){
        std::vector<QPoint> points;
        for(int i=0;i<randomLineEdit.text().toInt();i++){
            QPoint point(rand()%800,rand()%800);
            points.push_back(point);
        }
        setPoints(points);
    }
protected:
    KDTree *kdTree;
    KDTreeVisualizationHelper *visualisationHelper;
    std::vector<KDTreeElement> elements;
    std::vector<QPoint> points;
    UIPanel uiPanel;
    QPushButton randomButton{"Random"};
    QLabel randomLabel{"in range from 1 to "};
    QLineEdit randomLineEdit{"55"};
    QIntValidator randomInputValidator{1, 9999};

    int pointSize = 7;
};
#endif // ALGORITHM_VISUALIZER_KD_TREE_H
