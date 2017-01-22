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

    if( fileName == NULL )
    {
        qDebug() << "Open File name NOT choosen";
        return;
    }

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

void MyImage:: LABColorCorrection(QLabel *labelObj, bool L_channel, bool A_channel, bool B_channel)
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

    //QVector< QVector< QVector3D > > *lab = &resultImage.image_lab;

    int h = resultImage.image.height(),
        w = resultImage.image.width();

    if( L_channel == true )
    {
        qDebug() << "L = true";
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   resultImage.image_lab[i][j].setX( mainImgEx.x() + koef.x()*( resultImage.image_lab[i][j].x() - targImgEx.x() ) );}
        }
    }
    else {qDebug() << "L = false";}
    if( A_channel == true )
    {
        qDebug() << "A = true";
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   resultImage.image_lab[i][j].setY( mainImgEx.y() + koef.y()*( resultImage.image_lab[i][j].y() - targImgEx.y() ) );}
        }
    }else {qDebug() << "A = false";}
    if( B_channel == true )
    {
        qDebug() << "B = true";
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   resultImage.image_lab[i][j].setZ( mainImgEx.z() + koef.z()*( resultImage.image_lab[i][j].z() - targImgEx.z() ) );}
        }
    }else {qDebug() << "B = false";}


    resultImage.convert_LAB_to_RGB();

    labelObj->resize(310, 310);
    setImage(labelObj, &resultImage.image);

    isResultImageExist = true;
}
void MyImage:: YIQColorCorrection(QLabel *labelObj, bool Y_channel, bool I_channel, bool Q_channel)
{
    if( isMainImageLoaded == false || isTargetImageLoaded == false)
    {
        qDebug() << "NOT enough data to color correction!";
        return;
    }

    QVector3D expectation(0, 0, 0);
    QVector3D standard_deviation(0, 0, 0);

    resultImage.image = targetImage.image;
    QImage img = resultImage.image;

    int h = img.height(),
        w = img.width(),
        N = img.height()*img.width();

    QVector< QVector<QVector3D> > arrayYIQ ( w, QVector< QVector3D> ( h, QVector3D(0.0, 0.0, 0.0) ) );

    for(int i = 0, j; i < w; i++)
    {
        QRgb color_QRGB;

        for(j = 0; j < h; j++)
        {
            color_QRGB = img.pixel(i, j);
            arrayYIQ[i][j] = resultImage.RGB_to_YIQ( QVector3D( qRed(color_QRGB),
                                                                qGreen(color_QRGB),
                                                                qBlue(color_QRGB) ) );

            expectation.setX( expectation.x() + arrayYIQ[i][j].x() );
            expectation.setY( expectation.y() + arrayYIQ[i][j].y() );
            expectation.setZ( expectation.z() + arrayYIQ[i][j].z() );
        }
    }

    expectation.setX( expectation.x()/N );
    expectation.setY( expectation.y()/N );
    expectation.setZ( expectation.z()/N );

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            standard_deviation.setX( ( arrayYIQ[i][j].x() - expectation.x() )*( arrayYIQ[i][j].x() - expectation.x() ) );
            standard_deviation.setY( ( arrayYIQ[i][j].y() - expectation.y() )*( arrayYIQ[i][j].y() - expectation.y() ) );
            standard_deviation.setZ( ( arrayYIQ[i][j].z() - expectation.z() )*( arrayYIQ[i][j].z() - expectation.z() ) );
        }
    }

    standard_deviation.setX( sqrt( standard_deviation.x()/N ) );
    standard_deviation.setY( sqrt( standard_deviation.y()/N ) );
    standard_deviation.setZ( sqrt( standard_deviation.z()/N ) );

    QVector<QVector3D> moments = mainImage.momentsYIQ();

    QVector3D mainImgEx = moments[0],
              koef = moments[1];


    koef.setX( koef.x() / standard_deviation.x() );
    koef.setY( koef.y() / standard_deviation.y() );
    koef.setZ( koef.z() / standard_deviation.z() );

    qDebug() << "M[Y] - " << mainImgEx.x() << "  " << expectation.x();
    qDebug() << "M[I] - " << mainImgEx.y() << "  " << expectation.y();
    qDebug() << "M[Q] - " << mainImgEx.z() << "  " << expectation.z();

    qDebug() << "D[Y1]/D[Y2] - " << koef.x() ;
    qDebug() << "D[I1]/D[Y2] - " << koef.y() ;
    qDebug() << "D[Q1]/D[Y2] - " << koef.z() ;



    if( Y_channel == true )
    {   qDebug() << "Y channel will be changed";
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   arrayYIQ[i][j].setX( mainImgEx.x() + koef.x()*( arrayYIQ[i][j].x() - expectation.x() ) );}
        }
    }
    if( I_channel == true )
    {   qDebug() << "I channel will be changed";
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   arrayYIQ[i][j].setY( mainImgEx.y() + koef.y()*( arrayYIQ[i][j].y() - expectation.y() ) );}
        }
    }
    if( Q_channel == true )
    {   qDebug() << "Q channel will be changed";
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   arrayYIQ[i][j].setZ( mainImgEx.z() + koef.z()*( arrayYIQ[i][j].z() - expectation.z() ) );}
        }
    }


    resultImage.convert_YIQ_to_RGB( arrayYIQ );
    resultImage.set_image( resultImage.image );

    labelObj->resize(310, 310);
    setImage(labelObj, &resultImage.image);

    isResultImageExist = true;
}

