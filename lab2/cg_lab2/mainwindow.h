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

private slots:
    void on_loadMainImageButton_clicked();

    void on_loadTargetImageButton_clicked();

    void on_colorCorrectionButton_clicked();

    void on_mainImageRGBButton_clicked();

    void on_mainImageLABButton_clicked();

    void on_mainImageHSVButton_clicked();



    void on_mainImageYIQButton_clicked();



    void on_histogramsButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
