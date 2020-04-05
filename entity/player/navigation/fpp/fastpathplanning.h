#ifndef FASTPATHPLANNING_H
#define FASTPATHPLANNING_H

#include<entity/player/navigation/navalgorithm.h>
#include<GEARSystem/gearsystem.hh>

class FastPathPlanning : public NavAlgorithm
{
public:
    FastPathPlanning();
    ~FastPathPlanning();

    void reset();
    void addBall(const Position &pos, const Velocity &vel);
    void addOwnRobot(const Position &pos, const Velocity &vel);
    void addEnemyRobot(const Position &pos, const Velocity &vel);
    void addGoalArea(const Position &pos);

    void run();
    bool getPaths(QList<Position> &rec, int h, MRCTimer *timer);

    Angle getDirection() const;
    float getDistance() const;
    QList<Position> getPath() const;
    QList<QList<Position>> getAllPath() const;

private:
    std::pair<Position, Position> findPoint(QList<Position> &list, Position colisionPoint,int iteratorPoints);
    Position hasCollisionAtLine(Position pos);
    void addColisionPosition(Position _pos, Boolean _isBall);
    QList<std::pair<Position, Boolean>> _colisionPosition;
    QList<Position> _path;
    QList<QList<Position>> _allPath;

    // Path smoothing
    float _smoothPathResolution;
    QList<Position> _smoothPath;
    void smoothPath(QList<Position> &path);
    Position getVector(const Position &near, const Position &rand, float vectorLength);

    NavAlgorithm* clone() const;
};

#endif // FASTPATHPLANNING_H
