#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H
#include <memory>
#include <iostream>

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
 *      - Reset button DONE
 *      - Clear button DONE
 *      - Stop/Resume button DONE
 * What functions are important for all visualisations?
 *      - Mouse events
 *      - Resize event
 * What function main window needs?
 *      - Ability to select algorithms via dropdown DONE
 * Is there need for general input side panel?
 *      - Different algs will need different UI so it would be handy
 *      to be able to generate 5 buttons and 3 input filds automatically
 *      - In any case for each alg that needs custom UI it will be necessary
 *      to deal with things like layouts
 *      - So it makes sence to create abstraction for UI holder
 *      to avoid messing with layouts each time
 * Things to implement:
 *      - Main window with:
 *          - Reset, Clear, Stop/Resume buttons DONE
 *          - Alg selection dropdown DONE
 *          - Mechanism to forward Reset, Clear, Stop/Resume signals DONE
 *      - Base class for all alg visualizers with:
 *          - API for Reset, Clear, Stop/Resume DONE
 *      - Base class for UI side panel with:
 *          - API for creation of:
 *              - Horizontal combo box out of n provided widgets
 *          - FlowLayout FAIL
 *          - Transparent background
 */

class UIPanel: public QWidget
{
    Q_OBJECT

public:
    UIPanel(){
        setLayout(&mainLayout);
    }
    ~UIPanel(){
        for(auto &hLayout: hLayouts){
            delete hLayout;
        }
    }
    virtual void addLineOfWidgets(std::vector<QWidget*> widgets){
        auto hLayout = new QHBoxLayout();
        for(const auto &widget: widgets){
            hLayout->addWidget(widget);
        }
        mainLayout.addLayout(hLayout);
        hLayouts.push_back(hLayout);
    }
protected:
    QVBoxLayout mainLayout;
    std::vector<QHBoxLayout*> hLayouts;
};

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
    virtual void reset() override=0;
    virtual void clear() override=0;
    virtual void stopResume() override=0;
    virtual QString getName() override{
        return name;
    }
    virtual ~AlgorithmVisualizerBase() override{
        delete  timer;
    }
protected:
    QString name{"Default Name"};
    //Update timer
    QTimer* timer;
    int updateTimeMs = 30;
};

class PlaceholderAlgorithmVisualizer: public AlgorithmVisualizerBase
{
    Q_OBJECT

public:
    PlaceholderAlgorithmVisualizer(){
        name = "Placeholder";
        initUIPanel();
        initLayout();
        this->setMinimumSize(800, 800);
        connect(this->timer,SIGNAL(timeout()),this,SLOT(update()));
    }
    void paintEvent(QPaintEvent *e) override{
        QPainter painter(this);
        painter.setBrush(QBrush(Qt::cyan));
        painter.drawRect(0,0,800,800);
    }
    virtual void reset() override{std::cout<<"Reset"<<std::endl;}
    virtual void clear() override{std::cout<<"Clear"<<std::endl;}
    virtual void stopResume() override{std::cout<<"StopResume"<<std::endl;}
    virtual QString getName() override{return QString("PlaceholderAlgorithm");}
protected:
    void initUIPanel(){
        uIPanel.addLineOfWidgets(std::vector<QWidget*>{&someButton,&someLabel,&someLineEdit});
        uIPanel.addLineOfWidgets(std::vector<QWidget*>{&someButton1,&someLabel1,&someLineEdit1});
    }
    void initLayout(){
        mainLayout.addWidget(&uIPanel,0,0,1,1, Qt::AlignLeft | Qt::AlignTop);
        setLayout(&mainLayout);
    }
protected:
    UIPanel uIPanel;
    QGridLayout mainLayout;
    QPushButton someButton{"Some button"};
    QPushButton someButton1{"Some button1"};
    QLineEdit someLineEdit{"Some line edit"};
    QLineEdit someLineEdit1{"Some line edit1"};
    QLabel someLabel{"Some label"};
    QLabel someLabel1{"Some label1"};
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr){
        init();
        initAlgorithmVisulizer();
        initUIPanel();
        initConnects();
        setLayout(&mainLayout);
    }
    ~MainWindow(){
        delete defaultAlgorithmVisualizer;
    }
    void addAlgorithmVisualizer(AlgorithmVisualizer* algorithmVisualizer){
        algorithmVisualizers.push_back(algorithmVisualizer);
        dropdown.addItem(algorithmVisualizer->getName());
    }
protected:
    void init(){
        mainLayout.setSpacing(0);
        mainLayout.setMargin(0);
    }
    void initAlgorithmVisulizer(){
        defaultAlgorithmVisualizer = new PlaceholderAlgorithmVisualizer();
        addAlgorithmVisualizer(defaultAlgorithmVisualizer);
        setCurrAlgorithmVisualiser(defaultAlgorithmVisualizer);
    }
    void initUIPanel(){
        uiPanelLayout.addWidget(&dropdown);
        uiPanelLayout.addWidget(&resetButton);
        uiPanelLayout.addWidget(&clearButton);
        uiPanelLayout.addWidget(&stopResumeButton);
        uiPanel.setLayout(&uiPanelLayout);
        mainLayout.addWidget(&uiPanel,1,0,1,1,Qt::AlignHCenter | Qt::AlignBottom);
    }
    void initConnects(){
        connect(&dropdown,SIGNAL(currentIndexChanged(int)),this,SLOT(onDropdownChange()));
        connect(&resetButton,SIGNAL(pressed()),this,SLOT(onResetButtonPress()));
        connect(&clearButton,SIGNAL(pressed()),this,SLOT(onClearButtonPress()));
        connect(&stopResumeButton,SIGNAL(pressed()),this,SLOT(onStopResumeButtonPress()));
    }
protected slots:
    void setCurrAlgorithmVisualiser(AlgorithmVisualizer* algorithmVisualizer){
        if(algorithmVisualizer != currAlgorithmVisualizer){
            mainLayout.removeWidget(currAlgorithmVisualizer);
            currAlgorithmVisualizer = algorithmVisualizer;
            currAlgorithmVisualizer->setParent(this);
            mainLayout.addWidget(currAlgorithmVisualizer,0,0,2,1);
        }
    }
    void onDropdownChange(){
        setCurrAlgorithmVisualiser(algorithmVisualizers.at(dropdown.currentIndex()));
    }
    void onResetButtonPress(){
        currAlgorithmVisualizer->reset();
    }
    void onClearButtonPress(){
        currAlgorithmVisualizer->clear();
    }
    void onStopResumeButtonPress(){
        currAlgorithmVisualizer->stopResume();
    }

protected:

    AlgorithmVisualizer* currAlgorithmVisualizer = nullptr;
    AlgorithmVisualizer* defaultAlgorithmVisualizer = nullptr;
    std::vector<AlgorithmVisualizer*> algorithmVisualizers;

    // Bottom UI Panel
    QPushButton resetButton{"Reset"};
    QPushButton clearButton{"Clear"};
    QPushButton stopResumeButton{"Stop/Resume"};
    QComboBox dropdown;
    QWidget uiPanel;
    QHBoxLayout uiPanelLayout;

    QGridLayout mainLayout;

};
#endif // IMPLEMENTATION_H
