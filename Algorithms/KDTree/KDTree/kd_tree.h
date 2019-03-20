#ifndef KD_TREE_H
#define KD_TREE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <limits>

#include <QVector2D>

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test



struct KDTreeElement;
struct KDTreeVisualizationHelper;
class KDTreeNode;



struct KDTreeElement: public QVector2D{
    KDTreeElement(){}
    KDTreeElement(float x,float y):QVector2D(x,y){}
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

class KDTree
{
public:
    virtual ~KDTree()=default;
    virtual std::vector<KDTreeElement*> getElementsClosestTo(KDTreeElement* element)=0;
    virtual void setNewElements(std::vector<KDTreeElement*> elements)=0;
    virtual void clear()=0;
};

struct KDTreeVisualizationHelper{
    virtual ~KDTreeVisualizationHelper(){};
    virtual std::vector<std::pair<std::array<double,4>,int>> getSeparatingLines()=0;
};

struct AABB_D{
    AABB_D():xMin(0),yMin(0),xMax(0),yMax(0){}
    AABB_D(double xMin, double yMin, double xMax, double yMax):
        xMin(xMin),xMax(xMax),yMin(yMin),yMax(yMax){}
    double xMin,xMax,yMin,yMax;
    std::array<AABB_D,2> split(int axis, KDTreeElement *element) const{
        if(axis%2==0){
            return {
                AABB_D(xMin,yMin,element->x(),yMax),
                AABB_D(element->x(),yMin,xMax,yMax)};
        }else{
            return {
                AABB_D(xMin,yMin,xMax,element->y()),
                AABB_D(xMin,element->y(),xMax,yMax)};
        }
    }
    static AABB_D biggestAABB(){
        return  AABB_D(std::numeric_limits<double>::min(),std::numeric_limits<double>::min(),
                    std::numeric_limits<double>::max(),std::numeric_limits<double>::max());
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
    bool operator ==(const AABB_D &other) const{
        return xMin == other.xMin && yMin == other.yMin &&
               xMax == other.xMax && yMax == other.yMax;
    }
    void print(){
        std::cout<<"xMin = "<<xMin<<" yMin = "<<yMin<<" xMax = "<<xMax<<" yMax = "<<yMax<<std::endl;
    }
};


class KDTreeFirstImpl: public KDTree{
public:
    KDTreeFirstImpl(){ }
    KDTreeFirstImpl(const std::vector<KDTreeElement*> &elements): KDTreeFirstImpl(){
        setNewElements(elements);
    }
    virtual ~KDTreeFirstImpl() override{
    }
    virtual std::vector<KDTreeElement*> getElementsClosestTo(KDTreeElement* element) override{
        std::vector<KDTreeElement*> nearestElements;
        if(nodes.size()>0){
            int closestNodeId = -1;
            double closestDistanceToElement = std::numeric_limits<double>::max();
            searchForClosestNode(0,AABB_D::biggestAABB(),element,closestNodeId,closestDistanceToElement);
            auto closestElement = nodes.at(closestNodeId).element;
            if(sameValueElements.count(closestElement) != 0){
                nearestElements = sameValueElements.at(closestElement);
            }else{
                nearestElements.push_back(closestElement);
            }
        }
        return nearestElements;
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
    void searchForClosestNode(int currNodeId,const AABB_D &currNodeAABB, KDTreeElement *element,int &closestNodeId, double &closestDistanceToElement, int iteration=0){
        double minPossibleDistanceToElement = currNodeAABB.minDistanceTo(element);
        if(minPossibleDistanceToElement < closestDistanceToElement && currNodeId != -1){
            auto currNode = nodes.at(currNodeId);
            auto currDistanceToElement = (*currNode.element - *element).length();
            if(currDistanceToElement < closestDistanceToElement){
                closestNodeId = currNodeId;
                closestDistanceToElement = currDistanceToElement;
            }
            auto aabbs = currNodeAABB.split(iteration,currNode.element);
            auto leftChildDistanceToElement = (*nodes.at(currNode.leftChildId).element - *element).length();
            auto rightChildDistanceToElement = (*nodes.at(currNode.rightChildId).element - *element).length();
            if(leftChildDistanceToElement < rightChildDistanceToElement){
                searchForClosestNode(currNode.leftChildId,aabbs.at(0),element,closestNodeId,closestDistanceToElement,iteration+1);
                searchForClosestNode(currNode.rightChildId,aabbs.at(1),element,closestNodeId,closestDistanceToElement,iteration+1);
            }else{
                searchForClosestNode(currNode.rightChildId,aabbs.at(1),element,closestNodeId,closestDistanceToElement,iteration+1);
                searchForClosestNode(currNode.leftChildId,aabbs.at(0),element,closestNodeId,closestDistanceToElement,iteration+1);
            }

        }
    }
    int makeNode(){
        auto nodeId = nodes.size();
        nodes.push_back(KDTreeNode());
        return nodeId;
    }
protected:
    friend class KDTreeFirstImplVisualizationHelper;
    FRIEND_TEST(KDTreeFirstImpl,sortElements);
    FRIEND_TEST(KDTreeFirstImpl,removeAndHashNonUniqueElements);
    FRIEND_TEST(KDTreeFirstImpl,buildTree);
    std::vector<KDTreeNode> nodes;
    std::unordered_map<KDTreeElement*,std::vector<KDTreeElement*>> sameValueElements;
};

struct KDTreeFirstImplVisualizationHelper: public KDTreeVisualizationHelper{
    KDTreeFirstImplVisualizationHelper(KDTreeFirstImpl *kdTree):kdTree(kdTree){}
    virtual ~KDTreeFirstImplVisualizationHelper(){}
    virtual std::vector<std::pair<std::array<double,4>,int>> getSeparatingLines(){
        std::vector<std::pair<std::array<double,4>,int>> result;
        if(kdTree->nodes.size() > 0){
            iterateOverKDTree(0,AABB_D(0,0,800,800),result);
        }
        return result;
    }
protected:
    void iterateOverKDTree(int currNodeId, AABB_D aabb,std::vector<std::pair<std::array<double,4>,int>> &lineData, int iteration=0){
        if(currNodeId==-1) return;
        auto currNode = kdTree->nodes.at(currNodeId);
        auto currElement = currNode.element;
        if(iteration%2==0){
            lineData.push_back(std::pair<std::array<double,4>,int>(std::array<double,4>{currElement->x(),aabb.yMin,currElement->x(),aabb.yMax},iteration));
        }else{
            lineData.push_back(std::pair<std::array<double,4>,int>(std::array<double,4>{aabb.xMin,currElement->y(),aabb.xMax,currElement->y()},iteration));
        }
        auto aabbs = aabb.split(iteration,currElement);
        iterateOverKDTree(currNode.leftChildId,aabbs.at(0),lineData,iteration+1);
        iterateOverKDTree(currNode.rightChildId,aabbs.at(1),lineData,iteration+1);
    }
    KDTreeFirstImpl *kdTree;
};


#endif // KD_TREE_H
