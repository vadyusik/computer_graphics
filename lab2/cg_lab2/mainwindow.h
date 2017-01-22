#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QImage>
#include <QFileDialog>
#include <QString>

#include <QDebug>
#include <QPainter>
#include <QPixmap>

#include <QVector>
#include <QVector3D>
#include <QColor>
#include <QtMath>

#include <QTime>
#include <QPoint>
#include <QLabel>

#include <MyImage.h>
#include <histogramWindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    MyImage imageObj;
    histogramWindow histograms;

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Buttons slots
private slots:
    void on_loadMainImageButton_clicked();
    void on_loadTargetImageButton_clicked();

    void on_colorCorrectionButton_clicked();

    void on_mainImageRGBButton_clicked();
    void on_mainImageLABButton_clicked();
    void on_mainImageHSVButton_clicked();
    void on_mainImageYIQButton_clicked();

    void on_histogramsButton_clicked();

    // slots for histogramWindow
    void on_YIQcolorCorrectionButton_clicked();

    void on_HSVcolorCorrectionButton_clicked();

    void on_saveButton_clicked();

public slots:

    void RGBhistogramSLOT(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                           QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                          QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                           QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                          int mainImage_height, int histImage_height, int histogram_height
                          );
    void LABhistogramSLOT(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                           QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                          QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                           QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                          int mainImage_height, int histImage_height, int histogram_height
                          );
    void YIQhistogramSLOT(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                           QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                          QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                           QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                          int mainImage_height, int histImage_height, int histogram_height
                          );

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
