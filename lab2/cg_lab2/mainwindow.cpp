#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // RGB histograms for target and result images comparing
    QObject::connect(&histograms, SIGNAL( RGBhistogram(QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                       QLabel *,  QLabel *,  QLabel *,  QLabel *,QLabel *, QLabel *, QLabel *,
                                                       int, int, int) ),
                     this, SLOT( RGBhistogramSLOT(QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                  QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                  int, int, int) ) );
    // LAB histograms for target and result images comparing
    QObject::connect(&histograms, SIGNAL( LABhistogram(QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                       QLabel *,  QLabel *,  QLabel *,  QLabel *,QLabel *, QLabel *, QLabel *,
                                                       int, int, int) ),
                     this, SLOT( LABhistogramSLOT(QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                  QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                  int, int, int) ) );
    // YIQ histograms for target and result images comparing
    QObject::connect(&histograms, SIGNAL( YIQhistogram(QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                       QLabel *,  QLabel *,  QLabel *,  QLabel *,QLabel *, QLabel *, QLabel *,
                                                       int, int, int) ),
                     this, SLOT( YIQhistogramSLOT(QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                  QLabel *,  QLabel *,  QLabel *,  QLabel *, QLabel *, QLabel *, QLabel *,
                                                  int, int, int) ) );
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
    bool A = false, B = false, C = false;
    if( ui->A_checkBox->checkState() == Qt::Checked ) {A = true;}
    if( ui->B_checkBox->checkState() == Qt::Checked ) {B = true;}
    if( ui->C_checkBox->checkState() == Qt::Checked ) {C = true;}

    imageObj.LABColorCorrection(ui->resultImageLabel, A, B, C );
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
                                    ui->mainImageBHistogramLabel,
                                    290, 90);
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
                                    ui->mainImageBHistogramLabel,
                                    290, 90);

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
                                    ui->mainImageBHistogramLabel,
                                    290, 90);
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
                                    ui->mainImageBHistogramLabel,
                                    290, 90);
}

void MainWindow::on_histogramsButton_clicked()
{
    if( imageObj.isResultImageExist == false )
    {   return;}

    histograms.show();
}

void MainWindow::RGBhistogramSLOT(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                                   QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                                  QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                                   QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                                  int mainImage_height, int histImage_height, int histogram_height
                                  )
{
    if( this->imageObj.isTargetImageLoaded == false || this->imageObj.isResultImageExist == false)
    {   return;}

    imageObj.targetImage.histogramRGB(T_A_Label,  T_B_Label,  T_C_Label,
                                      T_A_HLabel, T_B_HLabel, T_C_HLabel,
                                      histImage_height, histogram_height);

    imageObj.resultImage.histogramRGB(R_A_Label,  R_B_Label,  R_C_Label,
                                      R_A_HLabel, R_B_HLabel, R_C_HLabel,
                                      histImage_height, histogram_height);

    TLabel->resize(mainImage_height, mainImage_height);

    QPixmap pxmptmp = QPixmap :: fromImage(imageObj.targetImage.image);

    if( imageObj.targetImage.image.height() > imageObj.targetImage.image.width() )
    {   pxmptmp = pxmptmp.scaledToHeight(mainImage_height);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(mainImage_height);}

    TLabel->resize(pxmptmp.width(), pxmptmp.height());
    TLabel->setPixmap( pxmptmp );

    RLabel->resize(mainImage_height, mainImage_height);
    pxmptmp = QPixmap :: fromImage(imageObj.resultImage.image);

    if( imageObj.resultImage.image.height() > imageObj.resultImage.image.width() )
    {   pxmptmp = pxmptmp.scaledToHeight(mainImage_height);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(mainImage_height);}

    RLabel->resize(pxmptmp.width(), pxmptmp.height());
    RLabel->setPixmap( pxmptmp );
}

