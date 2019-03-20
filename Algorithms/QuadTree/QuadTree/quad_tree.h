#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <tuple>
#include <ctime>
#include <type_traits>

using std::vector;
using std::array;
using std::shared_ptr;
using std::unique_ptr;
using std::tuple;

enum QUADRANT{LU=0,LB=1,RU=2,RB=3,CENTER=4};
template <class T>
class QuadTree;
template <class T>
struct QuadTreeElement;
template <class T>
class QuadTreeVisualionHelper;

//Axis aligned bounding box
template <class T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
struct AABB{
    AABB():xMin(0),yMin(0),xMax(0),yMax(0){}
    AABB(T xMin,T yMin,T xMax,T yMax):xMin(xMin),yMin(yMin),xMax(xMax),yMax(yMax){}
    T xMin,yMin,xMax,yMax;
    bool doesOverlap(const AABB &another)const{
        return this->xMax > another.xMin && another.xMax > this->xMin && this->yMax > another.yMin && another.yMax > this->yMin;
    }
    // Note: object is always inside itself for your purposes
    bool isCompletlyInside(const AABB &another)const{
        bool insideXRange = xMin>=another.xMin && xMax<=another.xMax;
        bool insideYRange = yMin>=another.yMin && yMax<=another.yMax;
        return insideXRange && insideYRange;
    }
    array<AABB<T>,4> split() const {
        T xCtr = xMin+(xMax - xMin)/2;
        T yCtr = yMin+(yMax - yMin)/2;
        return array<AABB<T>,4>{
            AABB<T>(xCtr,yCtr,xMax,yMax),
            AABB<T>(xCtr,yMin,xMax,yCtr),
            AABB<T>(xMin,yCtr,xCtr,yMax),
            AABB<T>(xMin,yMin,xCtr,yCtr)
        };
    }
    void translateBy(T dx,T dy){
        xMin+=dx;
        xMax+=dx;
        yMin+=dy;
        yMax+=dy;
    }
    bool operator ==(const AABB<T> &other) const{
        return xMin == other.xMin && yMin == other.yMin &&
               xMax == other.xMax && yMax == other.yMax;
    }
    void print(){
        std::cout<<"xMin = "<<xMin<<" yMin = "<<yMin<<" xMax = "<<xMax<<" yMax = "<<yMax<<std::endl;
    }
};

//Inherit this class for object to be used with quad tree
template <class T>
struct QuadTreeElement{

    //bool determines if shared or normal pointers will be used to keep data
    typedef QuadTreeElement<T>* ELEMENT_PTR;
    typedef QuadTreeElement<T>* Type;

    QuadTreeElement(){}
    QuadTreeElement(const AABB<T> &aabb):aabb(aabb){}
    QuadTreeElement(const QuadTreeElement &another){
        this->aabb = another.aabb;
    }
    QuadTreeElement(QuadTreeElement &&another){
        this->aabb = another.aabb;
    }
    virtual ~QuadTreeElement(){}
    virtual bool doesOverlap(const AABB<T> &another)const{
        return aabb.doesOverlap(another);
    }
    virtual bool doesOverlap(const  QuadTreeElement<T> &another)const{
        return aabb.doesOverlap(another.aabb);
    }
    static ELEMENT_PTR makeElement(const AABB<T> &aabb)
    {
      return new QuadTreeElement<T>(aabb);
    }
    template <class U>
    static U* dynamicCast(ELEMENT_PTR element)
    {
      return dynamic_cast<U*>(element);
    }
    AABB<T> aabb;
};

template <class T>
class QuadTree{

public:

    typedef typename QuadTreeElement<T>::ELEMENT_PTR ELEMENT_PTR;
    typedef vector<ELEMENT_PTR> ELEMENTS_PTR;

public:
    QuadTree(){}
    virtual ~QuadTree(){}
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6)=0;
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) const=0;
    virtual ELEMENTS_PTR getAllOverlappingElements() const=0;
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementTuples() const=0;
    virtual void reset()=0;
    virtual const QuadTreeVisualionHelper<T> *getVisualisationHelper() const=0;
};

template <class T>
class QuadTreeVisualionHelper{
public:
    virtual vector<AABB<T>> getNonLeafNodesBoundingBoxes() const=0;
};

#endif // QUADTREE_H
