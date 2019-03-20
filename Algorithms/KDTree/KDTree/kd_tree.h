#ifndef KD_TREE_H
#define KD_TREE_H
#include <vector>
#include <unordered_map>

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
        if(first != std::prev(last)){
            auto comparator = [iteration](const KDTreeElement* a,const KDTreeElement* b){
                if(iteration%2 == 0) return a->x() < b->x();
                else return a->y() < b->y();
            };
            std::nth_element(first,middle,last,comparator);
            int leftChildId = buildTree(first,middle,iteration+1);
            int rightChildId = buildTree(std::next(middle),last,iteration+1);
            nodes.at(nodeId).setElement(*middle);
            nodes.at(nodeId).setChildren(leftChildId,rightChildId);
        }
        return nodeId;
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
