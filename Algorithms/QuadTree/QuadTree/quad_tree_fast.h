#ifndef QUAD_TREE_FAST_H
#define QUAD_TREE_FAST_H
#include "quad_tree.h"
#include <unordered_map>
#include <set>
#include <unordered_set>
template <class T>
class QuadTreeFast;
template <class T>
class QuadTreeFastVisualionHelper;

template <class T>
struct QuadTreeFastNode{
    QuadTreeFastNode(){
        for(int i=0;i<4;i++){
            childrenId.at(i) = -1;
        }
    }
    bool isLeaf() const{
        return  childrenId[0]==-1 && childrenId[1]==-1 &&
                childrenId[2]==-1 && childrenId[3]==-1;
    }
    array<int,4> childrenId;
};
/*
 * The main difference from moderate quad tree is the fact that elements are stored not only in leafs
 * but in ordinary node if given element overlaps node's bounding box completly
 */
template <class T>
class QuadTreeFast: public QuadTree<T>{

public:

    typedef typename QuadTreeElement<T>::ELEMENT_PTR ELEMENT_PTR;
    typedef vector<ELEMENT_PTR> ELEMENTS_PTR;
    typedef std::unordered_map<int,vector<int>> MAP;
    typedef std::unordered_set<int> SET;

    friend class QuadTreeFastVisualionHelper<T>;

public:
    QuadTreeFast(){}
    QuadTreeFast(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ~QuadTreeFast(){}
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) const override{
        ELEMENTS_PTR placeHolder;
        return placeHolder;
    }
    virtual ELEMENTS_PTR getAllOverlappingElements() const override{
        SET elementIdSet;
        getAllOverlappingElementsRecursively(elementIdSet,rootId);
        ELEMENTS_PTR overlappingElementsPtrs(elementIdSet.size());
        int i=0;
        for(auto id: elementIdSet){
            overlappingElementsPtrs.at(i) = elementsPtrs.at(id);
            i++;
        }

        return overlappingElementsPtrs;
    }
    virtual vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> getAllOverlappingElementTuples() const override{
        vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> overlappingTuples;
        getAllOverlappingElementTuplesRecursively(overlappingTuples,vector<int>(),rootId);
        return overlappingTuples;
    }
    virtual void reset(){
        nodes.clear();
        elementsPtrs.clear();
        nodeIdToElementId.clear();
        boundingBoxes.clear();
        rootId=-1;
    }
    virtual const QuadTreeVisualionHelper<T> *getVisualisationHelper() const override{
        return &visualisationHelper;
    }

protected:
    virtual void buildTree(const ELEMENTS_PTR &inputElementsPtrs,
                           const AABB<T> &boundingBox,
                           int depth,
                           int nodeCapacity){
        reset();
        elementsPtrs = inputElementsPtrs;
        vector<int> elementsId(inputElementsPtrs.size());
        for(int i=0;i<inputElementsPtrs.size();i++){
            elementsId.at(i) = i;
        }
        rootId = makeSubtree(inputElementsPtrs,elementsId,boundingBox,depth,nodeCapacity);
    }
    int makeSubtree(const ELEMENTS_PTR &elementsPtrs,
                    vector<int> &elementsId,
                    const AABB<T> &boundingBox,
                    int levelRemaining,
                    int nodeCapacity)
    {
        int rootId = nodes.size();
        nodes.push_back(std::move(QuadTreeFastNode<T>()));
        boundingBoxes.push_back(boundingBox);
        if(elementsId.size() <= nodeCapacity || levelRemaining == 0){
//            nodeIdToElementId[rootId] = elementsId;
            nodeIdToElementId.insert(std::make_pair(rootId,std::move(elementsId)));
        }else{
            const array<AABB<T>,4> boundingBoxes = boundingBox.split();
            auto pointsIdByQuadrant = splitElementsIdByQuadrant(elementsPtrs,elementsId,boundingBox,boundingBoxes);
            elementsId.clear();
//            nodeIdToElementId[rootId] = pointsIdByQuadrant.at(QUADRANT::CENTER);
            nodeIdToElementId.insert(std::make_pair(rootId,std::move(pointsIdByQuadrant.at(QUADRANT::CENTER))));
            for(int i=0;i<4;i++){
                int childId = makeSubtree(elementsPtrs,pointsIdByQuadrant.at(i),boundingBoxes.at(i),levelRemaining-1,nodeCapacity);
                nodes.at(rootId).childrenId.at(i) = childId;
            }
        }
        return rootId;
    }

    array<vector<int>,5> splitElementsIdByQuadrant(const ELEMENTS_PTR &elementsPtrs, const vector<int> &elementsId, const AABB<T> &boundingBox,  const array<AABB<T>,4> &boundingBoxes) const{
            array<vector<int>,5> elementsIdByQuadrant;
            for(auto elementId: elementsId){
                if(boundingBox.isCompletlyInside(elementsPtrs.at(elementId)->aabb)){
                    elementsIdByQuadrant.at(QUADRANT::CENTER).push_back(elementId);
                }else{
                    for(int i=0;i<4;i++){
                        if(elementsPtrs.at(elementId)->aabb.doesOverlap(boundingBoxes.at(i))){
                            elementsIdByQuadrant.at(i).push_back(elementId);
                        }
                    }
                }
            }
            return elementsIdByQuadrant;
    }

