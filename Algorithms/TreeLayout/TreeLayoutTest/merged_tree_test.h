#ifndef MERGED_TREE_TEST_H
#define MERGED_TREE_TEST_H
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../TreeLayout/tree_layout.h"

using namespace testing;

TEST(MergedTree, add)
{
/*
 * lvl0         tr0
 *               |
 *           _________
 *           |   |   |
 * lvl1     tr1 tr2 tr3
 *
 * xCoords:  0   1   2   3
 */
    TreeNode tr0;
    MergedTree mt0{&tr0};
    MergedTree mt1{&tr0};
    MergedTree mt2{&tr0};
    MergedTree mt3{&tr0};
    mt0.add(&mt1);
    mt0.add(&mt2);
    mt0.add(&mt3);

    EXPECT_EQ(mt0.levels.size(),2);
    EXPECT_FLOAT_EQ(mt0.levels.at(0).at(0), 1);
    EXPECT_FLOAT_EQ(mt0.levels.at(0).at(1), 2);
    EXPECT_FLOAT_EQ(mt0.levels.at(1).at(0), 0);
    EXPECT_FLOAT_EQ(mt0.levels.at(1).at(1), 3);
/*
 * lvl0       tr4
 *             |
 *           _____
 *           |   |
 * lvl1     tr5 tr6
 *
 * xCoords:  0   1   2
 */
    MergedTree mt4{&tr0};
    MergedTree mt5{&tr0};
    MergedTree mt6{&tr0};

    mt4.add(&mt5);
    mt4.add(&mt6);

    EXPECT_EQ(mt4.levels.size(),2);
    EXPECT_FLOAT_EQ(mt4.levels.at(0).at(0), 0.5);
    EXPECT_FLOAT_EQ(mt4.levels.at(0).at(1), 1.5);
    EXPECT_FLOAT_EQ(mt4.levels.at(1).at(0), 0);
    EXPECT_FLOAT_EQ(mt4.levels.at(1).at(1), 2);
/*
 *
 * lvl0         tr7
 *               |
 *               _
 *               |
 * lvl1         tr0
 *               |
 *           _________
 *           |   |   |
 * lvl2     tr1 tr2 tr3
 *
 * xCoords:  0   1   2  3
 */
    MergedTree mt7{&tr0};

    mt7.add(&mt0);

    EXPECT_EQ(mt7.levels.size(),3);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(0), 1);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(1), 2);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(1), 2);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(1), 3);
/*
 *
 * lvl0              tr7
 *                    |
 *               ___________
 *               |         |
 * lvl1         tr0       tr4
 *               |         |
 *           _________   _____
 *           |   |   |   |   |
 * lvl2     tr1 tr2 tr3 tr5 tr6
 *
 * xCoords:  0   1   2   3   4   5
 */
    mt7.add(&mt4);

    EXPECT_EQ(mt7.levels.size(),3);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(0), 2.25);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(1), 3.25);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(1), 4.5);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(1), 5);
/*
 *
 * lvl0                   tr7
 *                         |
 *               _____________________
 *               |         |         |
 * lvl1         tr0       tr4       tr8
 *               |         |
 *           _________   _____
 *           |   |   |   |   |
 * lvl2     tr1 tr2 tr3 tr5 tr6
 *
 * xCoords:  0   1   2   3   4   5   6   7
 */

    MergedTree mt8{&tr0};

    mt7.add(&mt8);

    EXPECT_EQ(mt7.levels.size(),3);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(0), 3.5);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(1), 4.5);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(1), 7);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(1), 5);
/*
 *
 * lvl0                                      tr7
 *                                            |
 *                 ______________________________________________________
 *                 |                 |                 |                 |
 * lvl1           tr0               tr4               tr8               tr9
 *                 |                 |                                   |
 *           _____________        _______        _________________________________________________
 *           |     |     |        |     |        |     |     |     |     |     |     |     |     |
 * lvl2     tr1   tr2   tr3      tr5   tr6      tr11  tr12  tr13  tr14  tr15  tr16  tr17  tr18  tr19
 *
 * xCoords:  0     1     2     3     4     5     6     7     8     9     10    11    12    13    14
 */

    MergedTree mt9{&tr0};

    MergedTree mt11{&tr0};
    MergedTree mt12{&tr0};
    MergedTree mt13{&tr0};
    MergedTree mt14{&tr0};
    MergedTree mt15{&tr0};
    MergedTree mt16{&tr0};
    MergedTree mt17{&tr0};
    MergedTree mt18{&tr0};
    MergedTree mt19{&tr0};

    mt9.add(&mt11);
    mt9.add(&mt12);
    mt9.add(&mt13);
    mt9.add(&mt14);
    mt9.add(&mt15);
    mt9.add(&mt16);
    mt9.add(&mt17);
    mt9.add(&mt18);
    mt9.add(&mt19);

    mt7.add(&mt9);

    EXPECT_EQ(mt7.levels.size(),3);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(0), 5.5);
    EXPECT_FLOAT_EQ(mt7.levels.at(0).at(1), 6.5);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt7.levels.at(1).at(1), 11);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt7.levels.at(2).at(1), 15);
}
#endif // MERGED_TREE_TEST_H
