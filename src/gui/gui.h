#ifndef GUI_H
#define GUI_H

#include <QWidget>

namespace Ui {
class GUI;
}

class GUI : public QWidget
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = nullptr);
    ~GUI();

private:
    Ui::GUI *ui;
};

#endif // GUI_H
