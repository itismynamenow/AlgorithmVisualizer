#include <gtest/gtest.h>

#include "aabb_test.h"
#include "quad_tree_test.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
