#ifndef QTREEVISUALISATIONWIDGET_H
#define QTREEVISUALISATIONWIDGET_H

#include <QtWidgets>
#include <iostream>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <tuple>
#include <ctime>
#include <cmath>
#include <type_traits>

#include "quad_tree.h"
#include "quad_tree_slow.h"
#include "quad_tree_moderate.h"
#include "quad_tree_fast.h"
using std::vector;
using std::array;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;

struct MyCustomElement: public QuadTreeElement<int>{

    typedef int T;
    typedef QuadTreeElement<int>::ELEMENT_PTR ELEMENT_PTR;
    typedef  std::conditional<  is_shared_ptr<ELEMENT_PTR>::value,
                                shared_ptr<MyCustomElement>,
                                MyCustomElement*>::type Type;

    MyCustomElement(){
        init();
    }
    MyCustomElement(const AABB<int> &aabb, QColor color=Qt::yellow): QuadTreeElement(aabb), color(color){
        pos = QVector2D(aabb.xMin,aabb.yMin);
        init();
    }
    virtual ~MyCustomElement(){}
    void init(){
        QMatrix4x4 rotationMat;
        rotationMat.rotate(rand()%360,0,0,1);
        direction = (QVector2D(1,0).toVector4D()*rotationMat).toVector2D();
        speed = 1+rand()%3;
    }
    template <class V=ELEMENT_PTR, typename std::enable_if<is_shared_ptr<V>::value>::type* = nullptr>
    static shared_ptr<MyCustomElement> makeElement(const AABB<int> &aabb, QColor color=Qt::yellow)
    {
      return std::make_shared<MyCustomElement>(MyCustomElement(aabb,color));
    }

    template <class V=ELEMENT_PTR, typename std::enable_if<std::is_pointer<V>::value>::type* = nullptr>
    static MyCustomElement* makeElement(const AABB<int> &aabb, QColor color=Qt::yellow)
    {
      return new MyCustomElement(aabb,color);
    }
    void update(){
        pos = pos + direction * speed;
        this->aabb.translateBy(pos.x()-this->aabb.xMin,pos.y()-this->aabb.yMin);
    }
    QColor color;
    float speed = 6;
    QVector2D direction;
    QVector2D pos;
};

