#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadMainImageButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/vadya/Dropbox/coding/ComputerGraphic/ImageExamples",
        tr("Image Files (*.png *.jpg *.bmp)"));

    // my old version
    //QString fileName = QFileDialog::getOpenFileName(label_obj, tr("Open File"), "/home/vadya/Dropbox/coding/ComputerGraphic/ImageExamples", tr("Images (*.png *.xpm *.jpg)"));


    imageObj.load_image(fileName, true);
    //sourceImage = new QImage(fileName);
    //sourceImageScene = new GraphicsScene;
    sourceImageScene->clear();
    sourceImageItem  =  new QGraphicsPixmapItem ( QPixmap::fromImage(*sourceImage));
    sourceImageScene->addItem(sourceImageItem);
    sourceImageScene->setHA(sourceImage->width(), sourceImage->height());



    ui->souceImageGraphicsView->setScene(sourceImageScene);
    //ui->souceImageGraphicsView->repaint();
    //qDebug() << this->ui->mainImageWidget->height() << " " << this->ui->mainImageWidget->width();

    emit loadMainImage(this->ui->mainImageWidget, true);
}
