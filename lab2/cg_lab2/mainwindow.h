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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    MyImage imageObj;

    GraphicsScene *sourceImageScene = 0;
    GraphicsScene *targetImageScene = 0;
    GraphicsScene *resultImageScene = 0;

    QGraphicsPixmapItem *sourceImageItem = 0;
    QGraphicsPixmapItem *targetImageItem = 0;
    QGraphicsPixmapItem *resultImageItem = 0;


public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadMainImageButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
