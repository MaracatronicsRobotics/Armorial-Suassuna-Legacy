#ifndef FASTPATHPLANNING_H
#define FASTPATHPLANNING_H

#include<entity/player/navigation/navalgorithm.h>
#include<GEARSystem/gearsystem.hh>

class FastPathPlanning : public NavAlgorithm
{
public:
    FastPathPlanning();


    void reset();
    void addBall(const Position &pos, const Velocity &vel);
    void addOwnRobot(const Position &pos, const Velocity &vel);
    void addEnemyRobot(const Position &pos, const Velocity &vel);
    void addGoalArea(const Position &pos);

    void run();

    Angle getDirection() const = 0;
    float getDistance() const;
    QLinkedList<Position> getPath() const;

private:
    std::pair<Position, Position>findAnotherPoint(Position colisionPoint);
    Position hasCollision();
    float distrp(int x1, int y1, int x2, int y2,int px, int py);
    void addColisionPosition(Position _pos, Boolean _isBall);
    QList<std::pair<Position, Boolean>> _colisionPosition;
    QLinkedList<Position> _path;



};

#endif // FASTPATHPLANNING_H
