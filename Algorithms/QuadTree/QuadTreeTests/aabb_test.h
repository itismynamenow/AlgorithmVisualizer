#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../QuadTree/quad_tree.h"

using namespace testing;

TEST(AABB, doesOverlap)
{
    AABB<int> aabb(0,0,10,10);
    EXPECT_TRUE(aabb.doesOverlap(AABB<int>(1,1,10,10)));
    EXPECT_TRUE(aabb.doesOverlap(AABB<int>(-1,-1,10,10)));
    EXPECT_TRUE(aabb.doesOverlap(AABB<int>(9,9,10,10)));
    EXPECT_TRUE(aabb.doesOverlap(AABB<int>(-1,-1,1,1)));
    EXPECT_TRUE(aabb.doesOverlap(AABB<int>(9,9,100,100)));

    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(1,100,10,101)));
    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(100,1,101,10)));
    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(-10,1,-1,10)));
    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(1,-10,10,-1)));

    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(-1,1,0,7)));
    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(1,-1,7,0)));
    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(-1,-1,0,0)));
    EXPECT_FALSE(aabb.doesOverlap(AABB<int>(10,10,111,111)));
}
TEST(AABB, isCompletlyInside){
    AABB<int> aabb(0,0,10,10);
    EXPECT_TRUE(aabb.isCompletlyInside(AABB<int>(-1,-1,100,100)));
    EXPECT_TRUE(aabb.isCompletlyInside(AABB<int>(0,0,10,10)));
    EXPECT_TRUE(aabb.isCompletlyInside(AABB<int>(-1,-1,10,10)));

    EXPECT_FALSE(aabb.isCompletlyInside(AABB<int>(1,1,100,100)));
    EXPECT_FALSE(aabb.isCompletlyInside(AABB<int>(1,-1,100,100)));
    EXPECT_FALSE(aabb.isCompletlyInside(AABB<int>(-1,1,100,100)));
    EXPECT_FALSE(aabb.isCompletlyInside(AABB<int>(-1,-1,10,9)));
    EXPECT_FALSE(aabb.isCompletlyInside(AABB<int>(-1,-1,9,10)));
}
TEST(AABB, split){
    AABB<int> aabb(-10,-20,10,20);
    auto results = aabb.split();
    //enum QUADRANT{LU=0,LB=1,RU=2,RB=3,CENTER=4};
    EXPECT_TRUE(results.at(0) == AABB<int>( 0,0,10,20));
    EXPECT_TRUE(results.at(1) == AABB<int>( 0,-20,10,0));
    EXPECT_TRUE(results.at(2) == AABB<int>(-10,0,0,20));
    EXPECT_TRUE(results.at(3) == AABB<int>(-10,-20,0,0));
}
TEST(AABB, translateBy){
    AABB<int> aabb0(0,0,10,10);
    AABB<int> aabb1(1,2,11,12);
    aabb0.translateBy(1,2);
    EXPECT_TRUE(aabb0 == aabb1);
}
TEST(AABB, equalsOperator){
    AABB<int> aabb0(0,0,10,10);
    AABB<int> aabb1(1,2,11,12);
    AABB<int> aabb2(1,2,11,12);

    EXPECT_TRUE(aabb1 == aabb2);

    EXPECT_FALSE(aabb0 == aabb2);
}
