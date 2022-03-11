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
    static Robot robot(int ID, bool isBlue);
    static Ball ball();
    static Field field();

    //ControlPacket methods
    static ControlPacket voidControlPacket();
    static ControlPacket controlPacket(int ID, bool isBlue);

    //General methods
    static Position position(float x, float y, float z, bool isInvalid);
    static Angle angle(float value, bool isInDegrees, bool isInvalid);
    static Velocity velocity(float vx, float vy, float vz, bool isInvalid);
    static Acceleration acceleration(float ax, float ay, float az, bool isInvalid);
    static AngularSpeed angularSpeed(float vw, bool isInDegrees, bool isInvalid);

    //getRobot info methods
    static RobotIdentifier robotID(int ID, bool isBlue);
    static RobotStatus robotStatus(int ID, bool isBlue);

    //getField info methods
    static FieldLineSegment line(Position p1, Position p2);
    static FieldCircularArc arc(Position center, float a1, float a2);

    //algebric methods
    static Position threePoints(const Position &near, const Position &far, float distance, float beta, bool isInDegrees);

    static float distance(const Position &a, const Position &b);

    static bool isPointAtLine(const Position &s1, const Position &s2, const Position &point);
    static bool isPointAtSegment(const Position &s1, const Position &s2, const Position &point);
    static Position projectPointAtLine(const Position &s1, const Position &s2, const Position &point);
    static Position projectPointAtSegment(const Position &s1, const Position &s2, const Position &point);
    static float distanceToLine(const Position &s1, const Position &s2, const Position &point);
    static float distanceToSegment(const Position &s1, const Position &s2, const Position &point);

    static float scalarProduct(const Position &A, const Position &B);
    static float crossProduct(const Position &A, const Position &B);
    static float getPerpendicularCoefficient(const Position &s1, const Position &s2);
    static Position hasInterceptionSegments(const Position &s1, const Position &s2, const Position &s3, const Position &s4);

    static float getAngle(const Position &a, const Position &b);
    static float angleDiff(const float A, const Angle &B);
    static float angleDiff(const Angle &A, const float B);
    static float angleDiff(const Angle &A, const Angle &B);
    static float angleDiff(const float A, const float B);
    static void angleLimitZeroTwoPi(float *angle);

    // REMINDER: vectorSum = v1 + m*v2
    static Position vectorSum(const Position &v1, const Position &v2, float m = 1);
    static Position vectorSum(const Position &v1, const Velocity &v2, float m = 1);
    static Position vectorSum(const Velocity &v1, const Position &v2, float m = 1);
    static Velocity vectorSum(const Velocity &v1, const Velocity &v2, float m = 1);

    static bool checkInterval(double value, double minValue, double maxValue);
    static void limitValue(float *value, float minValue, float maxValue);
    static void limitMinValue(float *value, float minValue);
    static bool approximateToZero(float *value, float error = 1e-3);
};

#endif // UTILS_H
