#include "MyImage.h"

MyImage::MyImage(QObject *parent) :
    QObject(parent)
{   }

void MyImage::  load_image(QString fileName, bool is_main_image = false)
{
    //QFileDialog::getOpenFileName(this, tr("Open File"), "/home/vadya/Dropbox/coding/ComputerGraphic/ImageExamples", tr("Images (*.png *.xpm *.jpg)"));

    if( is_main_image == true )
    {
        mainImage.set_image     ( QImage(fileName) );
        qDebug() << mainImage.image.size().height() << " "
                 << mainImage.image.size().width();
    }else
    {
        targetImage.set_image   ( QImage(fileName) );
        qDebug() <<  targetImage.image.size().height() << " "
                  << targetImage.image.size().width();
    }


    /*
    qDebug() << "IN void MyImage::  slotLoadImage";
    QString fileName =
         QFileDialog::getOpenFileName(label_obj, tr("Open File"),
                                      "/home/vadya/Dropbox/coding/ComputerGraphic/ImageExamples",
                                       tr("Images (*.png *.xpm *.jpg)"));
    if( is_main_image == true )
    {
        mainImage.set_image( QImage(fileName) );

        label_obj->setPixmap(QPixmap::fromImage(mainImage.image));


        /*
        QPixmap bkgnd( QPixmap::fromImage(mainImage.image) );

        if( mainImage.image.size().height() > mainImage.image.size().width() )
        {

            double w = 720 * ( (double)( mainImage.image.size().width() )/
                               (double)( mainImage.image.size().height() ) );

            bkgnd = bkgnd.scaled(w, 720, Qt::IgnoreAspectRatio);
            widgetObj->resize(w, 720);
        }else
        {
            double h = 720 * ( (double)( mainImage.image.size().height() )/
                               (double)( mainImage.image.size().width() ) );
            bkgnd = bkgnd.scaled(720, h, Qt::IgnoreAspectRatio);
            widgetObj->resize(720, h);
        }

        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        widgetObj->setPalette(palette);
        //


    }else
    {
        targetImage.set_image( QImage(fileName) );
    }
    */
    //widgetO
}


void MyImage::  convert()
{
    this->mainImage.convert();
}


//// IMAGE STRUCTURE

void ImageStructure::   set_image(QImage obj)
{
    QTime time, eachtime;
    time.start();   eachtime.start();

    image = obj;
        //qDebug() << "Time till set image " << eachtime.elapsed();   eachtime.start();

    calculate_LAB();
        //qDebug() << "Time till convert into LAB " << eachtime.elapsed();   eachtime.start();

    calculate_moments();
        //qDebug() << "Time till calculate moments " << eachtime.elapsed();

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

            R = std::max( R * 0.00361399462, 0.01176470588);
            G = std::max( G * 0.00361399462, 0.01176470588);
            B = std::max( B * 0.00361399462, 0.01176470588);

            L = 0.3811*R + 0.5783*G + 0.0402*B;     L = qLn(L) / 2.3025850929;
            M = 0.1967*R + 0.7244*G + 0.0782*B;     M = qLn(M) / 2.3025850929;
            S = 0.0241*R + 0.1288*G + 0.8444*B;     S = qLn(S) / 2.3025850929;

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
    double L, A, B;


    for(int i = 0, j; i < w; i++)
    {
        for(j = 0; j < h; j++)
        {
            L = image_lab[i][j].x();
            A = image_lab[i][j].y();
            B = image_lab[i][j].z();

            //  Expected Value
            expectedValueL += L;
            expectedValueA += A;
            expectedValueB += B;

            //  Standard Deviation
            standardDeviationL += L*L;
            standardDeviationA += A*A;
            standardDeviationB += B*B;
        }
    }

    double N = h*w;

    expected_value.setX(expectedValueL/N);
    expected_value.setY(expectedValueA/N);
    expected_value.setZ(expectedValueB/N);

    standard_deviation.setX( qSqrt(standardDeviationL/N - expectedValueL*expectedValueL) );
    standard_deviation.setY( qSqrt(standardDeviationA/N - expectedValueA*expectedValueA) );
    standard_deviation.setZ( qSqrt(standardDeviationB/N - expectedValueB*expectedValueB) );
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

void ImageStructure::   convert()
{
    this->convert_LAB_to_RGB();
}
