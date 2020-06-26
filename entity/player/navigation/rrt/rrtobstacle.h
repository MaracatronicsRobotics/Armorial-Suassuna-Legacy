#ifndef RRTOBSTACLE_H
#define RRTOBSTACLE_H

#include <GEARSystem/Types/types.hh>
#include <utils/utils.hh>

class RRTObstacle {
public:
    RRTObstacle(const Position &pos, float radius);
    ~RRTObstacle();

    void setPos(const Position &pos);
    void setPos(float x, float y);
    void setRadius(float radius);

    Position getPos() const;
    float getRadius() const;
private:
    Position _pos;
    float _radius;
};

#endif // RRTOBSTACLE_H
