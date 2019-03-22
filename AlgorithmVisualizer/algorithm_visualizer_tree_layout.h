#ifndef ALGORITHM_VISUALIZER_TREE_LAYOUT_H
#define ALGORITHM_VISUALIZER_TREE_LAYOUT_H
#include "algorithm_visualizer_base.h"
#include "ui_panel.h"
#include "../Algorithms/TreeLayout/TreeLayout/tree_layout.h"

class AlgorithmVisualizerTreeLayout: public AlgorithmVisualizerBase{
public:
    AlgorithmVisualizerTreeLayout(){
        name = "Tree layout";
        setUpTestTree();
    }
    virtual ~AlgorithmVisualizerTreeLayout() override{
        for(auto node:nodes){
            delete node;
        }
    }
public slots:
    virtual void reset() override{

    }
    virtual void clear() override{
        reset();
    }
protected:
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setBrush(Qt::cyan);
        drawNode(painter,root);
    }
    void drawNode(QPainter &painter, TreeLayoutNode *node, bool hasParent=false){
        if(node->isSet){
            int x = node->position * (nodeSize + xGap) + margin;
            int y = node->level * (nodeSize + yGap) + margin;
            painter.drawRect(x, y, nodeSize, nodeSize);
            if(hasParent){
                painter.drawLine(x+nodeSize/2,y,x+nodeSize/2,y-yGap/2);
            }
            bool hasChildren = node->children.size() > 0;
            if(hasChildren){
                painter.drawLine(x+nodeSize/2,y+nodeSize,x+nodeSize/2,y+nodeSize+yGap/2);
                bool hasMoreThanOneChild = node->children.size() > 1;
                if(hasMoreThanOneChild){
                    auto firstChild = node->children.at(0);
                    auto lastChild = node->children.at(node->children.size()-1);
                    bool childsAreSet = firstChild->isSet && lastChild->isSet;
                    if(childsAreSet){
                        int xFirstChild = firstChild->position * (nodeSize + xGap) + margin;
                        int xLastChild = lastChild->position * (nodeSize + xGap) + margin;
                        painter.drawLine(xFirstChild + nodeSize/2,
                                         y + nodeSize + yGap/2,
                                         xLastChild + nodeSize/2,
                                         y + nodeSize + yGap/2);
                    }
                }
            }
            for(auto child: node->children){
                drawNode(painter,child, true);
            }
        }
    }
    void setUpTestTree(){
        for(int i=0;i<15;i++){
            nodes.push_back(new TreeLayoutNode());
        }
        /*
         *
         * lvl0                   n0
         *                         |
         *                 +-------+------+
         *                 |              |
         * lvl1           n1             n6
         *                 |              |
         *           +-----+-----+     +-----+
         *           |     |     |     |     |
         * lvl2     n2    n4    n5    n7    n13
         *           |                 |
         *           +     +-----+-----+-----+-----+
         *           |     |     |     |     |     |
         * lvl3     n3    n8    n9    n10   n11   n12
         *                                   |
         *                                   +
         *                                   |
         * lvl4                             n14
         *
         * xCoords:  0     1     2     3     4     5
         */
        nodes.at(2)->children.push_back(nodes.at(3));

        nodes.at(11)->children.push_back(nodes.at(14));

        nodes.at(1)->children.push_back(nodes.at(2));
        nodes.at(1)->children.push_back(nodes.at(4));
        nodes.at(1)->children.push_back(nodes.at(5));

        nodes.at(7)->children.push_back(nodes.at(8));
        nodes.at(7)->children.push_back(nodes.at(9));
        nodes.at(7)->children.push_back(nodes.at(10));
        nodes.at(7)->children.push_back(nodes.at(11));
        nodes.at(7)->children.push_back(nodes.at(12));

        nodes.at(6)->children.push_back(nodes.at(7));
        nodes.at(6)->children.push_back(nodes.at(13));

        nodes.at(0)->children.push_back(nodes.at(1));
        nodes.at(0)->children.push_back(nodes.at(6));

        root = nodes.at(0);
        treeLayout.setTree(root);
    }
protected:
    TreeLayout treeLayout;
    TreeLayoutNode *root;
    std::vector<TreeLayoutNode*> nodes;
    int nodeSize = 50;
    int xGap = 15;
    int yGap = 15;
    int margin = 200;
};

#endif // ALGORITHM_VISUALIZER_TREE_LAYOUT_H
