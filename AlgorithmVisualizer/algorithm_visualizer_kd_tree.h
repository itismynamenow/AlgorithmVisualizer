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
        referencePointSet = false;
        closestPointFound = false;
    }
    virtual void clear() override{
        reset();
    }
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override{
        bool callUpdate = false;
        if(mouseEvent->buttons() == Qt::RightButton){
            referencePointSet = true;
            referencePoint = mouseEvent->pos();
            findClosestElement();
        }else if(mouseEvent->buttons() == Qt::LeftButton){
            points.push_back(mouseEvent->pos());
            this->sendPointsToKDTree();
        }
    }
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);

        auto visitedAABBs = visualisationHelper->getVisitedAABBs();
        for(auto aabb: visitedAABBs){
            if(aabb.xMin < 0) aabb.xMin=0;
            if(aabb.yMin < 0) aabb.yMin=0;
            if(aabb.xMax > this->width()) aabb.xMax = this->width();
            if(aabb.yMax > this->height()) aabb.yMax = this->height();
            auto width = aabb.xMax - aabb.xMin;
            auto height = aabb.yMax - aabb.yMin;
            QColor color{0,30,30};
            color.setAlpha(40);
            painter.setBrush(color);
            painter.drawRect(aabb.xMin,aabb.yMin,width,height);
        }
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
        if(referencePointSet){
            painter.setBrush(QBrush(Qt::green));
            painter.drawEllipse(referencePoint.x()-pointSize/2,referencePoint.y()-pointSize/2,pointSize,pointSize);
        }
        if(closestPointFound){
            painter.setBrush(QBrush(Qt::red));
            painter.drawEllipse(closestPoint.x()-pointSize/2,closestPoint.y()-pointSize/2,pointSize,pointSize);
        }
    }
    void findClosestElement(){
        if(referencePointSet){
            KDTreeElement element {(float)referencePoint.x(), (float)referencePoint.y()};
            auto closestElements = kdTree->getElementsClosestTo(&element);

            if(closestElements.size() > 0){
                closestPoint = QPoint(closestElements.at(0)->x(),closestElements.at(0)->y());
                closestPointFound = true;
            }
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
        findClosestElement();
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
    QLineEdit randomLineEdit{"777"};
    QIntValidator randomInputValidator{1, 9999};

    QPoint referencePoint;
    QPoint closestPoint;
    bool referencePointSet = false;
    bool closestPointFound = false;
    int pointSize = 7;
};
#endif // ALGORITHM_VISUALIZER_KD_TREE_H
