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

#include "skill_goto.h"

#include <src/entities/player/player.h>

Skill_GoTo::Skill_GoTo() {

}

void Skill_GoTo::configure() {
    _targetPosition = Geometry::Vector2D(0.0, 0.0);
}

float smallestAngleDiff(float target, float source) {
    float a;
    a = fmod(target + 2*M_PI, 2 * M_PI) - fmod(source + 2*M_PI, 2 * M_PI);

    if (a > M_PI) {
        a = a - 2 * M_PI;
    } else if (a < -M_PI) {
        a = a + 2 * M_PI;
    }
    return a;
}

void Skill_GoTo::run() {
    float Kp = 20.0f;
    float Kd = 2.5f;
    static float lastError = 0.0f;

    float rightMotorSpeed;
    float leftMotorSpeed;
    bool reversed = false;

    float robotAngle = player()->getOrientation().value();
    //float angleToTarget = (_targetPosition - player()->getPosition()).angle();
    float angleToTarget = atan2(_targetPosition.y() - player()->getPosition().y(),
                                _targetPosition.x() - player()->getPosition().x());

    float angError = smallestAngleDiff(robotAngle, angleToTarget);
    if(fabs(angError) > M_PI/2.0 + M_PI/20.0) {
        reversed = true;
        robotAngle = Geometry::Angle(robotAngle + M_PI).value();
        // Calculates the error and reverses the front of the robot
        angError = smallestAngleDiff(robotAngle, angleToTarget);
    }

    float motorSpeed = (Kp*angError) + (Kd * (angError - lastError));// + 0.2 * sumErr;
    lastError = angError;

    float baseSpeed = 30;
    // Normalize
    motorSpeed = motorSpeed > baseSpeed ? baseSpeed : motorSpeed;
    motorSpeed = motorSpeed < -baseSpeed ? -baseSpeed : motorSpeed;

    if (motorSpeed > 0) {
        leftMotorSpeed = baseSpeed ;
        rightMotorSpeed = baseSpeed - motorSpeed;
    } else {
        leftMotorSpeed = baseSpeed + motorSpeed;
        rightMotorSpeed = baseSpeed;
    }

    if (reversed) {
        if (motorSpeed > 0) {
            leftMotorSpeed = -baseSpeed + motorSpeed;
            rightMotorSpeed = -baseSpeed ;
        } else {
            leftMotorSpeed = -baseSpeed ;
            rightMotorSpeed = -baseSpeed - motorSpeed;
        }
    }

    setWheelsSpeed(leftMotorSpeed, rightMotorSpeed);
}

void Skill_GoTo::setTargetPosition(const Geometry::Vector2D &targetPosition) {
    _targetPosition = targetPosition;
}
