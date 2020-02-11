#ifndef FREEANGLES_H
#define FREEANGLES_H

#include "obstacle.h"
#include <entity/contromodule/mrcteam.h>

class FreeAngles {
    private:
        float _initialAngle;
        float _finalAngle;
        bool _obstructed;

        static QList<Obstacle> getObstacles(const Position &watcher, float distanceRadius,int defineObstacle);
        static QList<Obstacle> getObstaclesToWatcher(const Position &watcher, const QList<Obstacle> &obstacles);

        static MRCTeam *_ourTeam;
        static MRCTeam *_opTeam;

    
    public:

        FreeAngles(){
            _initialAngle = 0;
            _finalAngle = 0;
            _obstructed = false;
        }

        void setInitialAngle(float initialAngle){
            _initialAngle = initialAngle;
        }
        void setFinalAngle(float finalAngle){
            _finalAngle = finalAngle;
        }
        void setObstructed(bool obstructed){
            _obstructed = obstructed;
        }

        float getInitialAngle(){
            return _initialAngle;
        }
        float getFinalAngle(){
            return _finalAngle;
        }
        bool getObstruced(){
            return _obstructed;
        }

        //defineObstacle, 0 - Aliados, 1 - Oponentes, 2 - todos, diferente disso o programa retorna erro

        static QList<FreeAngles> getFreeAngles(const Position &watcher, float angleMin, float angleMax, int defineObstacle, float distanceRadius);
        static void initialize(MRCTeam *ourTeam, MRCTeam *opTeam) { FreeAngles::_ourTeam = ourTeam; FreeAngles::_opTeam = opTeam; }

};

#endif
