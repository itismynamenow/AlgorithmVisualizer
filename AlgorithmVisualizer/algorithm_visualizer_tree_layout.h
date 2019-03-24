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
    void keyPressEvent(QKeyEvent *event) override{
        if(event->key()==Qt::Key_A)
        {
            viewTranslation.setX(viewTranslation.x()-10);
        }
        if(event->key()==Qt::Key_D)
        {
            viewTranslation.setX(viewTranslation.x()+10);
        }
        if(event->key()==Qt::Key_S)
        {
            viewTranslation.setY(viewTranslation.y()-10);
        }
        if(event->key()==Qt::Key_W)
        {
            viewTranslation.setY(viewTranslation.y()+10);
        }
    }
    void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setBrush(Qt::cyan);
        if(root) drawNode(painter,root);
    }
    void drawNode(QPainter &painter, TreeLayoutNode *node, bool hasParent=false){
        if(node->isSet){
            int x = node->position * (nodeSize + xGap) + viewTranslation.x();
            int y = node->level * (nodeSize + yGap) + margin + viewTranslation.y();
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
                        int xFirstChild = firstChild->position * (nodeSize + xGap) + viewTranslation.x();
                        int xLastChild = lastChild->position * (nodeSize + xGap) + viewTranslation.x();
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
    void wheelEvent(QWheelEvent *event) override{
        if(event->delta() > 0){
            nodeSize *= 1.2;
            xGap *= 1.2;
            yGap *= 1.2;
        }else{
            nodeSize /= 1.2;
            xGap /= 1.2;
            yGap /= 1.2;
        }

    }
    void setRandomData(){
        clear();
        int maxChildNum = childNumLineEdit.text().toInt();
        int maxLevel = levelNumLineEdit.text().toInt();
        int nodeLimit = nodeLimitLineEdit.text().toInt();
        int maxOffset = maxWidthLineEdit.text().toInt()/2;
        root =  new TreeLayoutNode();
        nodes.push_back(root);
        std::vector<double> offsets;
        generateRandomTree(root,0,maxChildNum,maxLevel,nodeLimit,offsets,maxOffset);
        treeLayout.setTree(root);
        if(root->isSet){
            viewTranslation.setX(this->width()/2-root->position*(nodeSize+xGap));
        }

    }
    void generateRandomTree(TreeLayoutNode *node, int currLevel,const int maxChildren,const int maxLevel, int &nodeLimit, std::vector<double> &offsetsPerLevel,const int maxOffset){
        if(currLevel <= maxLevel && nodeLimit > 0){
            int numChildren = rand() % maxChildren;

            if(offsetsPerLevel.size()<= currLevel) offsetsPerLevel.resize(currLevel+1,0);
            offsetsPerLevel.at(currLevel) += numChildren;

            if(offsetsPerLevel.at(currLevel) < maxOffset){

                nodeLimit -= numChildren;
                std::vector<int> randomIndices(numChildren,-1);
                std::iota(randomIndices.begin(),randomIndices.end(),0);
                std::random_shuffle (randomIndices.begin(), randomIndices.end());
                node->children = std::vector<TreeLayoutNode*>(numChildren);
                for(auto &child:node->children) child = new TreeLayoutNode();

                for(int i=0;i<numChildren;i++){
                    auto child = node->children.at(randomIndices.at(i));
                    generateRandomTree(child, currLevel+1, maxChildren, maxLevel, nodeLimit,offsetsPerLevel,maxOffset);
                }
            }
        }
    }
protected:
    TreeLayout treeLayout;
    TreeLayoutNode *root = nullptr;
    std::vector<TreeLayoutNode*> nodes;
    double nodeSize = 4;
    double xGap = 2;
    double yGap = 4;
    double margin = 50;
    //UI
    UIPanel uiPanel;
    QPushButton randomButton{"Random"};
    QLabel childNumLabel{"max child num: "};
    QLineEdit childNumLineEdit{"4"};
    QLabel levelNumLabel{"max level num: "};
    QLineEdit levelNumLineEdit{"25"};
    QLabel nodeLimitLabel{"Node limit: "};
    QLineEdit nodeLimitLineEdit{"300"};
    QLabel maxWidthLabel{"Max tree width: "};
    QLineEdit maxWidthLineEdit{"10"};
    QIntValidator childNumInputValidator{1, 10};
    QIntValidator levelNumInputValidator{1, 50};
    QIntValidator nodeLimitInputValidator{1, 599};
    QIntValidator maxWidthInputValidator{1, 199};

    QVector2D viewTranslation;
};

#endif // ALGORITHM_VISUALIZER_TREE_LAYOUT_H