void MyImage:: HSVColorCorrection(QLabel *labelObj, bool H_channel, bool S_channel, bool V_channel)
{
    if( isMainImageLoaded == false || isTargetImageLoaded == false)
    {
        qDebug() << "NOT enough data to color correction!";
        return;
    }

    QVector3D expectation(0, 0, 0);
    QVector3D standard_deviation(0, 0, 0);

    resultImage.image = targetImage.image;
    QImage img = resultImage.image;

    int h = img.height(),
        w = img.width(),
        N = img.height()*img.width();

    QVector< QVector<QVector3D> > arrayHSV ( w, QVector< QVector3D> ( h, QVector3D(0.0, 0.0, 0.0) ) );

    for(int i = 0, j; i < w; i++)
    {
        QRgb color_QRGB;

        for(j = 0; j < h; j++)
        {
            color_QRGB = img.pixel(i, j);
            arrayHSV[i][j] = resultImage.RGB_to_HSV( QVector3D( qRed(color_QRGB),
                                                                qGreen(color_QRGB),
                                                                qBlue(color_QRGB) ) );

            expectation.setX( expectation.x() + arrayHSV[i][j].x() );
            expectation.setY( expectation.y() + arrayHSV[i][j].y() );
            expectation.setZ( expectation.z() + arrayHSV[i][j].z() );
        }
    }

    expectation.setX( expectation.x()/N );
    expectation.setY( expectation.y()/N );
    expectation.setZ( expectation.z()/N );

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            standard_deviation.setX( ( arrayHSV[i][j].x() - expectation.x() )*( arrayHSV[i][j].x() - expectation.x() ) );
            standard_deviation.setY( ( arrayHSV[i][j].y() - expectation.y() )*( arrayHSV[i][j].y() - expectation.y() ) );
            standard_deviation.setZ( ( arrayHSV[i][j].z() - expectation.z() )*( arrayHSV[i][j].z() - expectation.z() ) );
        }
    }

    standard_deviation.setX( sqrt( standard_deviation.x()/N ) );
    standard_deviation.setY( sqrt( standard_deviation.y()/N ) );
    standard_deviation.setZ( sqrt( standard_deviation.z()/N ) );

    QVector<QVector3D> moments = mainImage.momentsHSV();

    QVector3D mainImgEx = moments[0],
              koef = moments[1];


    koef.setX( koef.x() / standard_deviation.x() );
    koef.setY( koef.y() / standard_deviation.y() );
    koef.setZ( koef.z() / standard_deviation.z() );

    qDebug() << "M[H] - " << mainImgEx.x() << "  " << expectation.x();
    qDebug() << "M[S] - " << mainImgEx.y() << "  " << expectation.y();
    qDebug() << "M[V] - " << mainImgEx.z() << "  " << expectation.z();

    qDebug() << "D[H1]/D[H2] - " << koef.x() ;
    qDebug() << "D[S1]/D[S2] - " << koef.y() ;
    qDebug() << "D[V1]/D[V2] - " << koef.z() ;



    if( H_channel == true )
    {
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   arrayHSV[i][j].setX( mainImgEx.x() + koef.x()*( arrayHSV[i][j].x() - expectation.x() ) );}
        }
    }
    if( S_channel == true )
    {
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   arrayHSV[i][j].setY( mainImgEx.y() + koef.y()*( arrayHSV[i][j].y() - expectation.y() ) );}
        }
    }
    if( V_channel == true )
    {
        for(int i = 0, j; i < w; i++)
        {
            for(j = 0; j < h; j++)
            {   arrayHSV[i][j].setZ( mainImgEx.z() + koef.z()*( arrayHSV[i][j].z() - expectation.z() ) );}
        }
    }


    resultImage.convert_HSV_to_RGB( arrayHSV );
    resultImage.set_image( resultImage.image );

    labelObj->resize(310, 310);
    setImage(labelObj, &resultImage.image);

    isResultImageExist = true;
}

