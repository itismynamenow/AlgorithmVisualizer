#ifndef MERGED_TREE_TEST_H
#define MERGED_TREE_TEST_H
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "../TreeLayout/tree_layout.h"

using namespace testing;

TEST(MergedTree, add)
{
/*
 * lvl0         mr0
 *               |
 *           _________
 *           |   |   |
 * lvl1     mr1 mr2 mr3
 *
 * xCoords:  0   1   2   3
 */
    TreeLayoutNode tr0;
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
 * lvl0       mr4
 *             |
 *           _____
 *           |   |
 * lvl1     mr5 mr6
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
 * lvl0         mr7
 *               |
 *               _
 *               |
 * lvl1         mr0
 *               |
 *           _________
 *           |   |   |
 * lvl2     mr1 mr2 mr3
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
 * lvl0              mr7
 *                    |
 *               ___________
 *               |         |
 * lvl1         mr0       mr4
 *               |         |
 *           _________   _____
 *           |   |   |   |   |
 * lvl2     mr1 mr2 mr3 mr5 mr6
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
 * lvl0                   mr7
 *                         |
 *               _____________________
 *               |         |         |
 * lvl1         mr0       mr4       mr8
 *               |         |
 *           _________   _____
 *           |   |   |   |   |
 * lvl2     mr1 mr2 mr3 mr5 mr6
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
 * lvl0                                      mr7
 *                                            |
 *                 ______________________________________________________
 *                 |                 |                 |                 |
 * lvl1           mr0               mr4               mr8               mr9
 *                 |                 |                                   |
 *           _____________        _______        _________________________________________________
 *           |     |     |        |     |        |     |     |     |     |     |     |     |     |
 * lvl2     mr1   mr2   mr3      mr5   mr6      mr11  mr12  mr13  mr14  mr15  mr16  mr17  mr18  mr19
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

/*
 *
 * lvl0                   mt25
 *                         |
 *                 +-------+------+
 *                 |              |
 * lvl1           mt24           mt26
 *                 |              |
 *           +-----+-----+     +-----+
 *           |     |     |     |     |
 * lvl2     mt21  mt22  mt23  mt27  mt28
 *           |                 |
 *           +     +-----+-----+-----+-----+
 *           |     |     |     |     |     |
 * lvl3     mt20 mt29  mt30  mt31  mt32   mt33
 *
 * xCoords:  0     1     2     3     4     5
 */
    MergedTree mt20{&tr0};
    MergedTree mt21{&tr0};
    MergedTree mt22{&tr0};
    MergedTree mt23{&tr0};
    MergedTree mt24{&tr0};
    MergedTree mt25{&tr0};
    MergedTree mt26{&tr0};
    MergedTree mt27{&tr0};
    MergedTree mt28{&tr0};
    MergedTree mt29{&tr0};
    MergedTree mt30{&tr0};
    MergedTree mt31{&tr0};
    MergedTree mt32{&tr0};
    MergedTree mt33{&tr0};

    mt21.add(&mt20);

    mt24.add(&mt21);
    mt24.add(&mt22);
    mt24.add(&mt23);

    mt27.add(&mt29);
    mt27.add(&mt30);
    mt27.add(&mt31);
    mt27.add(&mt32);
    mt27.add(&mt33);

    mt26.add(&mt27);
    mt26.add(&mt28);

    mt25.add(&mt24);
    mt25.add(&mt26);

    EXPECT_EQ(mt25.levels.size(),4);
    EXPECT_FLOAT_EQ(mt25.levels.at(0).at(0), 2.25);
    EXPECT_FLOAT_EQ(mt25.levels.at(0).at(1), 3.25);
    EXPECT_FLOAT_EQ(mt25.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt25.levels.at(1).at(1), 4.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt25.levels.at(2).at(1), 5);
    EXPECT_FLOAT_EQ(mt25.levels.at(3).at(0), 0);
    EXPECT_FLOAT_EQ(mt25.levels.at(3).at(1), 6);
/*
 *
 * lvl0                          mt25
 *                                |
 *                 +--------------+--------------+
 *                 |              |              |
 * lvl1           mt24           mt26           mt35
 *                 |              |              |
 *           +-----+-----+     +-----+        +-----+
 *           |     |     |     |     |        |     |
 * lvl2     mt21  mt22  mt23  mt27  mt28     mt34  mt36
 *           |                 |                    |
 *           +     +-----+-----+-----+-----+        |
 *           |     |     |     |     |     |        |
 * lvl3     mt20 mt29  mt30  mt31  mt32   mt33     mt37
 *
 * xCoords:  0     1     2     3     4     5     6     7
 */

    MergedTree mt34{&tr0};
    MergedTree mt35{&tr0};
    MergedTree mt36{&tr0};
    MergedTree mt37{&tr0};

    mt36.add(&mt37);

    mt35.add(&mt34);
    mt35.add(&mt36);

    mt25.add(&mt35);

    EXPECT_EQ(mt25.levels.size(),4);
    EXPECT_FLOAT_EQ(mt25.levels.at(0).at(0), 3.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(0).at(1), 4.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt25.levels.at(1).at(1), 7);
    EXPECT_FLOAT_EQ(mt25.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt25.levels.at(2).at(1), 7.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(3).at(0), 0);
    EXPECT_FLOAT_EQ(mt25.levels.at(3).at(1), 7.5);

/*
 *
 * lvl0                                 mt25
 *                                       |
 *                 +--------------+--------------+--------------+
 *                 |              |              |              |
 * lvl1           mt24           mt26           mt35           mt38
 *                 |              |              |              |
 *           +-----+-----+     +-----+        +-----+           |
 *           |     |     |     |     |        |     |           |
 * lvl2     mt21  mt22  mt23  mt27  mt28     mt34  mt36        mt39
 *           |                 |                    |           |
 *           +     +-----+-----+-----+-----+        |           |
 *           |     |     |     |     |     |        |           |
 * lvl3     mt20 mt29  mt30  mt31  mt32   mt33     mt37        mt40
 *                                                              |
 *                                                              |
 *                                                              |
 * lvl4                                                        mt41
 *
 * xCoords:  0     1     2     3     4     5     6     7     8     9     10
 */
    MergedTree mt38{&tr0};
    MergedTree mt39{&tr0};
    MergedTree mt40{&tr0};
    MergedTree mt41{&tr0};

    mt40.add(&mt41);
    mt39.add(&mt40);
    mt38.add(&mt39);

    mt25.add(&mt38);

    EXPECT_EQ(mt25.levels.size(),5);
    EXPECT_FLOAT_EQ(mt25.levels.at(0).at(0), 4.75);
    EXPECT_FLOAT_EQ(mt25.levels.at(0).at(1), 5.75);
    EXPECT_FLOAT_EQ(mt25.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt25.levels.at(1).at(1), 9.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt25.levels.at(2).at(1), 9.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(3).at(0), 0);
    EXPECT_FLOAT_EQ(mt25.levels.at(3).at(1), 9.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(4).at(0), 8.5);
    EXPECT_FLOAT_EQ(mt25.levels.at(4).at(1), 9.5);
/*
 *
 * lvl0                                      mt42
 *                                            |
 *                 ______________________________________________________
 *                 |                 |                 |                 |
 * lvl1           mt0               mt43               mt8               mt9
 *                 |                 |                                   |
 *           _____________        _______        _________________________________________________
 *           |     |     |        |     |        |     |     |     |     |     |     |     |     |
 * lvl2     mt1   mt2   mt3      mt44  mt45     mt11  mt12  mt13  mt14  mt15  mt16  mt17  mt18  mt19
 *                                |
 *                                |
 *                                |
 * lvl3                          mt46
 *
 * xCoords:  0     1     2     3     4     5     6     7     8     9     10    11    12    13    14
 */
    MergedTree mt42{&tr0};
    MergedTree mt43{&tr0};
    MergedTree mt44{&tr0};
    MergedTree mt45{&tr0};
    MergedTree mt46{&tr0};

    mt44.add(&mt46);

    mt43.add(&mt44);
    mt43.add(&mt45);

    mt42.add(&mt0);
    mt42.add(&mt43);
    mt42.add(&mt8);
    mt42.add(&mt9);

    EXPECT_EQ(mt42.levels.size(),4);
    EXPECT_FLOAT_EQ(mt42.levels.at(0).at(0), 5.5);
    EXPECT_FLOAT_EQ(mt42.levels.at(0).at(1), 6.5);
    EXPECT_FLOAT_EQ(mt42.levels.at(1).at(0), 1);
    EXPECT_FLOAT_EQ(mt42.levels.at(1).at(1), 11);
    EXPECT_FLOAT_EQ(mt42.levels.at(2).at(0), 0);
    EXPECT_FLOAT_EQ(mt42.levels.at(2).at(1), 15);
    EXPECT_FLOAT_EQ(mt42.levels.at(3).at(0), 3.5);
    EXPECT_FLOAT_EQ(mt42.levels.at(3).at(1), 4.5);
/*
 *
 * lvl0                      mt50
 *                             |
 *          +------------+-----------+-----------+
 *          |            |           |           |
 * lvl1    mt51         mt38        mt38        mt0
 *                       |           |           |
 *                       |           |     _____________
 *                       |           |     |     |     |
 * lvl2                 mt39        mt39  mt1   mt2   mt3
 *                       |           |
 *                       |           |
 *                       |           |
 * lvl3                 mt40        mt40
 *                       |           |
 *                       |           |
 *                       |           |
 * lvl4                 mt41        mt41
 *
 * xCoords:  0     1     2     3     4     5     6     7     8     9     10
 */
    MergedTree mt50{&tr0};
    MergedTree mt51{&tr0};

    mt50.add(&mt51);
    mt50.add(&mt38);
    mt50.add(&mt38);
    mt50.add(&mt0);

    EXPECT_EQ(mt50.levels.size(),5);
    EXPECT_FLOAT_EQ(mt50.levels.at(0).at(0), 3);
    EXPECT_FLOAT_EQ(mt50.levels.at(0).at(1), 4);
    EXPECT_FLOAT_EQ(mt50.levels.at(1).at(0), 0);
    EXPECT_FLOAT_EQ(mt50.levels.at(1).at(1), 7);
    EXPECT_FLOAT_EQ(mt50.levels.at(2).at(0), 2);
    EXPECT_FLOAT_EQ(mt50.levels.at(2).at(1), 8);
    EXPECT_FLOAT_EQ(mt50.levels.at(3).at(0), 2);
    EXPECT_FLOAT_EQ(mt50.levels.at(3).at(1), 5);
    EXPECT_FLOAT_EQ(mt50.levels.at(4).at(0), 2);
    EXPECT_FLOAT_EQ(mt50.levels.at(4).at(1), 5);
}
#endif // MERGED_TREE_TEST_H
