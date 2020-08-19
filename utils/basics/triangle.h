#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point2d.h"

class Triangle
{
public:
    Triangle();
    Point2d A,B,C;
    double AB, BC, CA;
    double area;
    void calcSidesAndArea(){
        double s;
        AB = sqrt(pow((B.y() - A.y()), 2) + pow((B.x() - A.x()), 2));
        BC = sqrt(pow((C.y() - B.y()), 2) + pow((C.x() - B.x()), 2));
        CA = sqrt(pow((A.y() - C.y()), 2) + pow((A.x() - C.x()), 2));
        s = (AB+BC+CA)/2;
        area = sqrt(abs(s*(s-AB)*(s-BC)*(s-CA)));
    }

};

#endif // TRIANGLE_H