void MyImage:: save_image(QWidget *wptr)
{
    if( isResultImageExist == false )
    {   return;}

    QString dir = QFileDialog::getExistingDirectory(wptr, tr("Open Directory"),
                                                    "/home/vadya/Dropbox/coding/ComputerGraphic/ImageExamples",
                                                    QFileDialog::ShowDirsOnly); // | QFileDialog::DontResolveSymlinks);

    if( dir == NULL )
    {   return;}

    dir += "/corrected image ";
    QTime tm = QTime::currentTime();
    dir += tm.toString(Qt::ISODate);
    dir +=".jpg";

    resultImage.image.save(dir);
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

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            image_lab[i][j] = RGB_to_LAB( image.pixel(i, j) );
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

    float R, G, B, L, M, S, l, a, b;

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
}

void ImageStructure::   convert_LAB_to_RGB()
{
    int h = image.height(),
        w = image.width();

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            QColor color = LAB_to_RGB(image_lab[i][j].x(),  // l
                                      image_lab[i][j].y(),  // a
                                      image_lab[i][j].z()); // b

            image.setPixel(i, j, color.rgba());
        }
    }

    calculate_moments();
}

void ImageStructure::   convert_YIQ_to_RGB(QVector< QVector<QVector3D> > &arr)
{
    int h = image.height(),
        w = image.width();

    qDebug() << h << " " << w;

    QVector3D clr;
    QColor color;

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            clr = YIQ_to_RGB(arr[i][j]);
            color = QColor( (int)clr.x(), (int)clr.y(), (int)clr.z() );

            image.setPixel(i, j, color.rgba());
        }
    }
}

void ImageStructure::   convert_HSV_to_RGB(QVector< QVector<QVector3D> > &arr)
{
    int h = image.height(),
        w = image.width();

    QVector3D clr;
    QColor color;

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            clr = HSV_to_RGB(arr[i][j]);
            color = QColor( (int)clr.x(), (int)clr.y(), (int)clr.z() );

            image.setPixel(i, j, color.rgba());
        }
    }
}

QVector<QVector3D> ImageStructure::   momentsYIQ()
{
    QVector3D expectation(0, 0, 0);
    QVector3D standard_deviation(0, 0, 0);

    int h = image.height(),
        w = image.width(),
        N = image.height()*image.width();

    QVector< QVector<QVector3D> > arrayYIQ ( w, QVector< QVector3D> ( h, QVector3D(0.0, 0.0, 0.0) ) );

    for(int i = 0, j; i < w; i++)
    {
        QRgb color_QRGB;

        for(j = 0; j < h; j++)
        {
            color_QRGB = image.pixel(i, j);
            arrayYIQ[i][j] = RGB_to_YIQ( QVector3D( qRed(color_QRGB),
                                                    qGreen(color_QRGB),
                                                    qBlue(color_QRGB) ) );

            expectation.setX( expectation.x() + arrayYIQ[i][j].x() );
            expectation.setY( expectation.y() + arrayYIQ[i][j].y() );
            expectation.setZ( expectation.z() + arrayYIQ[i][j].z() );
        }
    }

    expectation.setX( expectation.x()/N );
    expectation.setY( expectation.y()/N );
    expectation.setZ( expectation.z()/N );

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            standard_deviation.setX( ( arrayYIQ[i][j].x() - expectation.x() )*( arrayYIQ[i][j].x() - expectation.x() ) );
            standard_deviation.setY( ( arrayYIQ[i][j].y() - expectation.y() )*( arrayYIQ[i][j].y() - expectation.y() ) );
            standard_deviation.setZ( ( arrayYIQ[i][j].z() - expectation.z() )*( arrayYIQ[i][j].z() - expectation.z() ) );
        }
    }

    standard_deviation.setX( sqrt( standard_deviation.x()/N ) );
    standard_deviation.setY( sqrt( standard_deviation.y()/N ) );
    standard_deviation.setZ( sqrt( standard_deviation.z()/N ) );

    QVector<QVector3D> res;
    res.push_back(expectation);
    res.push_back(standard_deviation);

    return res;
}

