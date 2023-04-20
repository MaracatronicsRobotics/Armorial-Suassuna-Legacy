#include "navigation.h"

Navigation::Navigation(quint8 playerId, WorldMap *worldMap) {
    _worldMap = worldMap;
    _playerId = playerId;
    _univector = new Univector(playerId);
    _gui = nullptr;
}

QList<Geometry::Vector2D> Navigation::getObstacles(bool avoidOpponents, bool avoidTeammates, bool avoidBall){
    QList<Geometry::Vector2D> obstacles;

    Common::Enums::Color ourColor = Suassuna::Constants::teamColor();
    if (avoidOpponents) {
        Common::Enums::Color theirColor;
        if (ourColor == Common::Enums::Color::YELLOW) {
            theirColor = Common::Enums::Color::BLUE;
        } else {
            theirColor = Common::Enums::Color::YELLOW;
        }

        Team* theirTeam = getWorldMap()->getTeam(theirColor);
        for (auto theirPlayer : theirTeam->getPlayers()) {
            Geometry::Vector2D theirPlayerPosition = theirPlayer->getPosition();
            obstacles.push_back(theirPlayerPosition);
        }
    }

    if (avoidTeammates) {
        Team* ourTeam = getWorldMap()->getTeam(ourColor);
        for (auto ourPlayer : ourTeam->getPlayers()) {
            Geometry::Vector2D ourPlayerPosition = ourPlayer->getPosition();
            obstacles.push_back(ourPlayerPosition);
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
    Common::Enums::Color ourColor = Suassuna::Constants::teamColor();

    // Getting Player Velocity
    Geometry::Vector2D playerVelocity = getWorldMap()->getTeam(ourColor)->getPlayer(0).value().getVelocity();
    QPair<float, float> v_robot(playerVelocity.x(), playerVelocity.y());

    // Getting Ball Velocity
    Geometry::Vector2D ballVelocity = getWorldMap()->getBall().getVelocity();
    QPair<float, float> v_ball = {ballVelocity.x(), ballVelocity.y()};
    //std::cout << "Player0_velX: " << playerVelocity.x() << " Player0_velY: " << playerVelocity.y() << "\n";
    QVector<float> uni = _univector->generateUnivectorField(robotPos, targetPos, obstacles, v_robot, QPair<float, float>(0, 0));
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
