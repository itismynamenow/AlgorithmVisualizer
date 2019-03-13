#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <ctime>
#include <QtWidgets>
#include "quad_tree_widget.h"

class MainWindow: public QWidget
{
    Q_OBJECT
public:
    MainWindow(){
        lineEdit.setValidator(&inputValidator);

        mainLayout.addWidget(&qWidget,0,0);
        inputPanelLayout.addWidget(&clearButton);
        inputPanelLayout.addWidget(&randomButton);
        inputPanelLayout.addWidget(&lineEdit);
        mainLayout.addItem(&inputPanelLayout,1,0);
        setLayout(&mainLayout);
        connect(&clearButton, SIGNAL(clicked()), &qWidget, SLOT(reset()));
        connect(&randomButton, SIGNAL(clicked()), this, SLOT(setRandomData()));
        srand( time( 0 ) );
    }
private:

    QGridLayout mainLayout;
    QPushButton clearButton{"Clear"};
    QPushButton randomButton{"Random"};
    QLineEdit lineEdit{"99"};
    QTreeVisualisationWidget qWidget;
    QHBoxLayout inputPanelLayout;
    QIntValidator inputValidator{1, 9999};
private slots:
    void setRandomData(){
        vector<QPoint> points;
        for(int i=0;i<lineEdit.text().toInt();i++){
            QPoint point(rand()%800,rand()%800);
            points.push_back(point);
        }
        qWidget.setPoints(points);
    }

};

#endif // MAINWINDOW_H
