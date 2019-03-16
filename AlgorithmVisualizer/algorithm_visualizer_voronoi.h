#ifndef ALGORITHM_VISUALIZER_VORONOI_H
#define ALGORITHM_VISUALIZER_VORONOI_H
#include <iostream>
#include <unordered_map>
#include <bits/stdc++.h>

#include <QtWidgets>

#include "algorithm_visualizer_base.h"
#include "ui_panel.h"
#include "../Algorithms/VoronoiDiagram/VoronoiDiagram/voronoi_diagram.h"

class AlgorithmVisualizerVoronoi: public AlgorithmVisualizerBase
{
    Q_OBJECT

public:
    AlgorithmVisualizerVoronoi(){
        name = "Voronoi diagram";
        initUIPanel();
        initLayout();
    }
public slots:
    virtual void reset() override{
        points.clear();
        voronoiDiagram.clear();
    }
    virtual void clear() override{
        reset();
    }
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override{
        if(mouseEvent->buttons() == Qt::LeftButton){
            auto x = mouseEvent->pos().x();
            auto y = mouseEvent->pos().y();
            points.push_back(QVector2D(x,y));
            sendData();
        }
    }
    virtual void paintEvent(QPaintEvent *event) override{
        QPainter painter(this);
        painter.setPen(QPen(Qt::red, 2));
        for(const auto &polygon: voronoiDiagram.polygons){
            drawMyPolygon(painter, polygon);
        }
        painter.setPen(QPen(Qt::lightGray, 1));
        for(const auto &triangle: voronoiDiagram.triangles){
            drawMyPolygon(painter, triangle);
        }
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(Qt::yellow);
        for(const auto point: points){
            painter.drawEllipse(point.x()-circleSize/2,point.y()-circleSize/2,circleSize,circleSize);
        }
        painter.setBrush(Qt::red);
        for(const auto &vertex: voronoiDiagram.polygonsVertices){
            painter.drawEllipse(vertex.x()-circleSize/2,vertex.y()-circleSize/2,circleSize,circleSize);
        }
    }
    void drawMyPolygon(QPainter &painter, const MyPolygon &polygon){
        auto &vertices = polygon.vertices;
        for(unsigned i=0;i<vertices.size();i++){
            int iNext = (i+1)%polygon.vertices.size();
            painter.drawLine(vertices.at(i).x(),vertices.at(i).y(),vertices.at(iNext).x(),vertices.at(iNext).y());
        }
    }
    void initUIPanel(){
        uiPanel.addLineOfWidgets({&randomButton, &randomLabel, &randomLineEdit});
        connect(&randomButton, SIGNAL(clicked()), this, SLOT(setRandomData()));
        randomLineEdit.setValidator(&randomInputValidator);
    }
    void initLayout(){
        mainLayout.addWidget(&uiPanel,0,0,1,1, Qt::AlignLeft | Qt::AlignTop);
        setLayout(&mainLayout);
    }
    void sendData(){
        voronoiDiagram.setPoints(points);
    }


protected slots:
    void setRandomData(){
        int elementsToGenerate = randomLineEdit.text().toInt();

        for(unsigned i=0;i<elementsToGenerate;i++){
            double x = rand()%800;
            double y = rand()%800;
            points.push_back(QVector2D(x,y));
        }
        sendData();
    }
protected:
    UIPanel uiPanel;
    QPushButton randomButton{"Random"};
    QLabel randomLabel{"in range from 1 to "};
    QLineEdit randomLineEdit{"55"};
    QComboBox dropdown;
    QIntValidator randomInputValidator{1, 99};

    int circleSize = 8;

    std::vector<QVector2D> points;
    VoronoiDiagram voronoiDiagram;
};

#endif // ALGORITHM_VISUALIZER_VORONOI_H
