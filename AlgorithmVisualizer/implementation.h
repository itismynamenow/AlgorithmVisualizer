#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H
#include <memory>

#include <QtWidgets>

/*
 * What kind of algorithms can be visualized?
 *      - 2D stuff as KD-tree or Quad tree
 *      - Sorting algs
 *      - Tree and graph related algorithms
 *      - Pathfinding algs
 * What these algs have in common when it comes to visualization?
 *      - They can be represented via simple shapes
 *      - They need some gui elements as buttons and input fields
 *      - They need to work with mouse input
 * What are common to most algs GUI elements?
 *      - Reset button
 *      - Clear button
 *      - Stop/Resume button
 * What functions are important to all visualisations?
 *      - Mouse events
 *      - Resize event
 * What function main window needs?
 *      - Ability to select algorithms via dropdown
 * Is there need for general input side panel?
 *      - Different algs will need different UI so it would be handy
 *      to be able to generate 5 buttons and 3 input filds automatically
 *      - In any case for each alg that needs custom UI it will be necessary
 *      to deal with things like layouts
 *      - So it makes sence to create abstraction for UI holder
 *      to avoid messing with layouts each time
 * Things to implement:
 *      - Main window with:
 *          - Reset, Clear, Stop/Resume buttons
 *          - Alg selection dropdown
 *          - Mechanism to forward Reset, Clear, Stop/Resume signals
 *      - Base class for all alg visualizers with:
 *          - API for Reset, Clear, Stop/Resume
 *      - Base class for UI side panel with:
 *          - API for creation of:
 *              - Horizontal combo box out of n provided widgets
 *          - FlowLayout
 *          - Transparent background
 */

class AlgorithmVisualizer: public QWidget
{
    Q_OBJECT

public:
    virtual void reset()=0;
    virtual void clear()=0;
    virtual void stopResume()=0;
    virtual QString getName()=0;
    virtual ~AlgorithmVisualizer()=default;
};

class AlgorithmVisualizerBase: public AlgorithmVisualizer
{
    Q_OBJECT

public:
    AlgorithmVisualizerBase(){
        timer = new QTimer(this);
        timer->start(updateTimeMs);
    }
    virtual void reset()=0;
    virtual void clear()=0;
    virtual void stopResume()=0;
    virtual QString getName() override{
        return name;
    }
    virtual ~AlgorithmVisualizerBase(){
        delete  timer;
    }
protected:
    QString name{"Default Name"};
    //Update timer
    QTimer* timer;
    int updateTimeMs = 30;
};

class PlaceholderAlgorithmVisualizer: public AlgorithmVisualizerBase{
    Q_OBJECT

public:
    PlaceholderAlgorithmVisualizer(){
        this->setMinimumSize(800, 800);
        connect(this->timer,SIGNAL(timeout()),this,SLOT(update()));
    }
    void paintEvent(QPaintEvent *e) override{
        QPainter painter(this);
        painter.setBrush(QBrush(Qt::cyan));
        painter.drawRect(0,0,800,800);
    }
    virtual void reset() override{}
    virtual void clear() override{}
    virtual void stopResume() override{}
    virtual QString getName() override{return QString("PlaceholderAlgorithm");}
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0){
        initAlgorithmVisulizer();
        initUIPanel();
        setLayout(&mainLayout);
    }
    ~MainWindow(){
        delete currAlgorithmVisualizer;
    }
    void addAlgorithmVisualizer(AlgorithmVisualizer* algorithmVisualizer){
        algorithmVisualizers.push_back(algorithmVisualizer);
    }
    AlgorithmVisualizer* currAlgorithmVisualizer;
    std::vector<AlgorithmVisualizer*> algorithmVisualizers;

    // Bottom UI Panel
    QPushButton resetButton{"Reset"};
    QPushButton clearButton{"Clear"};
    QPushButton stopResumeButton{"Stop/Resume"};
    QComboBox dropdown;
    QWidget uiPanel;
    QHBoxLayout uiPanelLayout;

    QGridLayout mainLayout;
protected:
    void initAlgorithmVisulizer(){
        currAlgorithmVisualizer = new PlaceholderAlgorithmVisualizer();
        currAlgorithmVisualizer->setParent(this);
        mainLayout.addWidget(currAlgorithmVisualizer,0,0,2,1);
    }
    void initUIPanel(){
        uiPanelLayout.addWidget(&dropdown);
        uiPanelLayout.addWidget(&resetButton);
        uiPanelLayout.addWidget(&clearButton);
        uiPanelLayout.addWidget(&stopResumeButton);
        uiPanel.setLayout(&uiPanelLayout);
        mainLayout.addWidget(&uiPanel,1,0,1,1,Qt::AlignHCenter | Qt::AlignBottom);
    }

};
#endif // IMPLEMENTATION_H