void MainWindow::LABhistogramSLOT(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                                   QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                                  QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                                   QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                                  int mainImage_height, int histImage_height, int histogram_height
                                  )
{
    if( this->imageObj.isTargetImageLoaded == false || this->imageObj.isResultImageExist == false)
    {   return;}

    imageObj.targetImage.histogramLAB(T_A_Label,  T_B_Label,  T_C_Label,
                                      T_A_HLabel, T_B_HLabel, T_C_HLabel,
                                      histImage_height, histogram_height);

    imageObj.resultImage.histogramLAB(R_A_Label,  R_B_Label,  R_C_Label,
                                      R_A_HLabel, R_B_HLabel, R_C_HLabel,
                                      histImage_height, histogram_height);

    TLabel->resize(mainImage_height, mainImage_height);

    QPixmap pxmptmp = QPixmap :: fromImage(imageObj.targetImage.image);

    if( imageObj.targetImage.image.height() > imageObj.targetImage.image.width() )
    {   pxmptmp = pxmptmp.scaledToHeight(mainImage_height);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(mainImage_height);}

    TLabel->resize(pxmptmp.width(), pxmptmp.height());
    TLabel->setPixmap( pxmptmp );

    RLabel->resize(mainImage_height, mainImage_height);
    pxmptmp = QPixmap :: fromImage(imageObj.resultImage.image);

    if( imageObj.resultImage.image.height() > imageObj.resultImage.image.width() )
    {   pxmptmp = pxmptmp.scaledToHeight(mainImage_height);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(mainImage_height);}

    RLabel->resize(pxmptmp.width(), pxmptmp.height());
    RLabel->setPixmap( pxmptmp );
}

void MainWindow::YIQhistogramSLOT(QLabel *TLabel,  QLabel *T_A_Label,  QLabel *T_B_Label,  QLabel *T_C_Label,
                                                   QLabel *T_A_HLabel, QLabel *T_B_HLabel, QLabel *T_C_HLabel,
                                  QLabel *RLabel,  QLabel *R_A_Label,  QLabel *R_B_Label,  QLabel *R_C_Label,
                                                   QLabel *R_A_HLabel, QLabel *R_B_HLabel, QLabel *R_C_HLabel,
                                  int mainImage_height, int histImage_height, int histogram_height
                                  )
{
    if( this->imageObj.isTargetImageLoaded == false || this->imageObj.isResultImageExist == false)
    {   return;}

    imageObj.targetImage.histogramYIQ(T_A_Label,  T_B_Label,  T_C_Label,
                                      T_A_HLabel, T_B_HLabel, T_C_HLabel,
                                      histImage_height, histogram_height);

    imageObj.resultImage.histogramYIQ(R_A_Label,  R_B_Label,  R_C_Label,
                                      R_A_HLabel, R_B_HLabel, R_C_HLabel,
                                      histImage_height, histogram_height);

    TLabel->resize(mainImage_height, mainImage_height);

    QPixmap pxmptmp = QPixmap :: fromImage(imageObj.targetImage.image);

    if( imageObj.targetImage.image.height() > imageObj.targetImage.image.width() )
    {   pxmptmp = pxmptmp.scaledToHeight(mainImage_height);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(mainImage_height);}

    TLabel->resize(pxmptmp.width(), pxmptmp.height());
    TLabel->setPixmap( pxmptmp );

    RLabel->resize(mainImage_height, mainImage_height);
    pxmptmp = QPixmap :: fromImage(imageObj.resultImage.image);

    if( imageObj.resultImage.image.height() > imageObj.resultImage.image.width() )
    {   pxmptmp = pxmptmp.scaledToHeight(mainImage_height);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(mainImage_height);}

    RLabel->resize(pxmptmp.width(), pxmptmp.height());
    RLabel->setPixmap( pxmptmp );
}

void MainWindow::on_YIQcolorCorrectionButton_clicked()
{
    bool A = false, B = false, C = false;
    if( ui->A_checkBox->checkState() == Qt::Checked ) {A = true;}
    if( ui->B_checkBox->checkState() == Qt::Checked ) {B = true;}
    if( ui->C_checkBox->checkState() == Qt::Checked ) {C = true;}

    imageObj.YIQColorCorrection(ui->resultImageLabel, A, B, C );

}

void MainWindow::on_HSVcolorCorrectionButton_clicked()
{
    bool A = false, B = false, C = false;
    if( ui->A_checkBox->checkState() == Qt::Checked ) {A = true;}
    if( ui->B_checkBox->checkState() == Qt::Checked ) {B = true;}
    if( ui->C_checkBox->checkState() == Qt::Checked ) {C = true;}

    imageObj.HSVColorCorrection(ui->resultImageLabel, A, B, C );
}

void MainWindow::on_saveButton_clicked()
{
    imageObj.save_image((QWidget*)this);
}
