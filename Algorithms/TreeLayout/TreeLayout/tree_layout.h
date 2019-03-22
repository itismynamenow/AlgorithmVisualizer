#ifndef TREE_LAYOUT_H
#define TREE_LAYOUT_H

#include <vector>
#include <array>
#include <algorithm>
#include <memory>

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

struct TreeLayoutNode{
    std::vector<TreeLayoutNode*> children;
    double width = 1;
    bool isSet = false;
    int level=0;
    double position=0;
};

class MergedTree{
public:
    MergedTree(TreeLayoutNode *node){
        this->node = node;
        levels.push_back({0,node->width});
    }
    void add(MergedTree *another, int levelOffset=1){
        resizeContainers(another, levelOffset);
        //Find how much entire another tree will be shifted right
        double minOffset = computeMinOffset(another, levelOffset);

        for(int i=levelOffset;i<levelOffset+another->levels.size();i++){
            auto &thisLevel = this->levels.at(i);
            auto &anotherLevel = another->levels.at(i-levelOffset);
            auto anotherLevelWidth = anotherLevel.at(1) - anotherLevel.at(0);
            //Store previous value of level one
            if(i==1) prevLevelOne = thisLevel;
            //Check if there is no nodes on the level
            if(thisLevel.at(1)==0){
                //Take xMin (thisLevel.at(0)) from anotherLevel
                thisLevel.at(0) = anotherLevel.at(0) + minOffset;
                levelsFirstTreeId.at(i) = treesAdded+1;
            }
            thisLevel.at(1) =  anotherLevel.at(0) + anotherLevelWidth + minOffset;
            levelsLastTreeId.at(i) = treesAdded+1;
        }
        spreadSubtreesEqually();
        centerZeroLevel();
        //Update this value at the end only to avoid confusions
        treesAdded++;
    }
    void add(std::shared_ptr<MergedTree> another, int levelOffset=1){
        childMergedTrees.push_back(another);
        add(another.get());
    }
    void assignValuesToNodes(const int rootLevel, const double rootOffset, bool applyRootOffset=true){
        double xOffset = 0;
        if(applyRootOffset) xOffset = rootOffset - levels.at(0).at(0);
        //Add coords of root node
        node->position = levels.at(0).at(0) + xOffset;
        node->level = rootLevel;
        node->isSet = true;
        if(childMergedTrees.size() > 0){
            double nodeSpacing = getSpacing();
            for(int i=0;i<childMergedTrees.size();i++){
                auto child = childMergedTrees.at(i);
                auto currOffset = levels.at(1).at(0) + nodeSpacing * i + xOffset;
                child->assignValuesToNodes(rootLevel+1,currOffset);
            }
        }
    }
protected:
    //Computes min x offset of another tree
    //The result will determine how much entire another tree will be shifted right
    double computeMinOffset(MergedTree *another, int levelOffset=1)
    {
        double minAnotherOffset=0;
        for(int i=levelOffset;i<levelOffset+another->levels.size();i++){
            auto &thisLevel = this->levels.at(i);
            auto &anotherLevel = another->levels.at(i-levelOffset);
            double currAnotherOffset = thisLevel.at(1) - anotherLevel.at(0);
            if(currAnotherOffset > minAnotherOffset){
                minAnotherOffset = currAnotherOffset;
            }
        }
        return  minAnotherOffset;
    }
    //Resizes used vectors to accomodate data from another
    void resizeContainers(MergedTree *another, int levelOffset=1)
    {
        auto totalLevels = std::max(this->levels.size(),another->levels.size()+levelOffset);
        if(this->levels.size() < totalLevels){
            this->levels.resize(totalLevels,{0,0});
            levelsFirstTreeId.resize(totalLevels,0);
            levelsLastTreeId.resize(totalLevels,0);
        }
    }
    //Computes distances between level 1 nodes
    double getSpacing(){
        auto levelOneWidth = levels.at(1).at(1) - levels.at(1).at(0) - node->width;
        double nodeSpacing = 0;
        if(childMergedTrees.size()>1){
            nodeSpacing = levelOneWidth/((double)(childMergedTrees.size()-1));
        }
        return  nodeSpacing;
    }
    void centerZeroLevel(){
        if(levels.size()>1){
            auto firstLevelCenter = levels.at(1).at(0) +
                    (levels.at(1).at(1) - levels.at(1).at(0))/2;
            levels.at(0).at(0) = firstLevelCenter - node->width/2;
            levels.at(0).at(1) = firstLevelCenter + node->width/2;
        }
    }
    void spreadSubtreesEqually(){
        //Remeber we update treesAdded afterwards
        if(treesAdded>1){
            auto prevSpacing = (prevLevelOne.at(1) - prevLevelOne.at(0) - node->width) / (treesAdded-1);
            auto currentSpacing = levels.at(1).at(1) - prevLevelOne.at(1);
            if(prevSpacing > currentSpacing){
                for(int i=1;i<levels.size();i++){
                    if(levelsLastTreeId.at(i) == treesAdded+1){
                        levels.at(i).at(1) += - currentSpacing + prevSpacing;
                    }
                    if(levelsFirstTreeId.at(i) == treesAdded+1){
                        levels.at(i).at(0) += - currentSpacing + prevSpacing;
                    }
                }
            }else if(prevSpacing < currentSpacing){
                for(int i=1;i<levels.size();i++){
                    levels.at(i).at(0) += (currentSpacing - prevSpacing) * (levelsFirstTreeId.at(i)-1);
                    //Last element of level already has right spacing if it was just modified so we exclude it
                    if(levelsLastTreeId.at(i) == treesAdded+1){
                        levels.at(i).at(1) += (currentSpacing - prevSpacing) * (levelsLastTreeId.at(i)-2);
                    }else{
                        levels.at(i).at(1) += (currentSpacing - prevSpacing) * (levelsLastTreeId.at(i)-1);
                    }
                }
            }
        }
    }
    FRIEND_TEST(MergedTree, add);
    //For each level of tree we save xMin and xMax values
    std::vector<std::array<double,2>> levels;
    //Saves id of last subtree that was added to given level
    std::vector<int> levelsLastTreeId;
    //Saves id of first subtree that was added to given level
    std::vector<int> levelsFirstTreeId;
    std::array<double,2> prevLevelOne;
    int treesAdded = 0;
    TreeLayoutNode *node;
    std::vector<std::shared_ptr<MergedTree>> childMergedTrees;
};

class TreeLayout{
public:
    TreeLayout(){}
    TreeLayout(TreeLayoutNode *root){
        setTree(root);
    }
    void setTree(TreeLayoutNode *root){
        clear();
        this->root = root;
        rootMergedTree = buildMergedTree(root);
        rootMergedTree->assignValuesToNodes(0,0,false);
    }
    void clear(){
        root = nullptr;
    }
protected:
    std::shared_ptr<MergedTree> buildMergedTree(TreeLayoutNode *node) const{
        auto currMergedTree = std::make_shared<MergedTree>(MergedTree(node));
        for(auto &child: node->children){
            currMergedTree->add(buildMergedTree(child));
        }
        return  currMergedTree;
    }
    TreeLayoutNode *root = nullptr;
    std::shared_ptr<MergedTree> rootMergedTree;
};

#endif // TREE_LAYOUT_H
