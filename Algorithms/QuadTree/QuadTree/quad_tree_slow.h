#ifndef QUADTREESLOW_H
#define QUADTREESLOW_H
#include <set>
#include <quad_tree.h>

template <class T>
class QuadTreeSlowVisualionHelper;

template <class T>
struct QuadTreeSlowNode{

    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef shared_ptr<QuadTreeSlowNode<T>> NODE_SLOW_PTR;

    array<NODE_SLOW_PTR,4> children;
    ELEMENTS_PTR elementsPtr;
    AABB<T> boundingBox;
};

template <class T>
class QuadTreeSlow: public QuadTree<T>{

protected:
    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef typename QuadTree<T>::ELEMENT_PTR ELEMENT_PTR;
    typedef typename QuadTreeSlowNode<T>::NODE_SLOW_PTR NODE_SLOW_PTR;
    typedef bool(*ELEMENT_COMPARATOR)(const ELEMENT_PTR &x, const ELEMENT_PTR &y);
    typedef std::set<ELEMENT_PTR,ELEMENT_COMPARATOR> ELEMENT_SET;

    friend class QuadTreeSlowVisualionHelper<T>;

public:
    QuadTreeSlow(){}
    QuadTreeSlow(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6) override{
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) const override{
        ELEMENTS_PTR result;
        //TODO
        return result;
    }
    virtual ELEMENTS_PTR getAllOverlappingElements() const override{
        ELEMENT_COMPARATOR comparator = [](const ELEMENT_PTR &x, const ELEMENT_PTR &y){ return x.get() < y.get(); };
        ELEMENT_SET elementSet(comparator);
        getAllOverlappingElementsRecursively(elementSet,rootNode);
        return ELEMENTS_PTR(elementSet.begin(),elementSet.end());
    }
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementTuples() const override{
        vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> overlappingTuples;
        getAllOverlappingElementTuplesRecursively(overlappingTuples,rootNode);
        return overlappingTuples;
    }
    virtual void reset() override{
        rootNode.reset();
    }
    virtual const QuadTreeVisualionHelper<T> *getVisualisationHelper() const override{
        return &visualisationHelper;
    }

protected:
    virtual void buildTree(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth, int nodeCapacity){
        this->reset();
        rootNode = makeSubtree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    NODE_SLOW_PTR makeSubtree(const ELEMENTS_PTR &elementsPtrs, const AABB<T> &boundingBox, int levelRemaining, int nodeCapacity){
        if(elementsPtrs.size() == 0) return unique_ptr<QuadTreeSlowNode<T>>(nullptr);
        else{
            NODE_SLOW_PTR currentRootNode(new QuadTreeSlowNode<T>);
            currentRootNode->boundingBox = boundingBox;
            if(elementsPtrs.size() <= nodeCapacity || levelRemaining <= 0){
                currentRootNode->elementsPtr = elementsPtrs;
            }else{
                const array<AABB<T>,4> boundingBoxes = boundingBox.split();
                auto pointsByQuadrant = splitElementsPtrsByQuadrant(elementsPtrs,boundingBoxes);
                for(int i=0;i<4;i++){
                    currentRootNode->children.at(i) = makeSubtree(pointsByQuadrant.at(i),boundingBoxes.at(i),levelRemaining-1,nodeCapacity);
                }
            }
            return currentRootNode;
        }
    }
    inline array<ELEMENTS_PTR,4> splitElementsPtrsByQuadrant(const ELEMENTS_PTR &elementsPtrs,  const array<AABB<T>,4> &boundingBoxes) const{
        array<ELEMENTS_PTR,4> elementsPtrsByQuadrant;
        for(auto const &element: elementsPtrs){
            for(int i=0;i<4;i++){
                if(element->aabb.doesOverlap(boundingBoxes.at(i))){
                    elementsPtrsByQuadrant.at(i).push_back(element);
                }
            }
        }
        return elementsPtrsByQuadrant;
    }
    void getAllOverlappingElementTuplesRecursively(vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> &tuples, const NODE_SLOW_PTR &node) const{
        auto &elements = node->elementsPtr;
        if(node == nullptr){
            return;
        }
        else if(elements.size()>1){
            for(int i=0;i<elements.size();i++){
                for(int j=i+1;j<elements.size();j++){
                    if(elements.at(i)->doesOverlap(elements.at(j)->aabb)){
                        tuples.push_back(tuple<ELEMENT_PTR,ELEMENT_PTR>(elements.at(i),elements.at(j)));
                    }
                }
            }
        }
        else{
            for(auto const &child: node->children){
                getAllOverlappingElementTuplesRecursively(tuples,child);
            }
        }
    }

    void getAllOverlappingElementsRecursively(ELEMENT_SET &elementSet, const NODE_SLOW_PTR &node) const{
        auto &nodeElements = node->elementsPtr;
        if(node == nullptr){
            return;
        }
        else if(nodeElements.size()>1){
            for(int i=0;i<nodeElements.size();i++){
                for(int j=i+1;j<nodeElements.size();j++){
                    if(nodeElements.at(i)->doesOverlap(nodeElements.at(j)->aabb)){
                        elementSet.insert(nodeElements.at(i));
                        elementSet.insert(nodeElements.at(j));
                    }
                }
            }
        }
        else{
            for(auto const &child: node->children){
                getAllOverlappingElementsRecursively(elementSet,child);
            }
        }
    }

    shared_ptr<QuadTreeSlowNode<T>> rootNode;
    QuadTreeSlowVisualionHelper<T> visualisationHelper{this};
};

template <class T>
class QuadTreeSlowVisualionHelper: public QuadTreeVisualionHelper<T>{
public:
    QuadTreeSlowVisualionHelper(QuadTreeSlow<T> *quadTree):quadTree(quadTree){}
    virtual vector<AABB<T>> getNonLeafNodesBoundingBoxes() const{
        vector<AABB<T>> boundingBoxes;
        getNonLeafNodesBoundingBoxesRecursivly(boundingBoxes,quadTree->rootNode);
        return boundingBoxes;
    }
private:
     QuadTreeSlow<T> *quadTree;
     void getNonLeafNodesBoundingBoxesRecursivly(vector<AABB<T>> &boundingBoxes, shared_ptr<QuadTreeSlowNode<T>> node) const{
         if(node && node->elementsPtr.size()==0){
             boundingBoxes.push_back(node->boundingBox);
             for(auto &child: node->children){
                 getNonLeafNodesBoundingBoxesRecursivly(boundingBoxes,child);
             }
         }
     }
};

#endif // QUADTREESLOW_H
