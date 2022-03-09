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

#ifndef UTILS_H
#define UTILS_H

#include <proto/messages.grpc.pb.h>
#include <src/constants/constants.h>
#include <src/services/coach/coachservice.h>


class Utils
{
public:
    Utils();

    //General objects
    static Robot getRobot(int robotID, bool isBlue);
    static Ball getBall();
    static Field getField();

    //ControlPacket methods
    static ControlPacket getVoidControlPacket();
    static ControlPacket getControlPacket(int robotID, bool isBlue);

    //General methods
    static Position getPosition(float x, float y, float z, bool isInvalid);
    static Angle getAngle(float value, bool isInDegrees, bool isInvalid);
    static Velocity getVelocity(float vx, float vy, float vz, bool isInvalid);
    static Acceleration getAcceleration(float ax, float ay, float az, bool isInvalid);
    static AngularSpeed getAngularSpeed(float vw, bool isInDegrees, bool isInvalid);

    //getRobot info methods
    static RobotIdentifier getRobotID(int robotID, bool isBlue);
    static RobotStatus getRobotStatus(int robotID, bool isBlue);

    //getField info methods
    static FieldLineSegment getLine(Position p1, Position p2);
    static FieldCircularArc getArc(Position center, float a1, float a2);

    //algebric methods

private:
    Constants *_constants;
    static ControlPacket _baseControlPacket;
};

#endif // UTILS_H
