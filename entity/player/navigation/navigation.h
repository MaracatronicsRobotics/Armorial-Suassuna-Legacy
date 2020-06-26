/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef NAVIGATION_HH
#define NAVIGATION_HH

#include <entity/player/baseplayer.h>
#include <entity/player/navigation/navalgorithm.h>

class Navigation {
public:
    Navigation(Player *player, NavigationAlgorithm *navAlg);

    virtual ~Navigation();

    // Setters
    void setGoal(const Position &destination, const Angle &orientation, bool avoidTeammates, bool avoidOpponents, bool avoidBall, bool avoidGoalArea, bool avoidTheirGoalArea);
    // Linear Error
    void setLError(float lError) { _lError = lError; }
    // Angular Error
    void setAError(float aError) { _aError = aError; }
    // Max Linear Speed
    void setMaxLSpeed(float maxLSpeed) { _maxLSpeed = fabs(maxLSpeed); }
    // Max Angular Speed
    void setMaxASpeed(float maxASpeed) { _maxASpeed = fabs(maxASpeed); }
    // Max Linear Acceleration
    void setMaxLAcceleration(float maxLAcceleration) { _maxLAcceleration = maxLAcceleration; }
    void setNavigationAlgorithm(NavigationAlgorithm *navAlg);

    // Getters
    Angle getDirection() const;
    float getDistance() const;
    QLinkedList<Position> getPath() const;
    // Linear Error
    float getLError() const { return _lError; }
    // Angular Error
    float getAError() const { return _aError; }
    // Max Linear Speed
    float getMaxLSpeed() const { return _maxLSpeed; }
    // Max Angular Speed
    float getMaxASpeed() const { return _maxASpeed; }
    // Max Linear Acceleration
    float getmaxLAcceleration() const { return _maxLAcceleration; }
    // Nav Alg name
    QString getNavAlgName() const { return _navAlg->name();}

private:
    // General access
    Player* _player;
    NavigationAlgorithm *_navAlg;
    mutable QMutex _navAlgMutex;

    // Secondary access
    MRCTeam *_ourTeam;
    MRCTeam *_opTeam;
    Locations *_loc;

    Angle _direction;
    float _distance;

    // Errors
    // Linear Error
    float _lError;
    // Angular Error
    float _aError;

    // Maximum speed
    // Linear Speed
    float _maxLSpeed;
    // Angular Speed
    float _maxASpeed;

    // Maximum linear acceleration
    float _maxLAcceleration;

    float _calculateLinearSpeed(float distError, Velocity velocity);
    float _calculateAngularSpeed(float angGoal, Velocity velocity);
};

#endif // NAVIGATION_HH
