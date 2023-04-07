#include "navigation.h"

Navigation::Navigation(quint8 playerId, WorldMap *worldMap) {
    _worldMap = worldMap;
    _playerId = playerId;
    _univector = new Univector(playerId);
    _gui = nullptr;
}

QList<Geometry::Vector2D> Navigation::getObstacles(bool avoidOpponents, bool avoidTeammates, bool avoidBall){
    Common::Enums::Color ourColor = Suassuna::Constants::teamColor();

    QList<Geometry::Vector2D> obstacles;
    if (avoidOpponents) {
        Common::Enums::Color theirColor;
        if (ourColor == Common::Enums::Color::YELLOW) {
            theirColor = Common::Enums::Color::BLUE;
        } else {
            theirColor = Common::Enums::Color::YELLOW;
        }

        QList<quint8> opPlayers = getWorldMap()->getPlayersIds(theirColor);
        for(int i = 0; i < opPlayers.size(); i++) {
            QPair<Geometry::Vector2D, bool> pos = getWorldMap()->getPlayerPosition(theirColor, opPlayers[i]);
            if(pos.second){
                obstacles.push_back(Geometry::Vector2D(pos.first.x() * 1000.0, pos.first.y() * 1000.0));
            }
        }
    }

    if (avoidTeammates) {
        QList<quint8> allyPlayers = getWorldMap()->getPlayersIds(ourColor);
        for(int i = 0; i < allyPlayers.size(); i++) {
            if(allyPlayers[i] != _playerId) {
                QPair<Geometry::Vector2D, bool> pos = getWorldMap()->getPlayerPosition(ourColor, allyPlayers[i]);
                if(pos.second){
                    obstacles.push_back(Geometry::Vector2D(pos.first.x() * 1000.0, pos.first.y() * 1000.0));
                }
            }
        }
    }

    if (avoidBall) {
        Geometry::Vector2D ballPos = getWorldMap()->getBall().getPosition();
        obstacles.push_back(ballPos);
    }

    return obstacles;
}

QVector<float> Navigation::getVector(Geometry::Vector2D robotPos, Geometry::Vector2D targetPos, bool avoidOpponents, bool avoidTeammates, bool avoidBall){
    QList<Geometry::Vector2D> obstacles = getObstacles(avoidOpponents, avoidTeammates, avoidBall);
    QVector<float> uni = _univector->generateUnivectorField(robotPos, targetPos, obstacles, QPair<float, float>(0, 0), QPair<float, float>(0, 0));
//    QVector<float> uni = _univector->generateHyperbolicField(targetPos, robotPos);
    return uni;
}

Geometry::Angle Navigation::angleToTarget(Geometry::Vector2D robotPos, Geometry::Vector2D targetPos, bool avoidOpponents, bool avoidTeammates, bool avoidBall){
    robotPos = Geometry::Vector2D(1000 * robotPos.x(), 1000 * robotPos.y());
    targetPos = Geometry::Vector2D(1000 * targetPos.x(), 1000 * targetPos.y());
    //std::cout << "robotPos: " << robotPos.x() << " " << robotPos.y() << " targetPos: " << targetPos.x() << " " << targetPos.y() << std::endl;
    QVector<float> v = getVector(robotPos, targetPos, avoidOpponents, avoidTeammates, avoidBall);
    Geometry::Vector2D resultPos = Geometry::Vector2D(robotPos.x() + 100 * v[0], robotPos.y() + 100 * v[1]);
    Geometry::Angle angle = Geometry::Angle(atan2(resultPos.y() - robotPos.y(), resultPos.x() - robotPos.x()));
    //std::cout << "V: " << v[0] << " " << v[1] << std::endl;
    return angle;
}

WorldMap *Navigation::getWorldMap(){
    if(_worldMap == nullptr) {
        spdlog::info("[ERROR] WorldMap with nullptr value at Navigation.");
    } else {
        return _worldMap;
    }

    return nullptr;
}
