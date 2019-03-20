#ifndef KD_TREE_TEST_H
#define KD_TREE_TEST_H
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../KDTree/kd_tree.h"

using namespace testing;
using std::cout;
using std::endl;

std::vector<KDTreeElement*> elementsToPtrs(std::vector<KDTreeElement> &elements){
    std::vector<KDTreeElement*> elementsPtrs;
    for(auto &element: elements){
        elementsPtrs.push_back(&element);
    }
    return elementsPtrs;
}

void printElements(std::vector<KDTreeElement> &elements){
    for(auto element: elements){
        cout<<element.x()<<" "<<element.y()<<endl;
    }
}

void printElements(std::vector<KDTreeElement*> &elements){
    for(auto element: elements){
        cout<<element->x()<<" "<<element->y()<<endl;
    }
}

bool compareElements(std::vector<KDTreeElement*> elements0,std::vector<KDTreeElement*> elements1){
    bool isSame = elements0.size() == elements1.size();
    for(int i=0;i<elements0.size();i++){
        if(*elements0.at(i) != *elements1.at(i)){
            isSame = false;
            break;
        }
    }
    return  isSame;
}

TEST(KDTreeFirstImpl,sortElements){
    KDTreeFirstImpl kdTree;
    std::vector<KDTreeElement> elements = {
                KDTreeElement {0.5,1},
                KDTreeElement {1  ,2},
                KDTreeElement {2  ,4},
                KDTreeElement {2  ,4},
                KDTreeElement {2  ,5},
                KDTreeElement {4  ,8},
                KDTreeElement {444444  ,888888}
            };
    std::vector<KDTreeElement> elementsShuffled = elements;
    std::random_shuffle(elementsShuffled.begin(), elementsShuffled.end());
    std::vector<KDTreeElement*> elementsShuffledPtr = elementsToPtrs(elementsShuffled);
    kdTree.sortElements(elementsShuffledPtr);
    EXPECT_TRUE(elementsShuffledPtr.size() == elements.size());
    EXPECT_TRUE(compareElements(elementsShuffledPtr,elementsToPtrs(elements)));
}

TEST(KDTreeFirstImpl,removeAndHashNonUniqueElements){
    KDTreeFirstImpl kdTree;
    std::vector<KDTreeElement> elements = {
                KDTreeElement {0.5,1},
                KDTreeElement {1  ,2},
                KDTreeElement {2  ,4},
                KDTreeElement {2  ,4},
                KDTreeElement {2  ,5},
                KDTreeElement {4  ,8},
                KDTreeElement {4  ,8},
                KDTreeElement {4  ,8},
                KDTreeElement {4  ,8},
                KDTreeElement {4  ,8},
                KDTreeElement {444444  ,888888}
            };
    std::vector<KDTreeElement> elementsUnique = {
                KDTreeElement {0.5,1},
                KDTreeElement {1  ,2},
                KDTreeElement {2  ,4},
                KDTreeElement {2  ,5},
                KDTreeElement {4  ,8},
                KDTreeElement {444444  ,888888}
            };
    auto elementsPtrs = elementsToPtrs(elements);
    kdTree.removeAndHashNonUniqueElements(elementsPtrs);
    EXPECT_EQ(elementsPtrs.size(),elementsUnique.size());
    EXPECT_TRUE(compareElements(elementsPtrs,elementsToPtrs(elementsUnique)));
    EXPECT_EQ(kdTree.sameValueElements.size(),2);
    for(auto &pair: kdTree.sameValueElements){
        if(*pair.first == KDTreeElement {2  ,4}){
            EXPECT_EQ(pair.second.size(),2);
        }else if(*pair.first == KDTreeElement {4  ,8}){
            EXPECT_EQ(pair.second.size(),5);
        }else{
            FAIL();
        }
    }
    std::vector<KDTreeElement*> elementsPtrsEmpty;
    kdTree.removeAndHashNonUniqueElements(elementsPtrsEmpty);
    EXPECT_EQ(elementsPtrsEmpty.size(),0);
}

