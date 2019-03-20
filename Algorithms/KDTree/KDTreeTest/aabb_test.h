#ifndef AABB_TEST_H
#define AABB_TEST_H
#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../KDTree/kd_tree.h"

TEST(AABB,split){
    AABB aabb{2,4,10,20};
    KDTreeElement element{6,12};
    auto halfsByX = aabb.split(0,&element);
    EXPECT_TRUE(halfsByX.at(0)==AABB(2,4,6,20));
    EXPECT_TRUE(halfsByX.at(1)==AABB(6,4,10,20));
    auto halfsByY = aabb.split(1,&element);
    EXPECT_TRUE(halfsByY.at(0)==AABB(2,4,10,12));
    EXPECT_TRUE(halfsByY.at(1)==AABB(2,12,10,20));
}

TEST(AABB, minDistanceTo){
    AABB aabb{3,4,11,32};
    KDTreeElement el0 {0,0};
    EXPECT_NEAR(aabb.minDistanceTo(&el0),5,0.0001);
    KDTreeElement el1 {5,-100};
    EXPECT_NEAR(aabb.minDistanceTo(&el1),104,0.0001);
    KDTreeElement el2 {-100,5};
    EXPECT_NEAR(aabb.minDistanceTo(&el2),103,0.0001);
    KDTreeElement el3 {5,100};
    EXPECT_NEAR(aabb.minDistanceTo(&el3),68,0.0001);
    KDTreeElement el4 {100,5};
    EXPECT_NEAR(aabb.minDistanceTo(&el4),89,0.0001);
    KDTreeElement el5 {14,36};
    EXPECT_NEAR(aabb.minDistanceTo(&el5),5,0.0001);
    KDTreeElement el6 {0,36};
    EXPECT_NEAR(aabb.minDistanceTo(&el6),5,0.0001);
    KDTreeElement el7 {14,0};
    EXPECT_NEAR(aabb.minDistanceTo(&el7),5,0.0001);
}
#endif // AABB_TEST_H
