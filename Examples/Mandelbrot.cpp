#include <Mandelbrot.h>
//STL
#include <complex>
//BuhLog
#include <BuhLog.h>
//Qt
#include <QDebug>

Mandelbrot::Mandelbrot(int maxIter,int thresHold):
    maxIter_(maxIter)
   ,threshold_(thresHold)
{
    qInfo("Created instance of Mandelbrot");
}

Mandelbrot::~Mandelbrot()
{
    qInfo("Deleted instance of Mandelbrot");
}

void Mandelbrot::calculateFractal()
{
    // Iterate over each pixel
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            qDebug("Calculating pixel");
            // Calculate the corresponding complex number c
            std::complex<double> c((double)x / WIDTH * 3.5 - 2.5, (double)y / HEIGHT * 2 - 1);
            std::complex<double> z(0, 0);

            // Iterate z = z^2 + c
            int iterations = 0;
            while (std::abs(z) <= threshold_ && iterations < maxIter_) {
                z = z * z + c;
                iterations++;
            }

            // Set the pixel color based on the number of iterations
            if (iterations == maxIter_) {
                pixels_[x][y] = 0; // 0 if in the Mandelbrot set
            } else {
                pixels_[x][y] = iterations; // Other color based on iteration count
            }
        }
    }
    // Print out the pixels
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                std::cout << pixels_[x][y] << " ";
            }
            std::cout << std::endl;
        }
    qDebug("Mandelbrot completed");
}

int Mandelbrot::getValAt(int x, int y) const
{
    if((x > WIDTH  || x < 0) ||
       (y > HEIGHT || y < 0))
        qCritical("Out of matrix");

    return pixels_[x][y];
}
