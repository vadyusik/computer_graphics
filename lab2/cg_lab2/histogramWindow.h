#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QWidget>
#include <QLabel>

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

signals:

    void RGBhistogram(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                       QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                      QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                       QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                      int mainImage_height, int histImage_height, int histogram_height
                      );
    void LABhistogram(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                       QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                      QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                       QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                      int mainImage_height, int histImage_height, int histogram_height
                      );
    void YIQhistogram(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                       QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                      QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                       QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                      int mainImage_height, int histImage_height, int histogram_height
                      );

private:
    Ui::histogramWindow *ui;
};

#endif // HISTOGRAMWINDOW_H
