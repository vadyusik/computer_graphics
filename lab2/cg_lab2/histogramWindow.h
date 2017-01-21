#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QWidget>

namespace Ui {
class histogramWindow;
}

class histogramWindow : public QWidget
{
    Q_OBJECT

public:
    explicit histogramWindow(QWidget *parent = 0);
    ~histogramWindow();

private slots:
    void on_RGBButton_clicked();

    void on_LABButton_clicked();

    void on_YIQButton_clicked();

private:
    Ui::histogramWindow *ui;
};

#endif // HISTOGRAMWINDOW_H