QVector<QVector3D> ImageStructure::   momentsHSV()
{
    QVector3D expectation(0, 0, 0);
    QVector3D standard_deviation(0, 0, 0);

    int h = image.height(),
        w = image.width(),
        N = image.height()*image.width();

    QVector< QVector<QVector3D> > arrayHSV ( w, QVector< QVector3D> ( h, QVector3D(0.0, 0.0, 0.0) ) );

    for(int i = 0, j; i < w; i++)
    {
        QRgb color_QRGB;

        for(j = 0; j < h; j++)
        {
            color_QRGB = image.pixel(i, j);
            arrayHSV[i][j] = RGB_to_HSV( QVector3D( qRed(color_QRGB),
                                                    qGreen(color_QRGB),
                                                    qBlue(color_QRGB) ) );

            expectation.setX( expectation.x() + arrayHSV[i][j].x() );
            expectation.setY( expectation.y() + arrayHSV[i][j].y() );
            expectation.setZ( expectation.z() + arrayHSV[i][j].z() );
        }
    }

    expectation.setX( expectation.x()/N );
    expectation.setY( expectation.y()/N );
    expectation.setZ( expectation.z()/N );

    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            standard_deviation.setX( ( arrayHSV[i][j].x() - expectation.x() )*( arrayHSV[i][j].x() - expectation.x() ) );
            standard_deviation.setY( ( arrayHSV[i][j].y() - expectation.y() )*( arrayHSV[i][j].y() - expectation.y() ) );
            standard_deviation.setZ( ( arrayHSV[i][j].z() - expectation.z() )*( arrayHSV[i][j].z() - expectation.z() ) );
        }
    }

    standard_deviation.setX( sqrt( standard_deviation.x()/N ) );
    standard_deviation.setY( sqrt( standard_deviation.y()/N ) );
    standard_deviation.setZ( sqrt( standard_deviation.z()/N ) );

    QVector<QVector3D> res;
    res.push_back(expectation);
    res.push_back(standard_deviation);

    return res;
}

