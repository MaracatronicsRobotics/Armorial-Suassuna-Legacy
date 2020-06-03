#ifndef POINT2D_H
#define POINT2D_H

#include <iostream>
#include <math.h>

class Point2d{
public:
    Point2d() {}
    Point2d(double x, double y)
        : X(x), Y(y) {}

    double x() const { return X; }
    double y() const { return Y; }

    /**
     * Returns the norm of this vector.
     * @return the norm
    */
    double norm() const {
        return sqrt( X * X + Y * Y );
    }

    void setCoords(double x, double y) {
        X = x; Y = y;
    }

    // Print point
    friend std::ostream& operator << ( std::ostream& s, const Point2d& p )  {
      s << p.x() << " " << p.y();
      return s;
    }
private:
    double X;
    double Y;
};

#endif // POINT2D_H
