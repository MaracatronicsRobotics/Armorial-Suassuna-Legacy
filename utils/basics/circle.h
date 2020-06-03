#ifndef CIRCLE_H
#define CIRCLE_H

#include "point2d.h"

class Circle
{
public:
    Circle(double R, Point2d& C);
    Circle(double R, double X, double Y);

    Point2d getC() const { return c; }
    double getR() const { return r; }

    size_t intersect(const Circle& C2, Point2d& i1, Point2d& i2);

    friend std::ostream& operator << ( std::ostream& s, const Circle& C )  {
          s << "Center: " << C.getC() << ", r = " << C.getR();
          return s;
    }

private:
    Point2d c;
    double r;
};

#endif // CIRCLE_H
