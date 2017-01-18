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

private:

    void calculate_LAB();
    void calculate_moments();

    void convert_LAB_to_RGB(QPoint upPoint = QPoint(0, 0),
                            QPoint downPoint = QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max() ));
};




class MyImage : public QObject
{
    Q_OBJECT

private:

    ImageStructure mainImage;
    ImageStructure targetImage;
    ImageStructure resultImage;

    QVector< QVector< bool > >  isPixelChanged;

public:
    explicit MyImage(QObject *parent = 0);

    void convert();

    void load_image(QString fileName, bool is_main_image = false);

signals:

public slots:


};

#endif // MYIMAGE_H
