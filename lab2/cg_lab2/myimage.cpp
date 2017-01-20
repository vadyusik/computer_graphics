#include "MyImage.h"

MyImage::MyImage(QObject *parent) :
    QObject(parent)
{
    isMainImageLoaded   = false;
    isTargetImageLoaded = false;
    isResultImageExist  = false;

}
//rewrited 19-01-2017
void MyImage::   load_image(QLabel *labelObj, bool is_main_image)
{
    QString fileName =
        QFileDialog::getOpenFileName(labelObj,
            tr("Open Image"), "/home/vadya/Dropbox/coding/ComputerGraphic/ImageExamples",
            tr("Image Files (*.png *.jpg *.bmp)"));

    if( is_main_image == true )
    {
        mainImage.set_image     ( QImage(fileName) );
        labelObj->resize(720, 720);
        setImage(labelObj, &(mainImage.image) );
        isMainImageLoaded   = true;
    }else

    {
        targetImage.set_image   ( QImage(fileName) );
        labelObj->resize(310, 310);
        setImage(labelObj, &(targetImage.image) );
        isTargetImageLoaded = true;
    }
}
//writed 19-01-2017
void MyImage:: setImage(QLabel *labelObj, QImage *img)
{
    int h = img->height(),
        w = img->width(),
        l = labelObj->height();

    QPixmap pxmptmp = QPixmap :: fromImage(*img);

    if( h > w )
    {   pxmptmp = pxmptmp.scaledToHeight(l);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(l);}

    labelObj->resize(pxmptmp.width(), pxmptmp.height());
    labelObj->setPixmap( pxmptmp );
}

void MyImage::  convert()
{
    this->mainImage.convert();
}

void MyImage:: colorCorrection(QLabel *labelObj)
{
    if( isMainImageLoaded == false || isTargetImageLoaded == false)
    {
        qDebug() << "NOT enough data to color correction!";
        return;
    }

    resultImage.image = targetImage.image;
    resultImage.image_lab = targetImage.image_lab;

    QVector3D mainImgEx = mainImage.expected_value,
              targImgEx = targetImage.expected_value,
              koef;

    koef.setX( mainImage.standard_deviation.x() / targetImage.standard_deviation.x() );
    koef.setY( mainImage.standard_deviation.y() / targetImage.standard_deviation.y() );
    koef.setZ( mainImage.standard_deviation.z() / targetImage.standard_deviation.z() );

    QVector< QVector< QVector3D > > *lab = &resultImage.image_lab;

    int h = resultImage.image.height(),
        w = resultImage.image.width();

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            resultImage.image_lab[i][j].setX( mainImgEx.x() + koef.x()*( resultImage.image_lab[i][j].x() - targImgEx.x() ) );
            resultImage.image_lab[i][j].setY( mainImgEx.y() + koef.y()*( resultImage.image_lab[i][j].y() - targImgEx.y() ) );
            resultImage.image_lab[i][j].setZ( mainImgEx.z() + koef.z()*( resultImage.image_lab[i][j].z() - targImgEx.z() ) );
        }
    }

    resultImage.convert_LAB_to_RGB();

    labelObj->resize(310, 310);
    setImage(labelObj, &resultImage.image);
}


//// IMAGE STRUCTURE

void ImageStructure::   set_image(QImage obj)
{
    QTime time, eachtime;
    time.start();   eachtime.start();

    image = obj;

    calculate_LAB();
    calculate_moments();

    qDebug() << "Expected value of L color chanal is " << expected_value.x();
    qDebug() << "Expected value of A color chanal is " << expected_value.y();
    qDebug() << "Expected value of B color chanal is " << expected_value.z();

    qDebug() << "Standard deviation of L color chanal is " << standard_deviation.x();
    qDebug() << "Standard deviation of A color chanal is " << standard_deviation.y();
    qDebug() << "Standard deviation of B color chanal is " << standard_deviation.z();

    qDebug() << "Total time " << time.elapsed();
}

