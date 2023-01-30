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

#include "skill_rotateto.h"

#include <src/entities/player/player.h>

Skill_RotateTo::Skill_RotateTo() {
}

void Skill_RotateTo::configure() {
    _targetAngle = 0.0f;
    _isSimulation = true;
}

float smallestAngleDiff2(float target, float source) {
    float a;
    a = fmod(target + 2*M_PI, 2 * M_PI) - fmod(source + 2*M_PI, 2 * M_PI);

    if (a > M_PI) {
        a = a - 2 * M_PI;
    } else if (a < -M_PI) {
        a = a + 2 * M_PI;
    }
    return a;
}

void Skill_RotateTo::run() {
    float Kp = 20.0f;
    float Ki = 0.0f;
    float Kd = 2.5f;
    static float lastError = 0.0f;
    static float cumulativeError = 0.0f;

    float rightMotorSpeed;
    float leftMotorSpeed;
    bool reversed = false;

    float robotAngle = player()->getOrientation().value();
    float angError = smallestAngleDiff2(robotAngle, _targetAngle);

    if(fabs(angError) > M_PI/2.0 + M_PI/20.0) {
        reversed = true;
        robotAngle = Geometry::Angle(robotAngle + M_PI).value();
        // Calculates the error and reverses the front of the robot
        angError = smallestAngleDiff2(robotAngle, _targetAngle);
    }

    float motorSpeed = (Kp*angError) + (Ki*cumulativeError) + (Kd * (angError - lastError));// + 0.2 * sumErr;
    lastError = angError;
    // Maybe the cumulative error should be apllied before motorSpeed declaration
    cumulativeError += angError;

    // The standard movement of the robots will be a clockwise rotation
    leftMotorSpeed = motorSpeed ;
    rightMotorSpeed = -motorSpeed;

    if (reversed) {
        leftMotorSpeed = -motorSpeed ;
        rightMotorSpeed = motorSpeed;
    }

    setWheelsSpeed(leftMotorSpeed, rightMotorSpeed);
}

void Skill_RotateTo::setTargetAngle(const float &targetAngle) {
    _targetAngle = targetAngle;
}

void Skill_RotateTo::setSimulationGame(const bool &isSimulation) {
    _isSimulation = isSimulation;
}
