#ifndef QUAD_TREE_TEST_H
#define QUAD_TREE_TEST_H
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../QuadTree/quad_tree.h"
#include "../QuadTree/quad_tree_fast.h"

template <typename T>
class QuadTreeTest : public ::testing::Test {};
using MyTypes = ::testing::Types<int, unsigned int,double>;
TYPED_TEST_SUITE(QuadTreeTest, MyTypes);
TYPED_TEST(QuadTreeTest, constructor0Arg){
    using EL = QuadTreeElement<TypeParam>;
    std::vector<EL*> vec0EL;
    auto quadTree = new QuadTreeFast<TypeParam>(vec0EL, AABB<TypeParam>(0,0,100,100));
    EXPECT_TRUE(quadTree->getAllOverlappingElements().size()==0);
}

TYPED_TEST(QuadTreeTest, constructor1Arg){
    using EL = QuadTreeElement<TypeParam>;
    EL element0 (AABB<TypeParam>(0,0,1,1));
    std::vector<EL*> vec1EL {&element0};
    auto quadTree = new QuadTreeFast<TypeParam>(vec1EL, AABB<TypeParam>(0,0,100,100));
    EXPECT_TRUE(quadTree->getAllOverlappingElements().size()==0);
}

TYPED_TEST(QuadTreeTest, constructor2Arg){
    using EL = QuadTreeElement<TypeParam>;
    EL element0(AABB<TypeParam>(0,0,10,10));
    EL element1(AABB<TypeParam>(0,0,11,11));
    std::vector<EL*> vec2EL {&element0,&element1};
    auto quadTree = new QuadTreeFast<TypeParam>(vec2EL, AABB<TypeParam>(0,0,100,100));
    auto overlappingElements = quadTree->getAllOverlappingElements();
    ASSERT_TRUE(overlappingElements.size()==2);
    EXPECT_TRUE(overlappingElements.at(0)->aabb == element0.aabb || overlappingElements.at(0)->aabb == element1.aabb);
    EXPECT_TRUE(overlappingElements.at(1)->aabb == element0.aabb || overlappingElements.at(1)->aabb == element1.aabb);
    EXPECT_FALSE(overlappingElements.at(0)->aabb == overlappingElements.at(1)->aabb);
}

TYPED_TEST(QuadTreeTest, constructorSame6Arg){
    using EL = QuadTreeElement<TypeParam>;
    std::vector<EL> vecEL {
    EL(AABB<TypeParam>(0  ,  0,  1,  1)),
    EL(AABB<TypeParam>(0  ,  0,  1,  1)),
    EL(AABB<TypeParam>(0  ,  0,  1,  1)),
    EL(AABB<TypeParam>(0  ,  0,  1,  1)),
    EL(AABB<TypeParam>(0  ,  0,  1,  1)),
    EL(AABB<TypeParam>(0  ,  0,  1,  1))};
    std::vector<EL*> vecELptrs;
    for(auto &el: vecEL){
        vecELptrs.push_back(&el);
    }
    auto quadTree = new QuadTreeFast<TypeParam>(vecELptrs, AABB<TypeParam>(0,0,100,100));
    EXPECT_TRUE(quadTree->getAllOverlappingElements().size()==6);
}

TYPED_TEST(QuadTreeTest, constructor6noOverlapArg){
    using EL = QuadTreeElement<TypeParam>;
    std::vector<EL> vecEL {
    EL(AABB<TypeParam>(0  ,  0,  1,  1)),
    EL(AABB<TypeParam>(2  ,  2,  3,  3)),
    EL(AABB<TypeParam>(4  ,  4,  5,  5)),
    EL(AABB<TypeParam>(6  ,  6,  7,  7)),
    EL(AABB<TypeParam>(8  ,  8,  9,  9)),
    EL(AABB<TypeParam>(10 , 10, 11, 11))};
    std::vector<EL*> vecELptrs;
    for(auto &el: vecEL){
        vecELptrs.push_back(&el);
    }
    auto quadTree = new QuadTreeFast<TypeParam>(vecELptrs, AABB<TypeParam>(0,0,100,100));
    EXPECT_TRUE(quadTree->getAllOverlappingElements().size()==0);
}

TYPED_TEST(QuadTreeTest, setElements0Arg){
    using EL = QuadTreeElement<TypeParam>;
    std::vector<EL*> vec0EL;
    auto quadTree = new QuadTreeFast<TypeParam>();
    quadTree->setElements(vec0EL, AABB<TypeParam>(0,0,100,100));
    EXPECT_TRUE(quadTree->getAllOverlappingElements().size()==0);
}

TYPED_TEST(QuadTreeTest, setElements1Arg){
    using EL = QuadTreeElement<TypeParam>;
    EL element0 (AABB<TypeParam>(0,0,1,1));
    std::vector<EL*> vec1EL {&element0};
    auto quadTree = new QuadTreeFast<TypeParam>();
    quadTree->setElements(vec1EL, AABB<TypeParam>(0,0,100,100));
    EXPECT_TRUE(quadTree->getAllOverlappingElements().size()==0);
}

TYPED_TEST(QuadTreeTest, setElements2Arg){
    using EL = QuadTreeElement<TypeParam>;
    EL element0(AABB<TypeParam>(0,0,10,10));
    EL element1(AABB<TypeParam>(0,0,11,11));
    std::vector<EL*> vec2EL {&element0,&element1};
    auto quadTree = new QuadTreeFast<TypeParam>();
    quadTree->setElements(vec2EL, AABB<TypeParam>(0,0,100,100));
    auto overlappingElements = quadTree->getAllOverlappingElements();
    ASSERT_TRUE(overlappingElements.size()==2);
    EXPECT_TRUE(overlappingElements.at(0)->aabb == element0.aabb || overlappingElements.at(0)->aabb == element1.aabb);
    EXPECT_TRUE(overlappingElements.at(1)->aabb == element0.aabb || overlappingElements.at(1)->aabb == element1.aabb);
    EXPECT_FALSE(overlappingElements.at(0)->aabb == overlappingElements.at(1)->aabb);}


#endif // QUAD_TREE_TEST_H