// rewrited
void ImageStructure::   calculate_LAB()
{
    int h = image.height(),
        w = image.width();

    image_lab.clear();
    image_lab = QVector<QVector<QVector3D> >(w, QVector<QVector3D>(h, QVector3D(0.0, 0.0, 0.0) ) );

    QRgb color;
    double R, G, B, L, M, S, l, a, b;

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            color = image.pixel(i, j);

            R = qRed  (color);
            G = qGreen(color);
            B = qBlue (color);

            R = std::max( R * 0.00361399, 0.01176470588);
            G = std::max( G * 0.00361399, 0.01176470588);
            B = std::max( B * 0.00361399, 0.01176470588);

            L = 0.3811*R + 0.5783*G + 0.0402*B;     L = log10(L);//L = qLn(L) / 2.3025850929;
            M = 0.1967*R + 0.7244*G + 0.0782*B;     M = log10(M);//M = qLn(M) / 2.3025850929;
            S = 0.0241*R + 0.1288*G + 0.8444*B;     S = log10(S);//S = qLn(S) / 2.3025850929;

            l = 0.5774*( L + M + S);
            a = 0.4082*( L + M - 2.0*S);
            b = 0.7071*( L - M);

            image_lab[i][j] = QVector3D(l, a, b);
        }
    }
}
// rewrited
void ImageStructure::   calculate_moments()
{
    int h = image.height(),
        w = image.width();

    double  expectedValueL = 0,
            expectedValueA = 0,
            expectedValueB = 0,
            standardDeviationL = 0,
            standardDeviationA = 0,
            standardDeviationB = 0;

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            //  Expected Value
            expectedValueL += image_lab[i][j].x();;
            expectedValueA += image_lab[i][j].y();
            expectedValueB += image_lab[i][j].z();
        }
    }

    double N = h*w;

    expectedValueL /= N;
    expectedValueA /= N;
    expectedValueB /= N;

    expected_value.setX(expectedValueL);
    expected_value.setY(expectedValueA);
    expected_value.setZ(expectedValueB);

    double L, A, B;

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            L = image_lab[i][j].x() - expectedValueL;
            A = image_lab[i][j].y() - expectedValueA;
            B = image_lab[i][j].z() - expectedValueB;

            //  Standard Deviation
            standardDeviationL += L*L;
            standardDeviationA += A*A;
            standardDeviationB += B*B;
        }
    }

    standardDeviationL = qSqrt( standardDeviationL/N );
    standardDeviationA = qSqrt( standardDeviationA/N );
    standardDeviationB = qSqrt( standardDeviationB/N );

    standard_deviation.setX( standardDeviationL );
    standard_deviation.setY( standardDeviationA );
    standard_deviation.setZ( standardDeviationB );
}
// rewrited
void ImageStructure::   convert_LAB_to_RGB(QPoint upPoint, QPoint downPoint)
{

    int h = upPoint.y() - downPoint.y() + 1, //image.height(),
        w = upPoint.x() - downPoint.x() + 1; //image.width();

    if( downPoint.x() == std::numeric_limits<int>::max() )
    {
        downPoint = QPoint(image.width() - 1, image.height() - 1);
    }

    double R, G, B, L, M, S, l, a, b;

    for(int i = upPoint.x(), j; i < w; i++)
    {
        for(j = upPoint.y(); j < h; j++)
        {
            l = image_lab[i][j].x();
            a = image_lab[i][j].y();
            b = image_lab[i][j].z();

            L = 0.5774*l + 0.4082*a + 0.7071*b;
            M = 0.5774*l + 0.4082*a - 1.4142*b; ///!!!
            S = 0.5774*l - 0.4082*a; ///!!!

            L = qPow(10, L);
            M = qPow(10, M);
            S = qPow(10, S);

            R =  4.4679*L - 3.5873*M + 0.1193*S;
            G = -1.2186*L + 2.3809*M - 0.1624*S;
            B =  0.0497*L - 0.2439*M + 1.2045*S;

            R = std::min( R*276.7021, 255.0);
            G = std::min( G*276.7021, 255.0);
            B = std::min( B*276.7021, 255.0);

            QColor color(R, G, B);
            image.setPixel(i, j, color.rgba());
        }
    }


    /* what it is ?
    for(int i = w/2, j; i < w; i++)
    {
        for(j = h/2; j < h; j++)
        {
            QColor color(0, 0, 0);
            image.setPixel(i, j, color.rgba());
        }
    }
    //**/

    qDebug() << "inner convertation done";
}

