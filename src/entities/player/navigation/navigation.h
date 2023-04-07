#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <src/entities/player/player.h>
#include <src/common/constants/constants.h>
#include <src/entities/player/navigation/univector/univector.h>
#include <src/entities/worldmap/worldmap.h>
#include <src/gui/gui.h>
#include <Armorial/Common/Enums/Color/Color.h>
#include <Armorial/Common/Types/Field/Field.h>

class Navigation
{

public:
    Navigation(quint8 playerId, WorldMap *worldMap);
    QVector<float> getVector(Geometry::Vector2D robotPos, Geometry::Vector2D targetPos, bool avoidOpponents, bool avoidTeammates, bool avoidBall);
    Geometry::Angle angleToTarget(Geometry::Vector2D robotPos, Geometry::Vector2D targetPos, bool avoidOpponents, bool avoidTeammates, bool avoidBall);

    void setGUI(GUI *gui) { _gui = gui; }
    GUI *getGUI() { return _gui; }

private:
    Univector *_univector;
    WorldMap *_worldMap;
    quint8 _playerId;

    QVector<float> _univectorOutput;

    GUI *_gui;

    QList<Geometry::Vector2D> getObstacles(bool avoidOpponents, bool avoidTeammates, bool avoidBall);
    WorldMap *getWorldMap();

};

#endif // NAVIGATION_H
