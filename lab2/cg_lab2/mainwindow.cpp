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
    imageObj.load_image(ui->mainImageLabel, true);
}

void MainWindow::on_loadTargetImageButton_clicked()
{
    imageObj.load_image(ui->targetImageLabel, false);
}

void MainWindow::on_colorCorrectionButton_clicked()
{
    imageObj.colorCorrection(ui->resultImageLabel);
}

void MainWindow::on_mainImageRGBButton_clicked()
{
    if( this->imageObj.isMainImageLoaded == false )
    {   return;}

    imageObj.mainImage.histogramRGB(ui->mainImageRLabel,
                                    ui->mainImageGLabel,
                                    ui->mainImageBLabel,
                                    ui->mainImageRHistogramLabel,
                                    ui->mainImageGHistogramLabel,
                                    ui->mainImageBHistogramLabel);
}

void MainWindow::on_mainImageLABButton_clicked()
{
    if( this->imageObj.isMainImageLoaded == false )
    {   return;}

    imageObj.mainImage.histogramLAB(ui->mainImageRLabel,
                                    ui->mainImageGLabel,
                                    ui->mainImageBLabel,
                                    ui->mainImageRHistogramLabel,
                                    ui->mainImageGHistogramLabel,
                                    ui->mainImageBHistogramLabel);

}

void MainWindow::on_mainImageHSVButton_clicked()
{
    if( this->imageObj.isMainImageLoaded == false )
    {   return;}

    imageObj.mainImage.histogramHSV(ui->mainImageRLabel,
                                    ui->mainImageGLabel,
                                    ui->mainImageBLabel,
                                    ui->mainImageRHistogramLabel,
                                    ui->mainImageGHistogramLabel,
                                    ui->mainImageBHistogramLabel);
}



void MainWindow::on_mainImageYIQButton_clicked()
{
    if( this->imageObj.isMainImageLoaded == false )
    {   return;}

    imageObj.mainImage.histogramYIQ(ui->mainImageRLabel,
                                    ui->mainImageGLabel,
                                    ui->mainImageBLabel,
                                    ui->mainImageRHistogramLabel,
                                    ui->mainImageGHistogramLabel,
                                    ui->mainImageBHistogramLabel);
}

void MainWindow::on_histogramsButton_clicked()
{
    if( imageObj.isResultImageExist == false )
    {   return;}

    histograms.show();
}
