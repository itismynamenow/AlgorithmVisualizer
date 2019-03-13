#ifndef UI_PANEL_H
#define UI_PANEL_H
#include <memory>

#include <QtWidgets>

class UIPanel: public QWidget
{
    Q_OBJECT

public:
    UIPanel(){
        setLayout(&mainLayout);
    }
    ~UIPanel(){}
    virtual void addLineOfWidgets(std::initializer_list<QWidget*> widgets){
        std::unique_ptr<QHBoxLayout> hLayout (new QHBoxLayout());
        for(const auto &widget: widgets){
            hLayout->addWidget(widget);
        }
        mainLayout.addLayout(hLayout.get());
        hLayouts.push_back(std::move(hLayout));
    }
protected:
    QVBoxLayout mainLayout;
    std::vector<std::unique_ptr<QHBoxLayout>> hLayouts;
};
#endif // UI_PANEL_H
