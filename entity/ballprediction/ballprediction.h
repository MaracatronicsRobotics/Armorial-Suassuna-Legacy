#ifndef BALLPREDICTION_H
#define BALLPREDICTION_H

#include <GEARSystem/gearsystem.hh>
#include <entity/entity.h>
#include <include/timer.h>
#include <entity/contromodule/mrcteam.h>


class BallPrediction : public Entity
{
public:
    BallPrediction();
    QString name();

    // Set functions
    void setMinDataSize(int qt);
    void setTeam(MRCTeam *team);
    void setMinVelocity(double minVelocity);

private:
    // Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    // Auxiliary functions
    bool nobodyHasPoss();
    bool hasSufficientVelocity();

    // Vars
    QList<Position> _posBall;
    QList<Velocity> _velBall;
    QList<double>   _dtBall;
    int _minDataSize;
    double _minVelocity;

    // Timer
    Timer _timer;

    // Team
    MRCTeam *_team;

    // Mutex
    QMutex _ballMutex;
};

#endif // BALLPREDICTION_H
