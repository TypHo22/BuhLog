//QT
#include <QtCore>
#include <QPainter>
#include <QApplication>
#include <QLabel>
#include <QColor>
//BuhLog
#include <BuhLog.h>
//STL
#include <math.h>
#include <Mandelbrot.h>

QColor toColor(int val)
{
    qDebug("Calculating color");

    if(val == 0)
        return Qt::black;

    if(val > 0 && val < 3)
        return Qt::red;

    if(val > 3 && val < 10)
        return Qt::blue;

    if(val > 10 && val < 15)
        return Qt::magenta;

    return Qt::green;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BuhLog::startUp();
    BuhLog::configureLogger(true,-1,-1); //SingleFile Logging = false, no file limitation, new file every 10 seconds

    Mandelbrot mandel(256,2);
    mandel.calculateFractal();

    QLabel label;
    label.setBaseSize(500,500);
    QPixmap pix(500,500);
    pix.fill(Qt::black);
    QPainter *paint = new QPainter(&pix);


    for (int y = 0; y < 500; y++) {
        for (int x = 0; x < 500; x++) {

            const int val = mandel.getValAt(x,y);
            QColor c = toColor(val);

            paint->setPen(c);
            paint->drawPoint(x,y);
        }
    }

    delete paint;
    label.setPixmap(pix);
    label.show();
    label.setStyleSheet("QLabel { background-color : white;}");

    BuhLog::shutDown();
    return a.exec();
}