void ImageStructure::   convert_LAB_to_RGB()
{

    int h = image.height(),
        w = image.width();

    double R, G, B, L, M, S, l, a, b;

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            l = image_lab[i][j].x();
            a = image_lab[i][j].y();
            b = image_lab[i][j].z();

            L = 0.5774*l + 0.4082*a + 0.7071*b;
            M = 0.5774*l + 0.4082*a - 0.7071*b;//- 1.4142*b; ///!!!
            S = 0.5774*l - 0.8164*a;//- 0.4082*a; ///!!!

            L = qPow(10, L);
            M = qPow(10, M);
            S = qPow(10, S);

            R =  4.4679*L - 3.5873*M + 0.1193*S;
            G = -1.2186*L + 2.3809*M - 0.1624*S;
            B =  0.0497*L - 0.2439*M + 1.2045*S;

            R = std::max( 0.0, std::min( R*276.7021, 255.0) );
            G = std::max( 0.0, std::min( G*276.7021, 255.0) );
            B = std::max( 0.0, std::min( B*276.7021, 255.0) );

            QColor color(R, G, B);
            image.setPixel(i, j, color.rgba());
        }
    }
}

void ImageStructure:: histogramRGB(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                                   QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB)
{


    QImage R = image,
           G = image,
           B = image;

    int h = image.height(),
        w = image.width(),
        r, g, b;
    QVector<QVector3D> RGBh = QVector<QVector3D>(256, QVector3D(0, 0, 0));

    QRgb clrRGB;
    QColor color;

    for( int i = 0, j; i < w; i++ )
    {
        for( j = 0; j < h; j++)
        {
            clrRGB = image.pixel(i, j);

            r = qRed  (clrRGB);
            g = qGreen(clrRGB);
            b = qBlue (clrRGB);

            RGBh[r].setX(RGBh[r].x() + 1);
            RGBh[g].setY(RGBh[g].y() + 1);
            RGBh[b].setZ(RGBh[b].z() + 1);

            color = QColor(r, 0, 0);
            R.setPixel(i, j, color.rgba());

            color = QColor(0, g, 0);
            G.setPixel(i, j, color.rgba());

            color = QColor(0, 0, b);
            B.setPixel(i, j, color.rgba());
        }
    }

    labelR->resize(290, 290);
    setLabel(labelR, &R);
    labelG->resize(290, 290);
    setLabel(labelG, &G);
    labelB->resize(290, 290);
    setLabel(labelB, &B);

    double mx = -1;

    for( int i = 0; i < 256; i++ )
    {   mx = std::max( RGBh[i].x(), std::max( RGBh[i].y(), RGBh[i].z() ) );
        qDebug() << i << " : " << RGBh[i].x() << "; " << RGBh[i].y() << "; " << RGBh[i].z();
    }


    QImage RH = QImage(256, 90, QImage::Format_ARGB32);
    QImage GH = QImage(256, 90, QImage::Format_ARGB32);
    QImage BH = QImage(256, 90, QImage::Format_ARGB32);

    int hr, hg, hb;
    for( int i = 0, j; i < 256; i++ )
    {
        hr = std::min( (RGBh[i].x()*90.0)/mx, 90.0);
        color = Qt::red;    QRgb clr = color.rgba();
        for( j = 0; j < hr; j++)
        {   RH.setPixel(i, j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hr; j < 90; j++)
        {   RH.setPixel(i, j, clr );}

        hg = std::min( (RGBh[i].y()*90.0)/mx, 90.0);
        color = Qt::green;    clr = color.rgba();
        for( j = 0; j < hg; j++)
        {   GH.setPixel(i, j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hg; j < 90; j++)
        {   GH.setPixel(i, j, clr );}

        hb = std::min( (RGBh[i].z()*90.0)/mx, 90.0);
        color = Qt::blue;    clr = color.rgba();
        for( j = 0; j < hb; j++)
        {   BH.setPixel(i, j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hb; j < 90; j++)
        {   BH.setPixel(i, j, clr );}
    }

    setLabel(labelHistR, &RH);
    setLabel(labelHistG, &GH);
    setLabel(labelHistB, &BH);

}

void ImageStructure:: setLabel(QLabel *labelObj, QImage *img)
{
    int h = img->height(),
        w = img->width(),
        l = labelObj->height();

    QPixmap pxmptmp = QPixmap :: fromImage(*img);

    if( h > w )
    {   pxmptmp = pxmptmp.scaledToHeight(l);}
    else
    {   pxmptmp = pxmptmp.scaledToWidth(l);}

    labelObj->resize(pxmptmp.width(), pxmptmp.height());
    labelObj->setPixmap( pxmptmp );
}


void ImageStructure::   convert()
{
    this->convert_LAB_to_RGB();
}
