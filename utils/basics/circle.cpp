#include "circle.h"

Circle::Circle(double R, Point2d& C) : r(R), c(C)
{
}

Circle::Circle(double R, double X, double Y) : r(R), c(X, Y)
{
}

size_t Circle::intersect(const Circle &C2, Point2d &i1, Point2d &i2){
    // distance between the centers
    double d = Point2d(c.x() - C2.c.x(),
    c.y() - C2.c.y()).norm();

    // find number of solutions
    if(d > r + C2.r) // circles are too far apart, no solution(s)
    {
        std::cout << "Circles are too far apart\n";
        return 0;
    }
    else if(d == 0 && r == C2.r) // circles coincide
    {
        std::cout << "Circles coincide\n";
        return 0;
    }
    // one circle contains the other
    else if(d + std :: min(r, C2.r) < std :: max(r, C2.r))
    {
        std::cout << "One circle contains the other\n";
        return 0;
    }
    else
    {
        double a = (r*r - C2.r*C2.r + d*d)/ (2.0*d);
        double h = sqrt(r*r - a*a);

        // find p2
        Point2d p2( c.x() + (a * (C2.c.x() - c.x())) / d,
                c.y() + (a * (C2.c.y() - c.y())) / d);

        // find intersection points p3
        i1.setCoords( p2.x() + (h * (C2.c.y() - c.y())/ d),
                p2.y() - (h * (C2.c.x() - c.x())/ d)
        );
        i2.setCoords( p2.x() - (h * (C2.c.y() - c.y())/ d),
                p2.y() + (h * (C2.c.x() - c.x())/ d)
        );

        if(d == r + C2.r)
            return 1;
        return 2;
    }
}