    void getAllOverlappingElementsRecursively(SET &elementSet, int nodeId) const{
        if(nodeId != -1){
            auto node = nodes.at(nodeId);
            if(node.isLeaf()){
                vector<int> elementsId = nodeIdToElementId.at(nodeId);
                if(elementsId.size()>1){
                    for(int i=0;i<elementsId.size();i++){
                        for(int j=i+1;j<elementsId.size();j++){
                            if(elementsPtrs.at(elementsId.at(i))->doesOverlap(elementsPtrs.at(elementsId.at(j))->aabb)){
                                elementSet.insert(elementsId.at(i));
                                elementSet.insert(elementsId.at(j));
                            }
                        }
                    }
                }
            }else{
                vector<int> elementsId = nodeIdToElementId.at(nodeId);
                for(int i=0;i<elementsId.size();i++){
                    elementSet.insert(elementsId.at(i));
                }
                for(int childId: node.childrenId){
                    getAllOverlappingElementsRecursively(elementSet,childId);
                }
            }
        }
    }

    void getAllOverlappingElementTuplesRecursively(vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> &tuples,const vector<int> &elementsIdUpperNode, int nodeId) const{
        if(nodeId != -1){
            auto node = nodes.at(nodeId);
            if(node.isLeaf()){
                vector<int> elementsId = nodeIdToElementId.at(nodeId);
                if(elementsId.size()>1){
                    for(int i=0;i<elementsId.size();i++){
                        for(int j=i+1;j<elementsId.size();j++){
                            if(elementsPtrs.at(elementsId.at(i))->doesOverlap(elementsPtrs.at(elementsId.at(j))->aabb)){
                                tuples.push_back(tuple<ELEMENT_PTR,ELEMENT_PTR>(
                                                     elementsPtrs.at(elementsId.at(i)),
                                                     elementsPtrs.at(elementsId.at(j))));
                            }
                        }
                    }
                }
                //all elementsIdUpperNode intersect entire bounding box and all elements of current node
                for(int i=0;i<elementsId.size();i++){
                    for(int j=0;j<elementsIdUpperNode.size();j++){
                        tuples.push_back(tuple<ELEMENT_PTR,ELEMENT_PTR>(
                                             elementsPtrs.at(elementsId.at(i)),
                                             elementsPtrs.at(elementsIdUpperNode.at(j))));
                    }
                }
            }else{
                vector<int> elementsId = nodeIdToElementId.at(nodeId);
                if(elementsId.size()>1){
                    for(int i=0;i<elementsId.size();i++){
                        for(int j=i+1;j<elementsId.size();j++){
                            tuples.push_back(tuple<ELEMENT_PTR,ELEMENT_PTR>(elementsPtrs.at(elementsId.at(i)),elementsPtrs.at(elementsId.at(j))));
                        }
                    }
                }
                elementsId.insert(elementsId.end(),elementsIdUpperNode.begin(),elementsIdUpperNode.end());
                for(int childId: node.childrenId){
                    getAllOverlappingElementTuplesRecursively(tuples,elementsId,childId);
                }
            }
        }
    }

    vector<QuadTreeFastNode<T>> nodes;
    vector<AABB<T>> boundingBoxes;
    ELEMENTS_PTR elementsPtrs;
    MAP nodeIdToElementId;
    int rootId=-1;
    QuadTreeFastVisualionHelper<T> visualisationHelper{this};

};

template <class T>
class QuadTreeFastVisualionHelper: public QuadTreeVisualionHelper<T>{
public:
    QuadTreeFastVisualionHelper(QuadTreeFast<T>* quadTree): quadTree(quadTree){}
    virtual vector<AABB<T>> getNonLeafNodesBoundingBoxes() const override{
        vector<AABB<T>> boundingBoxes;
        getNonLeafNodesBoundingBoxesRecursivly(boundingBoxes,quadTree->rootId);
        return boundingBoxes;
    }
private:
    QuadTreeFast<T>* quadTree;
    void getNonLeafNodesBoundingBoxesRecursivly(vector<AABB<T>> &boundingBoxes, int nodeId) const{
        if(nodeId != -1){
            auto node = quadTree->nodes.at(nodeId);
            if(!node.isLeaf()){
                boundingBoxes.push_back(quadTree->boundingBoxes.at(nodeId));
                for(int childId: node.childrenId){
                    getNonLeafNodesBoundingBoxesRecursivly(boundingBoxes,childId);
                }
            }
        }
    }
};

#endif // QUAD_TREE_FAST_H
