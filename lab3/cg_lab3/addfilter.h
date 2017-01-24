#ifndef ADDFILTER_H
#define ADDFILTER_H

#include <QWidget>

namespace Ui {
class addfilter;
}

class addfilter : public QWidget
{
    Q_OBJECT

public:
    explicit addfilter(QWidget *parent = 0);
    ~addfilter();

private:
    Ui::addfilter *ui;
};

#endif // ADDFILTER_H
