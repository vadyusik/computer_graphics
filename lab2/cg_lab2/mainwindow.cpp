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
