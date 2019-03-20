#ifndef KD_TREE_H
#define KD_TREE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

#include <QVector2D>


#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test



struct KDTreeElement;
struct KDTreeVisualizationHelper;
class KDTreeNode;



class KDTree
{
public:
    virtual ~KDTree()=default;
    virtual KDTreeElement* getElementClosestTo(KDTreeElement* element)=0;
    virtual void setNewElements(std::vector<KDTreeElement*> elements)=0;
    virtual void clear()=0;
    virtual KDTreeVisualizationHelper* getVisualizationHelper() const=0;
};

struct KDTreeElement: public QVector2D{
    KDTreeElement(){}
    KDTreeElement(float x,float y):QVector2D(x,y){}
};

struct AABB{
    AABB():xMin(0),yMin(0),xMax(0),yMax(0){}
    AABB(double xMin, double yMin, double xMax, double yMax):
        xMin(xMin),xMax(xMax),yMin(yMin),yMax(yMax){}
    double xMin,xMax,yMin,yMax;
    std::array<AABB,2> split(int axis){
        if(axis%2==0){
            return {
                AABB(xMin,yMin,xMin+(xMax-xMin)/2,yMax),
                AABB(xMin+(xMax-xMin)/2,yMin,xMax,yMax)};
        }else{
            return {
                AABB(xMin,yMin,xMax,yMin+(yMax-yMin)/2),
                AABB(xMin,yMin+(yMax-yMin)/2,xMax,yMax)};
        }
    }
    double minDistanceTo(const KDTreeElement* element) const{
        bool isInsideXRange = xMin<=element->x() && xMax >= element->x();
        bool isInsideYRange = yMin<=element->y() && yMax >= element->y();
        if(isInsideXRange && isInsideYRange){
            return 0;
        }else if(isInsideXRange){
            if(element->y() < yMin) return yMin - element->y();
            else return element->y() - yMax;
        }else if(isInsideYRange){
            if(element->x() < xMin) return xMin - element->x();
            else return element->x() - xMax;
        }else{
            double closestX, closestY;
            if(element->x() < xMin) closestX = xMin;
            else closestX = xMax;
            if(element->y() < yMin) closestY = yMin;
            else closestY = yMax;
            return sqrt(pow(closestX-element->x(),2)+pow(closestY-element->y(),2));
        }
    }
    bool operator ==(const AABB &other) const{
        return xMin == other.xMin && yMin == other.yMin &&
               xMax == other.xMax && yMax == other.yMax;
    }
    void print(){
        std::cout<<"xMin = "<<xMin<<" yMin = "<<yMin<<" xMax = "<<xMax<<" yMax = "<<yMax<<std::endl;
    }
};

struct KDTreeVisualizationHelper{
    virtual ~KDTreeVisualizationHelper(){};
};

class KDTreeBase: public KDTree
{
public:
    virtual ~KDTreeBase() override=default;
    virtual KDTreeVisualizationHelper* getVisualizationHelper() const override{
        return visualizationHelper;
    }
protected:
    friend struct KDTreeVisualizationHelper;
    KDTreeVisualizationHelper *visualizationHelper;
};


struct KDTreeFirstImplVisualizationHelper: public KDTreeVisualizationHelper{
    virtual ~KDTreeFirstImplVisualizationHelper(){}
};

class KDTreeNode{
public:
    KDTreeNode(){}
    KDTreeNode(KDTreeElement *element):element(element){}
    void setChildren(int leftChildId, int rightChildId){
        this->leftChildId = leftChildId;
        this->rightChildId = rightChildId;
    }
    void setElement(KDTreeElement *element){
        this->element = element;
    }
    int leftChildId = -1, rightChildId = -1;
    KDTreeElement* element = nullptr;
};



class KDTreeFirstImpl: public KDTreeBase{
public:
    KDTreeFirstImpl(){
        visualizationHelper = new KDTreeFirstImplVisualizationHelper();
    }
    KDTreeFirstImpl(const std::vector<KDTreeElement*> &elements): KDTreeFirstImpl(){
        setNewElements(elements);
    }
    virtual ~KDTreeFirstImpl() override{
        delete  visualizationHelper;
    }
    virtual KDTreeElement* getElementClosestTo(KDTreeElement* element) override{
        KDTreeElement* nearestElement = nullptr;

        return nearestElement;
    }
    virtual void setNewElements(std::vector<KDTreeElement*> elements) override{
        clear();
        sortElements(elements);
        removeAndHashNonUniqueElements(elements);
        buildTree(elements.begin(),elements.end());
    }
    virtual void clear() override{
        nodes.clear();
        sameValueElements.clear();
    }

protected:
    void sortElements(std::vector<KDTreeElement*> &elements){
        std::sort(elements.begin(),elements.end(),[](const KDTreeElement* a,const KDTreeElement* b){
            if(a->x() != b->x()) return a->x() < b->x();
            else return a->y() < b->y();
        });
    }
    void removeAndHashNonUniqueElements(std::vector<KDTreeElement*> &elements){
        if(elements.size() > 1){
            std::vector<KDTreeElement*> sameElements,uniqueElements;
            KDTreeElement* prevElement = nullptr;
            auto hashSameElementsIfNeeded = [&](){
                if(sameElements.size() > 1){
                    sameValueElements.insert({prevElement,sameElements});
                }
                uniqueElements.push_back(prevElement);
                sameElements.clear();
            };

            for(int i=0;i<elements.size();i++){
                auto currElement = elements.at(i);
                if(prevElement != nullptr){
                    if(*currElement != *prevElement){
                        hashSameElementsIfNeeded();
                    }
                }
                prevElement = currElement;
                sameElements.push_back(currElement);
            }
            hashSameElementsIfNeeded();
            elements = uniqueElements;
        }
    }
    using ITERATOR = std::vector<KDTreeElement*>::iterator;
    int buildTree(const ITERATOR first, const ITERATOR last, unsigned iteration = 0){
        if(first==last) return -1;
        auto distance = std::distance(first,last);
        auto middle = std::next(first,distance/2);
        int nodeId = makeNode();
        if(std::next(first) != last){
            auto comparator = [iteration](const KDTreeElement* a,const KDTreeElement* b){
                if(iteration%2 == 0) return a->x() < b->x();
                else return a->y() < b->y();
            };
            std::nth_element(first,middle,last,comparator);
            int leftChildId = buildTree(first,middle,iteration+1);
            int rightChildId = buildTree(std::next(middle),last,iteration+1);
            nodes.at(nodeId).setChildren(leftChildId,rightChildId);
        }
        nodes.at(nodeId).setElement(*middle);
        return nodeId;
    }
    void searchForClosestNode(int currNodeId, int &closestNodeId, AABB &colosestNodeAAB, int iteration=0){

    }
    int makeNode(){
        auto nodeId = nodes.size();
        nodes.push_back(KDTreeNode());
        return nodeId;
    }
protected:
    FRIEND_TEST(KDTreeFirstImpl,sortElements);
    FRIEND_TEST(KDTreeFirstImpl,removeAndHashNonUniqueElements);
    FRIEND_TEST(KDTreeFirstImpl,buildTree);
    std::vector<KDTreeNode> nodes;
    std::unordered_map<KDTreeElement*,std::vector<KDTreeElement*>> sameValueElements;
};



#endif // KD_TREE_H
