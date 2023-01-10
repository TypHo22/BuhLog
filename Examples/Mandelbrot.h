#ifndef MANDELBROT_H
#define MANDELBROT_H
const int WIDTH = 500;
const int HEIGHT = 500;

class Mandelbrot
{
public:
    Mandelbrot(int maxIter, int thresHold);

    ~Mandelbrot();

    void calculateFractal();
    int getValAt(int x, int y) const;

private:
    int pixels_[WIDTH][HEIGHT];
    int height_;
    int width_;
    int maxIter_;
    double threshold_;
};

#endif //MANDEBROT_H