void ImageStructure::   histogramRGB(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                                     QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB,
                                     int histImage_height, int histogram_height)
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

    labelR->resize(histImage_height, histImage_height);   setLabel(labelR, &R);
    labelG->resize(histImage_height, histImage_height);   setLabel(labelG, &G);
    labelB->resize(histImage_height, histImage_height);   setLabel(labelB, &B);

    float mx = -1;

    for( int i = 1; i < 255; i++ )
    {   mx = std::max( std::max( RGBh[i].x(), mx ), std::max( RGBh[i].y(), RGBh[i].z() ) ); }

    //int histImage_height, int histogram_height

    QImage RH = QImage(256, histogram_height, QImage::Format_ARGB32);
    QImage GH = QImage(256, histogram_height, QImage::Format_ARGB32);
    QImage BH = QImage(256, histogram_height, QImage::Format_ARGB32);

    int hr, hg, hb;
    for( int i = 0, j; i < 256; i++ )
    {
        hr = std::min( (RGBh[i].x()*(float)histogram_height)/mx, (float)histogram_height);
        color = Qt::red;    QRgb clr = color.rgba();
        for( j = 0; j < hr; j++)
        {   RH.setPixel(i, histogram_height - 1 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hr; j < histogram_height; j++)
        {   RH.setPixel(i, histogram_height - 1 - j, clr );}

        hg = std::min( (RGBh[i].y()*(float)histogram_height)/mx, (float)histogram_height );
        color = Qt::green;    clr = color.rgba();
        for( j = 0; j < hg; j++)
        {   GH.setPixel(i, histogram_height - 1 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hg; j < histogram_height; j++)
        {   GH.setPixel(i, histogram_height - 1 - j, clr );}

        hb = std::min( (RGBh[i].z()*(float)histogram_height)/mx, (float)histogram_height);
        color = Qt::blue;    clr = color.rgba();
        for( j = 0; j < hb; j++)
        {   BH.setPixel(i, histogram_height - 1 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hb; j < histogram_height; j++)
        {   BH.setPixel(i, histogram_height - 1 - j, clr );}
    }

    labelHistR->setPixmap( QPixmap :: fromImage(RH) );
    labelHistG->setPixmap( QPixmap :: fromImage(GH) );
    labelHistB->setPixmap( QPixmap :: fromImage(BH) );

    qDebug() << "RGB histogram created";
}

void ImageStructure::   histogramLAB(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                                     QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB,
                                     int histImage_height, int histogram_height)
{
    QImage L = image,
           A = image,
           B = image;

    int h = image.height(),
        w = image.width(),
        li, ai, bi;
    double l, a, b;

    QVector<QVector3D> LABhs = QVector<QVector3D>(256, QVector3D(0, 0, 0));

    QColor color;
    for( int i = 0, j; i < w; i++ )
    {
        for( j = 0; j < h; j++)
        {
            l = image_lab[i][j].x();
            a = image_lab[i][j].y();
            b = image_lab[i][j].z();

            color = LAB_to_RGB(l, 0.0, 0.0);
            L.setPixel(i, j, color.rgba());

            color = LAB_to_RGB(0.0, a, 0.0);
            A.setPixel(i, j, color.rgba());

            color = LAB_to_RGB(0.0, 0.0, b);
            B.setPixel(i, j, color.rgba());

            l = (l + 3.343094) * 76.5757937335;     li = std::max( 0, std::min( 255, (int)(l) ) );
            a = (a + 0.890421) * 158.290365125;     ai = std::max( 0, std::min( 255, (int)(a) ) );
            b = (b + 0.165687) * 712.112024123;     bi = std::max( 0, std::min( 255, (int)(b) ) );

            LABhs[li].setX(LABhs[li].x() + 1);
            LABhs[ai].setY(LABhs[ai].y() + 1);
            LABhs[bi].setZ(LABhs[bi].z() + 1);
        }
    }

    labelR->resize(histImage_height, histImage_height);   setLabel(labelR, &L);
    labelG->resize(histImage_height, histImage_height);   setLabel(labelG, &A);
    labelB->resize(histImage_height, histImage_height);   setLabel(labelB, &B);

    float mx = -1;

    for( int i = 0; i < 256; i++ )
    {
        mx = std::max( std::max( LABhs[i].x(), mx ), std::max( LABhs[i].y(), LABhs[i].z() ) );
        //qDebug() << i << " : " <<  LABhs[i].x() << " ; " << LABhs[i].y() << " ; " << LABhs[i].z();
    }

    QImage LH = QImage(256, histogram_height, QImage::Format_ARGB32);
    QImage AH = QImage(256, histogram_height, QImage::Format_ARGB32);
    QImage BH = QImage(256, histogram_height, QImage::Format_ARGB32);

    int hl, ha, hb;
    for( int i = 0, j; i < 256; i++ )
    {
        hl = std::min( (LABhs[i].x()*(float)histogram_height)/mx, (float)histogram_height);
        color = Qt::black;    QRgb clr = color.rgba();
        for( j = 0; j < hl; j++)
        {   LH.setPixel(i, histogram_height - 1 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hl; j < histogram_height; j++)
        {   LH.setPixel(i, histogram_height - 1 - j, clr );}

        ha = std::min( (LABhs[i].y()*(float)histogram_height)/mx, (float)histogram_height);
        color = Qt::yellow;    clr = color.rgba();
        for( j = 0; j < ha; j++)
        {   AH.setPixel(i, histogram_height - 1 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = ha; j < histogram_height; j++)
        {   AH.setPixel(i, histogram_height - 1 - j, clr );}

        hb = std::min( (LABhs[i].z()*(float)histogram_height)/mx, (float)histogram_height);
        color = Qt::blue;    clr = color.rgba();
        for( j = 0; j < hb; j++)
        {   BH.setPixel(i, histogram_height - 1 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hb; j < histogram_height; j++)
        {   BH.setPixel(i, histogram_height - 1 - j, clr );}
    }

    labelHistR->setPixmap( QPixmap :: fromImage(LH) );
    labelHistG->setPixmap( QPixmap :: fromImage(AH) );
    labelHistB->setPixmap( QPixmap :: fromImage(BH) );

    qDebug() << "LAB histogram created";
}

void ImageStructure::   histogramHSV(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                                     QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB,
                                     int histImage_height, int histogram_height)
{
    QImage H = image,
           S = image,
           V = image;

    int h = image.height(),
        w = image.width();
    float hue, s, v;

    QVector<QVector3D> HSVhs = QVector<QVector3D>(256, QVector3D(0, 0, 0));

    QVector3D color, tmp;
    QRgb clr;
    for( int i = 0, j; i < w; i++ )
    {
        for( j = 0; j < h; j++)
        {
            clr = image.pixel(i, j);

            color = RGB_to_HSV(QVector3D( qRed(clr), qGreen(clr), qBlue (clr) ) );

            tmp = HSV_to_RGB( QVector3D( color.x(), 0.0, 0.0 ) );
            H.setPixel(i, j, QColor(tmp.x(), tmp.y(), tmp.z()).rgba());

            tmp = HSV_to_RGB( QVector3D( color.x(), color.y(), color.z() ) );
            S.setPixel(i, j, QColor(tmp.x(), tmp.y(), tmp.z()).rgba());

            tmp = HSV_to_RGB( QVector3D( 0.0, 0.0, color.z() ) );
            H.setPixel(i, j, QColor(tmp.x(), tmp.y(), tmp.z()).rgba());


            /*
            hue = (hue + 3.343094) * 76.5757937335;     li = std::max( 0, std::min( 255, (int)(hue) ) );
            a = (a + 0.890421) * 158.290365125;         ai = std::max( 0, std::min( 255, (int)(a) ) );
            b = (b + 0.165687) * 712.112024123;         bi = std::max( 0, std::min( 255, (int)(b) ) );

            HSVhs[li].setX(HSVhs[li].x() + 1);
            HSVhs[ai].setY(HSVhs[ai].y() + 1);
            HSVhs[bi].setZ(HSVhs[bi].z() + 1);
            */
        }
    }

    labelR->resize(290, 290);   setLabel(labelR, &H);
    labelG->resize(290, 290);   setLabel(labelG, &S);
    labelB->resize(290, 290);   setLabel(labelB, &V);

    /*
    float mx = -1;

    for( int i = 0; i < 256; i++ )
    {
        mx = std::max( std::max( HSVhs[i].x(), mx ), std::max( HSVhs[i].y(), HSVhs[i].z() ) );
        //qDebug() << i << " : " <<  HSVhs[i].x() << " ; " << HSVhs[i].y() << " ; " << HSVhs[i].z();
    }

    QImage LH = QImage(256, 90, QImage::Format_ARGB32);
    QImage AH = QImage(256, 90, QImage::Format_ARGB32);
    QImage BH = QImage(256, 90, QImage::Format_ARGB32);

    int hl, ha, hb;
    for( int i = 0, j; i < 256; i++ )
    {
        hl = std::min( (HSVhs[i].x()*90.0)/mx, 90.0);
        color = Qt::black;    QRgb clr = color.rgba();
        for( j = 0; j < hl; j++)
        {   LH.setPixel(i, 89 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hl; j < 90; j++)
        {   LH.setPixel(i, 89 - j, clr );}

        ha = std::min( (HSVhs[i].y()*90.0)/mx, 90.0);
        color = Qt::yellow;    clr = color.rgba();
        for( j = 0; j < ha; j++)
        {   AH.setPixel(i, 89 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = ha; j < 90; j++)
        {   AH.setPixel(i, 89 - j, clr );}

        hb = std::min( (HSVhs[i].z()*90.0)/mx, 90.0);
        color = Qt::blue;    clr = color.rgba();
        for( j = 0; j < hb; j++)
        {   BH.setPixel(i, 89 - j, clr );}
        color = Qt::white;  clr = color.rgba();
        for( j = hb; j < 90; j++)
        {   BH.setPixel(i, 89 - j, clr );}
    }

    labelHistR->setPixmap( QPixmap :: fromImage(LH) );
    labelHistG->setPixmap( QPixmap :: fromImage(AH) );
    labelHistB->setPixmap( QPixmap :: fromImage(BH) );
    */
}

void ImageStructure::   histogramYIQ(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                                     QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB,
                                     int histImage_height, int histogram_height)
{
    QImage Y = image,
           I = image,
           Q = image;

    int h = image.height(),
        w = image.width();
    float y, i, q;
    int yI, iI, qI;

    QVector<QVector3D> YIQhs = QVector<QVector3D>(256, QVector3D(0, 0, 0));

    QVector3D color, tmp;
    QRgb clr;
    for( int k = 0, j; k < w; k++ )
    {
        for( j = 0; j < h; j++)
        {
            clr = image.pixel(k, j);

            color = RGB_to_YIQ(QVector3D( qRed(clr), qGreen(clr), qBlue (clr) ) );

            tmp = YIQ_to_RGB( QVector3D( color.x(), 0.0, 0.0 ) );
            Y.setPixel(k, j, QColor(tmp.x(), tmp.y(), tmp.z()).rgba());

            tmp = YIQ_to_RGB( QVector3D( 0.0, color.y(), 0.0 ) );
            I.setPixel(k, j, QColor(tmp.x(), tmp.y(), tmp.z()).rgba());

            tmp = YIQ_to_RGB( QVector3D( 0.0, 0.0, color.z() ) );
            Q.setPixel(k, j, QColor(tmp.x(), tmp.y(), tmp.z()).rgba());


            y = color.x();                          yI = std::max( 0, std::min( 255, (int)(y) ) );
            i = (color.y() + 151.98) * 1.18734375;      iI = std::max( 0, std::min( 255, (int)(i) ) );
            q = (color.z() + 134.64) * 1.04691406;      qI = std::max( 0, std::min( 255, (int)(q) ) );

            YIQhs[yI].setX(YIQhs[yI].x() + 1);
            YIQhs[iI].setY(YIQhs[iI].y() + 1);
            YIQhs[qI].setZ(YIQhs[qI].z() + 1);
        }
    }

    labelR->resize( histImage_height, histImage_height );   setLabel(labelR, &Y);
    labelG->resize( histImage_height, histImage_height );   setLabel(labelG, &I);
    labelB->resize( histImage_height, histImage_height );   setLabel(labelB, &Q);


    float mx = -1;
    for( int i = 1; i < 255; i++ )
    {
        mx = std::max( std::max( YIQhs[i].x(), mx ), std::max( YIQhs[i].y(), YIQhs[i].z() ) );
        //qDebug() << i << " : " <<  YIQhs[i].x() << " ; " << YIQhs[i].y() << " ; " << YIQhs[i].z();
    }

    QImage YH = QImage(256, histogram_height, QImage::Format_ARGB32);
    QImage IH = QImage(256, histogram_height, QImage::Format_ARGB32);
    QImage QH = QImage(256, histogram_height, QImage::Format_ARGB32);

    QColor clrH;

    int hy, hi, hq;
    for( int k = 0, j; k < 256; k++ )
    {
        hy = std::min( (YIQhs[k].x()*(float)histogram_height)/mx, (float)histogram_height );
        clrH = Qt::black;    QRgb clr = clrH.rgba();
        for( j = 0; j < hy; j++)
        {   YH.setPixel(k, histogram_height - 1 - j, clr );}
        clrH = Qt::white;  clr = clrH.rgba();
        for( j = hy; j < histogram_height; j++)
        {   YH.setPixel(k, histogram_height - 1 - j, clr );}

        hi = std::min( (YIQhs[k].y()*(float)histogram_height)/mx, (float)histogram_height );
        clrH = Qt::red;    clr = clrH.rgba();
        for( j = 0; j < hi; j++)
        {   IH.setPixel(k, histogram_height - 1 - j, clr );}
        clrH = Qt::white;  clr = clrH.rgba();
        for( j = hi; j < histogram_height; j++)
        {   IH.setPixel(k, histogram_height - 1 - j, clr );}

        hq = std::min( (YIQhs[k].z()*(float)histogram_height)/mx, (float)histogram_height);
        clrH = Qt::green;    clr = clrH.rgba();
        for( j = 0; j < hq; j++)
        {   QH.setPixel(k, histogram_height - 1 - j, clr );}
        clrH = Qt::white;  clr = clrH.rgba();
        for( j = hq; j < histogram_height; j++)
        {   QH.setPixel(k, histogram_height - 1 - j, clr );}
    }

    labelHistR->setPixmap( QPixmap :: fromImage(YH) );
    labelHistG->setPixmap( QPixmap :: fromImage(IH) );
    labelHistB->setPixmap( QPixmap :: fromImage(QH) );

    qDebug() << "YIQ histogram created";
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

QColor inline ImageStructure::      LAB_to_RGB(float l, float a, float b)
{
    float R, G, B, L, M, S;

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

    return QColor(R, G, B);
}

QVector3D inline ImageStructure::   RGB_to_LAB(QRgb color)
{
        double R, G, B, L, M, S, l, a, b;

        R = qRed  (color);
        G = qGreen(color);
        B = qBlue (color);

        R = std::max( R * 0.00361399, 0.01176470588);
        G = std::max( G * 0.00361399, 0.01176470588);
        B = std::max( B * 0.00361399, 0.01176470588);

        L = 0.3811*R + 0.5783*G + 0.0402*B;     L = log10(L);
        M = 0.1967*R + 0.7244*G + 0.0782*B;     M = log10(M);
        S = 0.0241*R + 0.1288*G + 0.8444*B;     S = log10(S);

        l = 0.5774*( L + M + S);
        a = 0.4082*( L + M - 2.0*S);
        b = 0.7071*( L - M);

        return QVector3D(l, a, b);
}

/*
Calculate HSV to RGB
 0 ≤ H < 360, 0 ≤ S ≤ 1 and 0 ≤ V ≤ 1:
*/
QVector3D inline ImageStructure::   HSV_to_RGB(QVector3D clrHSV)
{
    QVector3D resRGB, sat;

    while( clrHSV.x( ) < 0 )
    {  clrHSV.setX( clrHSV.x( ) + 360 ); }
    while( clrHSV.x( ) > 360 )
    {  clrHSV.setX( clrHSV.x( ) - 360 ); }

    if (clrHSV.x() < 120.0)
    {
        sat.setX( (120.0 - clrHSV.x() ) / 60.0 );
        sat.setY( clrHSV.x() / 60.0 );
        sat.setZ( 0 );
    }
    else if (clrHSV.x() < 240.0)
    {
        sat.setX( 0 );
        sat.setY( ( 240 - clrHSV.x() ) / 60.0 );
        sat.setZ( ( clrHSV.x() - 120 ) / 60.0 );
    }
    else
    {
        sat.setX( (clrHSV.x() - 240) / 60.0 );
        sat.setY( 0 );
        sat.setZ( (360 - clrHSV.x()) / 60.0 );
    }

    sat.setX( std::min( sat.x(), (float)1.0 ) );
    sat.setY( std::min( sat.y(), (float)1.0 ) );
    sat.setZ( std::min( sat.z(), (float)1.0 ) );

    resRGB.setX( 255*( 1.0 - clrHSV.y() + clrHSV.y() * sat.x() ) * clrHSV.z() );
    resRGB.setY( 255*( 1.0 - clrHSV.y() + clrHSV.y() * sat.y() ) * clrHSV.z() );
    resRGB.setZ( 255*( 1.0 - clrHSV.y() + clrHSV.y() * sat.z() ) * clrHSV.z() );
    return( resRGB );
}
/*
Calculate RGB to HSV
RGB betweeen 0 and 255
 0 ≤ H < 360, 0 ≤ S ≤ 1 and 0 ≤ V ≤ 1:
*/

QVector3D inline ImageStructure::RGB_to_HSV(QVector3D clrRGB)
{
    clrRGB.setX(clrRGB.x()/255.0);
    clrRGB.setY(clrRGB.y()/255.0);
    clrRGB.setZ(clrRGB.z()/255.0);

    float themin, themax, delta;

    QVector3D resHSV;

    themin = std::min( clrRGB.x(), std::min( clrRGB.y(), clrRGB.z() ) );
    themax = std::max( clrRGB.x(), std::max( clrRGB.y(), clrRGB.z() ) );
    delta = themax - themin;

    resHSV.setZ( themax );
    resHSV.setY( 0 );
    resHSV.setX( 0 );

    if (themax > 0)
    {   resHSV.setY( delta / themax);}

    if (delta > 0)
    {
        if (themax == clrRGB.x() && themax != clrRGB.y() )
        {   resHSV.setX( resHSV.x( ) + ( clrRGB.y() - clrRGB.z() ) / delta );}

        if (themax == clrRGB.y() && themax != clrRGB.z() )
        {   resHSV.setX( resHSV.x( ) + (2 + (clrRGB.z() - clrRGB.x() ) / delta) );}

        if (themax == clrRGB.z() && themax != clrRGB.x() )
        {   resHSV.setX( resHSV.x( ) + 4 + (clrRGB.x() - clrRGB.y()) / delta );}

        resHSV.setX( resHSV.x( )*60 );
    }

    while( resHSV.x( ) < 0 )
    {  resHSV.setX( resHSV.x( ) + 360 ); }
    while( resHSV.x( ) > 360 )
    {  resHSV.setX( resHSV.x( ) - 360 ); }

    return(resHSV);
}

QVector3D inline ImageStructure::RGB_to_YIQ(QVector3D clrRGB)
{
    float Y, I, Q;
    Y = 0.299 * clrRGB.x() + 0.587 * clrRGB.y() + 0.114 * clrRGB.z();
    I = 0.596 * clrRGB.x() - 0.274 * clrRGB.y() - 0.322 * clrRGB.z();
    Q = 0.212 * clrRGB.x() - 0.528 * clrRGB.y() + 0.311 * clrRGB.z();
    return QVector3D(Y, I, Q);
}

QVector3D inline ImageStructure::YIQ_to_RGB(QVector3D clrYIQ)
{
    float R, G, B;
    R = clrYIQ.x() + 0.956 * clrYIQ.y() + 0.623 * clrYIQ.z();
    G = clrYIQ.x() - 0.272 * clrYIQ.y() - 0.648 * clrYIQ.z();
    B = clrYIQ.x() - 1.105 * clrYIQ.y() + 1.705 * clrYIQ.z();

    R = std::max((float)0.0, std::min(R, (float)255) );
    G = std::max((float)0.0, std::min(G, (float)255.0) );
    B = std::max((float)0.0, std::min(B, (float)255.0) );

    return QVector3D(R, G, B);
}

