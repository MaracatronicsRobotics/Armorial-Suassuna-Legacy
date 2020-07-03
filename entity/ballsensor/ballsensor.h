#ifndef BALLSENSOR_H
#define BALLSENSOR_H

#include <GEARSystem/worldmap.hh>
#include <entity/referee/SSLReferee/sslreferee.h>

class BallSensor : public Entity {
private:
    // External access
    SSLReferee *_ref;
    WorldMap *_wm;

    // Internal
    Position _lastBallPos;

    void initialization();
    void loop();
    void finalization();
public:
    BallSensor(SSLReferee *ref, WorldMap *wm);
    QString name();
};


#endif // BALLSENSOR_H
