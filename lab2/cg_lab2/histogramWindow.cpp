#include "histogramWindow.h"
#include "ui_histogramWindow.h"

histogramWindow::histogramWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::histogramWindow)
{
    ui->setupUi(this);
}

histogramWindow::~histogramWindow()
{
    delete ui;
}

void histogramWindow::on_RGBButton_clicked()
{

}

void histogramWindow::on_LABButton_clicked()
{

}

void histogramWindow::on_YIQButton_clicked()
{

}
