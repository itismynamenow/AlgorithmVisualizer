#ifndef QUAD_TREE_MODERATE_H
#define QUAD_TREE_MODERATE_H
#include <quad_tree.h>
#include <unordered_map>
#include <set>
#include <unordered_set>
template <class T>
class QuadTreeModerate;
template <class T>
class QuadTreeModerateVisualionHelper;

template <class T>
struct QuadTreeModerateNode{
    QuadTreeModerateNode(){
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


template <class T>
class QuadTreeModerate: public QuadTree<T>{

public:
    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef typename QuadTree<T>::ELEMENT_PTR ELEMENT_PTR;
    typedef std::unordered_map<int,vector<int>> MAP;
    typedef std::unordered_set<int> SET;

    friend class QuadTreeModerateVisualionHelper<T>;

public:
    QuadTreeModerate(){}
    QuadTreeModerate(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ~QuadTreeModerate(){}
    virtual void setElements(const ELEMENTS_PTR &inputElementsPtrs, const AABB<T> &boundingBox, int depth=6, int nodeCapacity=6){
        buildTree(inputElementsPtrs,boundingBox,depth,nodeCapacity);
    }
    virtual ELEMENTS_PTR getElementsThatOverlap(const AABB<T> &aabb) const override{
        ELEMENTS_PTR placeHolder;
        return placeHolder;}
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
        getAllOverlappingElementTuplesRecursively(overlappingTuples,rootId);
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
                    const vector<int> &elementsId,
                    const AABB<T> &boundingBox,
                    int levelRemaining,
                    int nodeCapacity)
    {
        int rootId = nodes.size();
        nodes.push_back(QuadTreeModerateNode<T>());
        boundingBoxes.push_back(boundingBox);
        if(elementsId.size() <= nodeCapacity || levelRemaining == 0){
            nodeIdToElementId[rootId] = elementsId;
        }else{
            const array<AABB<T>,4> boundingBoxes = boundingBox.split();
            auto pointsIdByQuadrant = splitElementsIdByQuadrant(elementsPtrs,elementsId,boundingBoxes);
            for(int i=0;i<4;i++){
                int childId = makeSubtree(elementsPtrs,pointsIdByQuadrant.at(i),boundingBoxes.at(i),levelRemaining-1,nodeCapacity);
                nodes.at(rootId).childrenId.at(i) = childId;
            }
        }
        return rootId;
    }

    array<vector<int>,4> splitElementsIdByQuadrant(const ELEMENTS_PTR &elementsPtrs, const vector<int> &elementsId,  const array<AABB<T>,4> &boundingBoxes) const{
            array<vector<int>,4> elementsIdByQuadrant;
            for(auto elementId: elementsId){
                for(int i=0;i<4;i++){
                    if(elementsPtrs.at(elementId)->aabb.doesOverlap(boundingBoxes.at(i))){
                        elementsIdByQuadrant.at(i).push_back(elementId);
                    }
                }
            }
            return elementsIdByQuadrant;
    }

    void getAllOverlappingElementTuplesRecursively(vector<tuple<ELEMENT_PTR,ELEMENT_PTR>> &tuples, int nodeId) const{
        if(nodeId != -1){
            auto node = nodes.at(nodeId);
            if(node.isLeaf()){
                vector<int> elementsId = nodeIdToElementId.at(nodeId);
                if(elementsId.size()>1){
                    for(int i=0;i<elementsId.size();i++){
                        for(int j=i+1;j<elementsId.size();j++){
                            if(elementsPtrs.at(elementsId.at(i))->doesOverlap(elementsPtrs.at(elementsId.at(j))->aabb)){
                                tuples.push_back(tuple<ELEMENT_PTR,ELEMENT_PTR>(elementsPtrs.at(elementsId.at(i)),elementsPtrs.at(elementsId.at(j))));
                            }
                        }
                    }
                }
            }else{
                for(int childId: node.childrenId){
                    getAllOverlappingElementTuplesRecursively(tuples,childId);
                }
            }
        }
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
                for(int childId: node.childrenId){
                    getAllOverlappingElementsRecursively(elementSet,childId);
                }
            }
        }
    }
    vector<QuadTreeModerateNode<T>> nodes;
    vector<AABB<T>> boundingBoxes;
    ELEMENTS_PTR elementsPtrs;
    MAP nodeIdToElementId;
    int rootId=-1;
    QuadTreeModerateVisualionHelper<T> visualisationHelper{this};
};

template <class T>
class QuadTreeModerateVisualionHelper: public QuadTreeVisualionHelper<T>{
public:
    QuadTreeModerateVisualionHelper(QuadTreeModerate<T>* quadTree): quadTree(quadTree){}
    virtual vector<AABB<T>> getNonLeafNodesBoundingBoxes() const override{
        vector<AABB<T>> boundingBoxes;
        getNonLeafNodesBoundingBoxesRecursivly(boundingBoxes,quadTree->rootId);
        return boundingBoxes;
    }
private:
    QuadTreeModerate<T>* quadTree;
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

#endif // QUAD_TREE_MODERATE_H
