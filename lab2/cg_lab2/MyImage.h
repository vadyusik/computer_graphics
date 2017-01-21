#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <QObject>

#include <QVector>
#include <QVector3D>

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QColor>

#include <QFileDialog>
#include <QString>
#include <QDebug>

#include <QtMath>

#include <QTime>
#include <QPoint>
#include <QLabel>


class ImageStructure
{
public:

    QImage image;
    QVector< QVector< QVector3D > > image_lab;

    QVector3D expected_value;
    QVector3D standard_deviation;


public:
    ImageStructure() {}

    void set_image(QImage obj);

    void convert();

    void convert_LAB_to_RGB(QPoint upPoint, //= QPoint(0, 0),
                            QPoint downPoint);// = QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max() ));

    void convert_LAB_to_RGB();

    void histogramRGB(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                      QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB);
    void histogramLAB(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                      QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB);
    void histogramHSV(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                      QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB);
    void histogramYIQ(QLabel *labelR, QLabel *labelG, QLabel *labelB,
                      QLabel *labelHistR, QLabel *labelHistG, QLabel *labelHistB);


private:

    void calculate_LAB();
    void calculate_moments();
    void setLabel(QLabel *labelObj, QImage *img);

    QColor      inline LAB_to_RGB(float l, float a, float b);
    QVector3D   inline RGB_to_LAB(QRgb color);

    QVector3D inline HSV_to_RGB(QVector3D clrHSV);
    QVector3D inline RGB_to_HSV(QVector3D clrRGB);

    QVector3D inline RGB_to_YIQ(QVector3D clrRGB);
    QVector3D inline YIQ_to_RGB(QVector3D clrYIQ);
};


class MyImage : public QObject
{
    Q_OBJECT

public:

    ImageStructure mainImage;
    ImageStructure targetImage;
    ImageStructure resultImage;

    bool isMainImageLoaded;
    bool isTargetImageLoaded;
    bool isResultImageExist;

    QVector< QVector< bool > >  isPixelChanged;

public:
    explicit MyImage(QObject *parent = 0);

    void convert();

    void colorCorrection(QLabel *labelObj);

    void load_image(QLabel *labelObj, bool is_main_image = false);

private:

    void setImage(QLabel *labelObj, QImage *img);

signals:

public slots:

};

#endif // MYIMAGE_H
