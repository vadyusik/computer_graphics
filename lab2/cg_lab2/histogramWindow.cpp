#include "histogramWindow.h"
#include "ui_histogramWindow.h"

#include <QDebug>

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
    emit RGBhistogram(ui->targetImageFullLabel, ui->targetImageALabel, ui->targetImageBLabel, ui->targetImageCLabel,
                      ui->targetHistogramALabel, ui->targetHistogramBLabel, ui->targetHistogramCLabel,
                      ui->resultImageFullLabel, ui->resultImageALabel, ui->resultImageBLabel, ui->resultImageCLabel,
                      ui->resultHistogramALabel, ui->resultHistogramBLabel, ui->resultHistogramCLabel,
                      340, 270, 80);
    qDebug() << "SIGNAL RGBhistogram emited";
}

void histogramWindow::on_LABButton_clicked()
{
    emit LABhistogram(ui->targetImageFullLabel, ui->targetImageALabel, ui->targetImageBLabel, ui->targetImageCLabel,
                      ui->targetHistogramALabel, ui->targetHistogramBLabel, ui->targetHistogramCLabel,
                      ui->resultImageFullLabel, ui->resultImageALabel, ui->resultImageBLabel, ui->resultImageCLabel,
                      ui->resultHistogramALabel, ui->resultHistogramBLabel, ui->resultHistogramCLabel,
                      340, 270, 80);
}

void histogramWindow::on_YIQButton_clicked()
{
    emit YIQhistogram(ui->targetImageFullLabel, ui->targetImageALabel, ui->targetImageBLabel, ui->targetImageCLabel,
                      ui->targetHistogramALabel, ui->targetHistogramBLabel, ui->targetHistogramCLabel,
                      ui->resultImageFullLabel, ui->resultImageALabel, ui->resultImageBLabel, ui->resultImageCLabel,
                      ui->resultHistogramALabel, ui->resultHistogramBLabel, ui->resultHistogramCLabel,
                      340, 270, 80);
}
