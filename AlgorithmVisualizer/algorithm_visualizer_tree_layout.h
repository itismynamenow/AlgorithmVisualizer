#ifndef ALGORITHM_VISUALIZER_TREE_LAYOUT_H
#define ALGORITHM_VISUALIZER_TREE_LAYOUT_H
#include "algorithm_visualizer_base.h"
#include "ui_panel.h"
#include "../Algorithms/TreeLayout/TreeLayout/tree_layout.h"

class AlgorithmVisualizerTreeLayout: public AlgorithmVisualizerBase{
public:
    AlgorithmVisualizerTreeLayout(){
        name = "Tree layout";
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
        auto nodesPos =treeLayout.getNodesPositions();
        if(nodesPos.size()>0){
            painter.setBrush(Qt::cyan);
            for(int i=0;i<nodesPos.size();i++){
                for(int j=0;j<nodesPos.at(i).size();j++){
                    auto currLevel = i;
                    auto currX = nodesPos.at(i).at(j);
                    painter.drawRect(currX*nodeXDistance + margin, currLevel*nodeYDistance+margin,nodeSize,nodeSize );
                }
            }
        }
    }
protected:
    TreeLayout treeLayout;
    int nodeSize = 50;
    int nodeXDistance = 65;
    int nodeYDistance = 65;
    int margin = 200;
};

#endif // ALGORITHM_VISUALIZER_TREE_LAYOUT_H
