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
#include <src/entities/worldmap/worldmap.h>
#include <src/services/coach/coachservice.h>
#include <src/utils/text/text.h>

class Utils
{
public:
    Utils();

    // General objects
    static Robot getRobotObject(int ID, bool isBlue);
    static Robot getInvalidRobotObject();
    static Ball getBallObject();
    static Field getFieldObject();

    static void setConstants(Constants *constants);
    static Constants* getConstants();

    static void setWorldMap(WorldMap *worldMap);
    static WorldMap* getWorldMap();

    // ControlPacket methods
    static ControlPacket controlPacket(int ID, bool isBlue, float vx = 0.0f, float vy = 0.0f, float vz = 0.0f
            , float vw = 0.0f, bool isInDegrees = false
            , float vxKick = 0.0f, float vyKick = 0.0f, float vzKick = 0.0f, bool dribleOn = false);

    // General methods
    static Position getPositionObject(float x, float y, float z = 0.0f, bool isInvalid = false);
    static Angle getAngleObject(float value, bool isInDegrees, bool isInvalid);
    static Velocity getVelocityObject(float vx, float vy, float vz, bool isInvalid);
    static Acceleration getAccelerationObject(float ax, float ay, float az, bool isInvalid);
    static AngularSpeed getAngularSpeedObject(float vw, bool isInDegrees, bool isInvalid);

    // Robot info methods
    static RobotIdentifier getRobotIdObject(int ID, bool isBlue);
    static RobotStatus getRobotStatusObject(int ID, bool isBlue);

    // Field info methods
    static FieldLineSegment getFieldLineObject(Position p1, Position p2);
    static FieldCircularArc getFieldArcObject(Position center, float a1, float a2);

    //algebric methods
    static Position threePoints(const Position &near, const Position &far, float distance, float beta, bool isInDegrees = false);

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

    static bool isInsideOurField(const Position &pos);
    static bool isInsideTheirField(const Position &pos);
    static bool isInsideOurArea(const Position &pos, float factor = 1.0);
    static bool isInsideTheirArea(const Position &pos, float factor = 1.0);
    static bool isOutsideField(const Position &pos, float factor = 1.0);
    static bool isOutsideField(const Position &pos, float dx, float dy);
    static bool isInsideField(const Position &pos, float factor = 1.0);
    static bool isInsideField(const Position &pos, float dx, float dy);

private:
    static Constants* _constants;
    static WorldMap* _worldMap;

    static bool _isInsideArea(const Position &pos, float factor, const Position &goalLeftPost, const Position &goalRightDeslocatedPost);
    static bool _isOutsideField(const Position &pos, const float maxX, const float maxY);
};

#endif // UTILS_H
