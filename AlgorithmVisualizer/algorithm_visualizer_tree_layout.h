#ifndef ALGORITHM_VISUALIZER_TREE_LAYOUT_H
#define ALGORITHM_VISUALIZER_TREE_LAYOUT_H
#include "algorithm_visualizer_base.h"
#include "ui_panel.h"
#include "../Algorithms/TreeLayout/TreeLayout/tree_layout.h"

class AlgorithmVisualizerTreeLayout: public AlgorithmVisualizerBase{

    Q_OBJECT

public:
    AlgorithmVisualizerTreeLayout(){
        name = "Tree layout";
        setUpTestTree();
        initUIPanel();
        initLayout();
    }
    virtual ~AlgorithmVisualizerTreeLayout() override{
        for(auto node:nodes){
            delete node;
        }
    }
public slots:
    virtual void reset() override{
        for(auto node:nodes){
            delete node;
        }
        root = nullptr;
        nodes.clear();
    }
    virtual void clear() override{
        reset();
    }
protected:
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setBrush(Qt::cyan);
        if(root) drawNode(painter,root);
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
    void initUIPanel(){
        uiPanel.addLineOfWidgets({&randomButton});
        uiPanel.addLineOfWidgets({&childNumLabel, &childNumLineEdit});
        uiPanel.addLineOfWidgets({&levelNumLabel, &levelNumLineEdit});
        uiPanel.addLineOfWidgets({&nodeLimitLabel, &nodeLimitLineEdit});
        uiPanel.addLineOfWidgets({&maxWidthLabel, &maxWidthLineEdit});
        connect(&randomButton, SIGNAL(clicked()), this, SLOT(setRandomData()));
        childNumLineEdit.setValidator(&childNumInputValidator);
        levelNumLineEdit.setValidator(&levelNumInputValidator);
        nodeLimitLineEdit.setValidator(&nodeLimitInputValidator);
        maxWidthLineEdit.setValidator(&maxWidthInputValidator);
    }
    void initLayout(){
        mainLayout.addWidget(&uiPanel,0,0,1,1, Qt::AlignRight | Qt::AlignTop);
        setLayout(&mainLayout);
    }
protected slots:
    void setRandomData(){
        clear();
        int maxChildNum = childNumLineEdit.text().toInt();
        int maxLevel = levelNumLineEdit.text().toInt();
        int nodeLimit = nodeLimitLineEdit.text().toInt();
        int maxOffset = maxWidthLineEdit.text().toInt()/2;
        root =  new TreeLayoutNode();
        nodes.push_back(root);
        generateRandomTree(root,0,maxChildNum,maxLevel,nodeLimit,0,maxOffset);
        treeLayout.setTree(root);
    }
    void generateRandomTree(TreeLayoutNode *node, int currLevel,const int maxChildren,const int maxLevel, int &nodeLimit, int currOffset,const int maxOffset){
        if(currLevel <= maxLevel && nodeLimit > 0 && abs(currOffset)< maxOffset){
            int numChildren = rand() % maxChildren;
            nodeLimit -= numChildren;
            for(int i=0;i<numChildren;i++){
                auto child = new TreeLayoutNode();
                node->children.push_back(child);
                nodes.push_back(child);
                int nextOffset = -numChildren/2 + i;
                generateRandomTree(child, currLevel+1, maxChildren, maxLevel, nodeLimit,currOffset+nextOffset,maxOffset);
            }
        }
    }
protected:
    TreeLayout treeLayout;
    TreeLayoutNode *root;
    std::vector<TreeLayoutNode*> nodes;
    int nodeSize = 16;
    int xGap = 8;
    int yGap = 8;
    int margin = 50;
    //UI
    UIPanel uiPanel;
    QPushButton randomButton{"Random"};
    QLabel childNumLabel{"max child num: "};
    QLineEdit childNumLineEdit{"3"};
    QLabel levelNumLabel{"max level num: "};
    QLineEdit levelNumLineEdit{"20"};
    QLabel nodeLimitLabel{"Node limit: "};
    QLineEdit nodeLimitLineEdit{"300"};
    QLabel maxWidthLabel{"Max tree width: "};
    QLineEdit maxWidthLineEdit{"60"};
    QIntValidator childNumInputValidator{1, 10};
    QIntValidator levelNumInputValidator{1, 50};
    QIntValidator nodeLimitInputValidator{1, 599};
    QIntValidator maxWidthInputValidator{1, 199};
};

#endif // ALGORITHM_VISUALIZER_TREE_LAYOUT_H
