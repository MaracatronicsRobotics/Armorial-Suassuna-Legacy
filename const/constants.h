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

struct RGBA{
    float r;
    float g;
    float b;
    float a;
    float z;
    RGBA(float red, float green, float blue, float alpha, float zToDraw){
        r = red / 255.0;
        g = green / 255.0;
        b = blue / 255.0;
        a = alpha;
        z = zToDraw;
    }
};

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
    float _distToConsiderBallMovement;

    /* Goalie */
    quint8 _goalieId;

    /*Files Path*/
    QString _clustersPath;
    QString _MLPPath;

    /* PID */
    float _linearKp;
    float _linearKi;
    float _linearKd;

    float _angularKp;
    float _angularKi;
    float _angularKd;

public:
    MRCConstants(QString fileName);

    /* GUI */
    static float robotZ;

    float getDistToConsiderBallMovement() const;
    float getBallRadius() const;
    float getRobotRadius() const;
    float getMaxKickPower() const;
    int getQtPlayers() const;
    float getFPPSmoothPathResolution() const;
    float getFPPBreakDistance() const;
    float getFPPRobotThreshHold() const;
    float getFPPBallThreshHold() const;
    QString getClustersPath() const;
    int getThreadFrequency() const;
    int getGuiUpdateFrequency() const;
    quint8 getGoalieId() const;

    float getLinearKp() const;
    float getLinearKi() const;
    float getLinearKd() const;

    float getAngularKp() const;
    float getAngularKi() const;
    float getAngularKd() const;

    /* Colors */
    static std::string red;
    static std::string green;
    static std::string yellow;
    static std::string blue;
    static std::string magenta;
    static std::string cyan;
    static std::string defaultBold;
    static std::string reset;



    QString getMLPPath() const;
};

#endif // CONSTANTS_H