TEST(KDTreeFirstImpl,buildTree){
    KDTreeFirstImpl kdTree;
    std::vector<KDTreeElement> elements = {
                KDTreeElement {0.5,1},
                KDTreeElement {1  ,2},
                KDTreeElement {2  ,4},
                KDTreeElement {3  ,5},
                KDTreeElement {4  ,8},
                KDTreeElement {444444  ,888888}
            };
    auto elementsPtrs = elementsToPtrs(elements);
    auto rootId = kdTree.buildTree(elementsPtrs.begin(),elementsPtrs.end());
    auto &nodes = kdTree.nodes;
    EXPECT_EQ(nodes.size(), elements.size());
    auto rootNode = nodes.at(rootId);
    EXPECT_EQ(*rootNode.element,KDTreeElement (3 ,5));
    auto rootLeft = nodes.at(rootNode.leftChildId);
    EXPECT_EQ(*rootLeft.element,KDTreeElement (1 ,2));
    auto rootLeftLeft = nodes.at(rootLeft.leftChildId);
    EXPECT_EQ(*rootLeftLeft.element,KDTreeElement (0.5 ,1));
    auto rootLeftRight = nodes.at(rootLeft.rightChildId);
    EXPECT_EQ(*rootLeftRight.element,KDTreeElement (2 ,4));
    auto rootRight = nodes.at(rootNode.rightChildId);
    EXPECT_EQ(*rootRight.element,KDTreeElement (444444  ,888888));
    auto rootRightLeft = nodes.at(rootRight.leftChildId);
    EXPECT_EQ(*rootRightLeft.element,KDTreeElement (4  ,8 ));

}

class KDTreeFirstImplFiveElementsOnSameLine: public  ::testing::Test {
    protected:
    void SetUp() override {
        kdTree = new KDTreeFirstImpl();
        elements = {
            KDTreeElement {444444  ,888888},
            KDTreeElement {444444  ,888888},
            KDTreeElement {444444  ,888888},
            KDTreeElement {0.5,1},
            KDTreeElement {1  ,2},
            KDTreeElement {2  ,4},
            KDTreeElement {4  ,8},
            KDTreeElement {444444  ,888888}
        };
        for(auto &element: elements){
            elementsPtr.push_back(&element);
        }
    }
    void TearDown() override {
        delete  kdTree;
    }
    KDTree *kdTree;
    std::vector<KDTreeElement*> elementsPtr;
private:
    std::vector<KDTreeElement> elements;
};

TEST_F(KDTreeFirstImplFiveElementsOnSameLine,tElementsClosestTo)
{
    kdTree->setNewElements(elementsPtr);

    KDTreeElement referenceElement {4.5,9};
    auto closestElements = kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (4 ,8));

    referenceElement = KDTreeElement(0,0);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (0.5,1));

    referenceElement = KDTreeElement(1,2);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (1,2));

    referenceElement = KDTreeElement(1,2.1);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (1,2));

    referenceElement = KDTreeElement(3.1,6.1);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (4,8));

    referenceElement = KDTreeElement(2.9,5.9);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (2,4));

    referenceElement = KDTreeElement(4444,8888);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (4,8));

    referenceElement = KDTreeElement(5555,9999);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (4,8));

    referenceElement = KDTreeElement(-4444,-8888);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (0.5,1));

    referenceElement = KDTreeElement(-3333,-7777);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (0.5,1));

    referenceElement = KDTreeElement(444449  ,888889);
    closestElements= kdTree->getElementsClosestTo(&referenceElement);
    EXPECT_TRUE(*closestElements.at(0) == KDTreeElement (444444  ,888888));
    EXPECT_TRUE(closestElements.size() == 4);
    for(auto element: closestElements){
        EXPECT_TRUE(*element == KDTreeElement (444444  ,888888));
    }
}


TEST(KDTreeFirstImplTest, ZeroElements)
{
    KDTree *kdTree = new KDTreeFirstImpl();
    KDTreeElement referenceElement {4.5,9};
    EXPECT_TRUE(kdTree->getElementsClosestTo(&referenceElement).size()==0);

    delete kdTree;
}

TEST(KDTreeFirstImplTest, Clear)
{
    KDTree *kdTree = new KDTreeFirstImpl();
    std::vector<KDTreeElement> elements{
        KDTreeElement {0.5,1},
        KDTreeElement {1  ,2},
        KDTreeElement {2  ,4},
        KDTreeElement {4  ,8},
        KDTreeElement {444444  ,888888}
    };
    std::vector<KDTreeElement*> elementsPtr;
    for(auto &element: elements){
        elementsPtr.push_back(&element);
    }
    kdTree->setNewElements(elementsPtr);


    KDTreeElement referenceElement {4.5,9};

    kdTree->clear();

    EXPECT_TRUE(kdTree->getElementsClosestTo(&referenceElement).size() == 0);
}
#endif //KD_TREE_TEST_H
