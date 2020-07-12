/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

#include <QtCore>


class MRCConstants
{
private:
    int _threadFrequency;
    int _guiUpdateFrequency;
    QString val;
    QFile file;

    /* Fast Path Planning */
    float _FPPBallThreshHold;
    float _FPPRobotThreshHold;
    float _FPPBreakDistance;
    float _FPPSmoothPathResolution;

    /* Soccer constants */
    int _qtPlayers;
    float _maxKickPower;
    float _robotRadius;
    float _ballRadius;

    /* Ball Sensor constants */
    static float _distToConsiderBallMovement;

    /* Colors */
    static std::string red;
    static std::string green;
    static std::string yellow;
    static std::string blue;
    static std::string magenta;
    static std::string cyan;
    static std::string defaultBold;
    static std::string reset;
    float _distToConsiderBallMovement;

    /*Files Path*/
    QString _clusters;

public:
    MRCConstants(QString fileName);


    float getDistToConsiderBallMovement() const;
    float getBallRadius() const;
    float getRobotRadius() const;
    float getMaxKickPower() const;
    int getQtPlayers() const;
    float getFPPSmoothPathResolution() const;
    float getFPPBreakDistance() const;
    float getFPPRobotThreshHold() const;
    float getFPPBallThreshHold() const;
    QString getClusters() const;
    int getThreadFrequency() const;
    int getGuiUpdateFrequency() const;
};

#endif // CONSTANTS_H
