#ifndef QUAD_TREE_ALGORITHM_VISUALIZER_H
#define QUAD_TREE_ALGORITHM_VISUALIZER_H
#include "ui_panel.h"
#include "algorithm_visualizer_base.h"
#include "../Algorithms/QuadTree/QuadTree/quad_tree_widget.h"

class AlgorithmVisualizerQuadTree: public AlgorithmVisualizerBase{

    Q_OBJECT

 public:
    AlgorithmVisualizerQuadTree(){
        name = "Quad tree";
        initUIPanel();
        initLayout();
    }
    void setPoints(vector<QPoint> points){
        holder.reset();
        holder.boundingBox = AABB<int>(0,0,this->width(),this->height());
        holder.addElements(pointsToElements(points));
        this->update();
    }
    virtual ~AlgorithmVisualizerQuadTree() override{}
public slots:
    virtual void reset() override{
        points.clear();
        holder.reset();
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
            this->sendPointsToQTree();
            callUpdate = true;
        }
    }
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        sendPointsToQTree();

        //Draw tree
        for(auto &boundingBox: holder.visualisationHelper->getNonLeafNodesBoundingBoxes()){
            int xCtr = boundingBox.xMin + (boundingBox.xMax - boundingBox.xMin)/2;
            int yCtr = boundingBox.yMin + (boundingBox.yMax - boundingBox.yMin)/2;
            painter.drawLine(boundingBox.xMin,yCtr,boundingBox.xMax,yCtr);
            painter.drawLine(xCtr,boundingBox.yMin,xCtr,boundingBox.yMax);

        }

        //Draw boxes
        for(auto element: holder.elementsPtrs){
            painter.setBrush(QBrush(Qt::cyan));
            int width = element->aabb.xMax - element->aabb.xMin;
            int height = element->aabb.yMax - element->aabb.yMin;
            painter.drawRect(element->aabb.xMin,element->aabb.yMin,width,height);
        }
//        for(QPoint point: points){
//            painter.setBrush(QBrush(Qt::yellow));
//            painter.drawRect(point.x(),point.y(),quadSide,quadSide);
//        }

        //Highlight colliding boxes
        auto overlappingObjects = holder.getOverlappingObjects();
        for(auto overlappingObject: overlappingObjects){
            painter.setBrush(QBrush(Qt::red));
            int width = overlappingObject->aabb.xMax - overlappingObject->aabb.xMin;
            int height = overlappingObject->aabb.yMax - overlappingObject->aabb.yMin;
            painter.drawRect(overlappingObject->aabb.xMin,overlappingObject->aabb.yMin,width,height);
        }

    }
    void sendPointsToQTree(){
        holder.update();
        holder.boundingBox = AABB<int>(0,0,this->width(),this->height());
        holder.addElements(pointsToElements(points));
        points.clear();
    }
    vector<AABB<int>> pointsToElements(vector<QPoint> &points){
        vector<AABB<int>> elements;
        for(auto point: points){
            int width = 10 + point.x()+rand()%30;
            int height = 10 + point.y()+rand()%30;
            elements.push_back(AABB<int>(point.x(),point.y(),width,height));
        }
        return  elements;
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
        vector<QPoint> points;
        for(int i=0;i<randomLineEdit.text().toInt();i++){
            QPoint point(rand()%800,rand()%800);
            points.push_back(point);
        }
        setPoints(points);
    }

protected:

    MyCustomElementsHolder holder;
    int quadSide = 5;
    vector<QPoint> points;
    UIPanel uiPanel;
    QPushButton randomButton{"Random"};
    QLabel randomLabel{"in range from 1 to "};
    QLineEdit randomLineEdit{"55"};
    QIntValidator randomInputValidator{1, 9999};
};

#endif // QUAD_TREE_ALGORITHM_VISUALIZER_H
