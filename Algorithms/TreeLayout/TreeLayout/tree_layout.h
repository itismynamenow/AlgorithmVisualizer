#ifndef TREE_LAYOUT_H
#define TREE_LAYOUT_H

#include <vector>
#include <array>
#include <algorithm>

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

struct TreeNode{
    std::vector<TreeNode> children;
    double width = 1;
};

class MergedTree{
public:
    MergedTree(TreeNode *node){
        this->node = node;
        levels.push_back({0,node->width});
    }
    void add(MergedTree *another, int levelOffset=1){
        auto totalLevels = std::max(this->levels.size(),another->levels.size()+levelOffset);
        if(this->levels.size() < totalLevels){
            this->levels.resize(totalLevels,{0,0});
        }
        double maxAnotherOffset=0;
        int maxAnotherOffsetLevel=0;
        for(int i=levelOffset;i<levelOffset+another->levels.size();i++){
            auto &thisLevel = this->levels.at(i);
            auto thisLevelWidth = thisLevel.at(1) - thisLevel.at(0);
            auto &anotherLevel = another->levels.at(i-levelOffset);
            double currAnotherOffset = thisLevelWidth - anotherLevel.at(0);
            if(currAnotherOffset > maxAnotherOffset){
                maxAnotherOffset = currAnotherOffset;
                maxAnotherOffsetLevel = i;
            }
        }

        for(int i=levelOffset;i<levelOffset+another->levels.size();i++){
            auto &thisLevel = this->levels.at(i);
            auto &anotherLevel = another->levels.at(i-levelOffset);
            auto anotherLevelWidth = anotherLevel.at(1) - anotherLevel.at(0);
            //Check if there is no nodes on the level
            if(thisLevel.at(1)==0){
                //Take xMin (thisLevel.at(0)) from anotherLevel
                thisLevel.at(0) = anotherLevel.at(0);
            }
            thisLevel.at(1) = anotherLevel.at(0) + anotherLevelWidth + maxAnotherOffset;
        }
        centerZeroLevel();
    }
protected:
    void centerZeroLevel(){
        if(levels.size()>=2){
            auto firstLevelCenter = levels.at(1).at(0) +
                    (levels.at(1).at(1) - levels.at(1).at(0))/2;
            levels.at(0).at(0) = firstLevelCenter - node->width/2;
            levels.at(0).at(1) = firstLevelCenter + node->width/2;
        }
    }
    FRIEND_TEST(MergedTree, add);
    ///Each array represents x range and hold min and max x value
    std::vector<std::array<double,2>> levels;
    TreeNode *node;
};

#endif // TREE_LAYOUT_H