struct MyCustomElementsHolder{
    MyCustomElementsHolder():quadTree(new QuadTreeFast<int>()){
        visualisationHelper = quadTree->getVisualisationHelper();
    }
    void addElement(const AABB<int> &aabb, QColor color=Qt::yellow){
        elementsPtrs.push_back(MyCustomElement::makeElement(aabb,color));
        vector<QuadTreeElement<int>::Type> elementsCastedPtrs(elementsPtrs.begin(),elementsPtrs.end());
        quadTree->setElements(elementsCastedPtrs,AABB<int>(0,0,799,799));
    }
    void addElements(vector<AABB<int>> aabbs){
        for(auto aabb: aabbs){
            elementsPtrs.push_back(MyCustomElement::makeElement(aabb));
        }
        vector<QuadTreeElement<int>::Type> elementsCastedPtrs(elementsPtrs.begin(),elementsPtrs.end());
        quadTree->setElements(elementsCastedPtrs,boundingBox,6,4);
    }
    vector<QuadTreeElement<int>::Type> getOverlappingObjects(){
        auto overlappingElements = quadTree->getAllOverlappingElements();
        vector<QuadTreeElement<int>::Type> overlappingElementsCasted(overlappingElements.size());
        for(int i=0;i<overlappingElements.size();i++){
            overlappingElementsCasted.at(i) = QuadTreeElement<int>::dynamicCast<MyCustomElement>(overlappingElements.at(i));
        }
        return overlappingElementsCasted;
    }
    void reset(){
        elementsPtrs.clear();
        quadTree->reset();
    }
    void update(){

        auto overlappingTuples = quadTree->getAllOverlappingElementTuples();
        for(auto overlapingTuple: overlappingTuples){
            auto element0 = QuadTreeElement<int>::dynamicCast<MyCustomElement>(std::get<0>(overlapingTuple));
            auto element1 = QuadTreeElement<int>::dynamicCast<MyCustomElement>(std::get<1>(overlapingTuple));
            int width0 =  element0->aabb.xMax - element0->aabb.xMin;
            int height0 = element0->aabb.yMax - element0->aabb.yMin;
            int width1 =  element1->aabb.xMax - element1->aabb.xMin;
            int height1 = element1->aabb.yMax - element1->aabb.yMin;
            int x0Ctr = element0->aabb.xMin + width0 /2;
            int y0Ctr = element0->aabb.yMin + height0/2;
            int x1Ctr = element1->aabb.xMin + width1 /2;
            int y1Ctr = element1->aabb.yMin + height1/2;
            int xOverlap = abs(abs(x0Ctr-x1Ctr) - (width0+width1)/2);
            int yOverlap = abs(abs(y0Ctr-y1Ctr) - (height0+height1)/2);
            if(xOverlap < yOverlap){
                if(x0Ctr < x1Ctr){
                    element0->direction.setX(-abs(element0->direction.x()));
                    element1->direction.setX( abs(element1->direction.x()));
                }else{
                    element0->direction.setX( abs(element0->direction.x()));
                    element1->direction.setX(-abs(element1->direction.x()));
                }
            }else{
                if(y0Ctr < y1Ctr){
                    element0->direction.setY(-abs(element0->direction.y()));
                    element1->direction.setY( abs(element1->direction.y()));
                }else{
                    element0->direction.setY( abs(element0->direction.y()));
                    element1->direction.setY(-abs(element1->direction.y()));
                }
            }

        }
        for(auto element: elementsPtrs){

            if(element->aabb.xMin < boundingBox.xMin){
                element->direction.setX(abs(element->direction.x()));
            }
            if(element->aabb.xMax > boundingBox.xMax){
                element->direction.setX(-abs(element->direction.x()));
            }
            if(element->aabb.yMin < boundingBox.yMin){
                element->direction.setY(abs(element->direction.y()));
            }
            if(element->aabb.yMax > boundingBox.yMax){
                element->direction.setY(-abs(element->direction.y()));
            }
            element->update();
        }
    }

    vector<MyCustomElement::Type> elementsPtrs;
    unique_ptr<QuadTree<int>> quadTree;
    const QuadTreeVisualionHelper<int> *visualisationHelper;
    AABB<int> boundingBox{0,0,799,799};
};

class QTreeVisualisationWidget: public QWidget
{
    Q_OBJECT
public:
    QTreeVisualisationWidget(){
        this->setMinimumSize(800,800);
        connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
        timer.start(updateIntervalMs);
        srand( time( 0 ) );
    }
    void setPoints(vector<QPoint> points){
        holder.reset();
        holder.boundingBox = AABB<int>(0,0,this->width(),this->height());
        holder.addElements(pointsToElements(points));
        this->update();
    }
public slots:
    void reset(){
        points.clear();
        holder.reset();
//        this->update();
    }
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override{
        bool callUpdate = false;
        std::cout<<mouseEvent->pos().x()<<" "<<mouseEvent->pos().y()<<std::endl;
        if(mouseEvent->buttons() == Qt::RightButton){
            if(timer.isActive()){
                timer.stop();
            }else{
                timer.start(updateIntervalMs);
            }
            callUpdate = true;
        }else if(mouseEvent->buttons() == Qt::LeftButton){
            points.push_back(mouseEvent->pos());
            this->sendPointsToQTree();
            callUpdate = true;
        }
//        if(callUpdate) this->update();
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
//        update();
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

    MyCustomElementsHolder holder;
    int quadSide = 5;
    vector<QPoint> points;
    QTimer timer{this};
    int updateIntervalMs = 30;
};

#endif // QTREEVISUALISATIONWIDGET_H
