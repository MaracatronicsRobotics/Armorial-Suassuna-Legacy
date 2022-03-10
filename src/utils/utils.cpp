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

#include "utils.h"

Utils::Utils() {

}

Robot Utils::getRobot(int robotID, bool isBlue) {
    //Returns a blank robot object with ID: robotID; and Color: isBlue
    Robot robot;

    RobotIdentifier robotIdentifier = getRobotID(robotID, isBlue);
    Position robotPos = getPosition(0.0f, 0.0f, 0.0f, true);
    Angle robotAngle = getAngle(0.0f, false, true);
    Velocity robotVel = getVelocity(0.0f, 0.0f, 0.0f, true);
    Acceleration robotAcceleration = getAcceleration(0.0f, 0.0f, 0.0f, true);
    AngularSpeed robotAngularSpd = getAngularSpeed(0.0f, false, true);
    RobotStatus robotStatus = getRobotStatus(robotID, isBlue);

    robot.set_allocated_robotidentifier(&robotIdentifier);
    robot.set_allocated_robotposition(&robotPos);
    robot.set_allocated_robotorientation(&robotAngle);
    robot.set_allocated_robotacceleration(&robotAcceleration);
    robot.set_allocated_robotangularspeed(&robotAngularSpd);
    robot.set_allocated_robotstatus(&robotStatus);

    return robot;
}

Ball Utils::getBall() {
    Ball ball;

    Position ballPos = getPosition(0.0f, 0.0f, 0.0f, true);
    Velocity ballVel = getVelocity(0.0f, 0.0f, 0.0f, true);
    Acceleration ballAcceleration = getAcceleration(0.0f, 0.0f, 0.0f, true);

    ball.set_allocated_ballposition(&ballPos);
    ball.set_allocated_ballvelocity(&ballVel);
    ball.set_allocated_ballacceleration(&ballAcceleration);

    return ball;
}

Field Utils::getField() {
    Field field;

    field.set_fieldlength(0.0f);
    field.set_fieldwidth(0.0f);
    field.set_goalwidth(0.0f);
    field.set_goaldepth(0.0f);
    field.set_defensewidth(0.0f);
    field.set_defenselength(0.0f);
    field.set_boundarywidth(0.0f);
    field.set_centerradius(0.0f);

    return field;
}

ControlPacket Utils::getVoidControlPacket() {
    //This method returns a void controlPacket

    RobotIdentifier baseID;
    Color *baseColor = new Color();
    baseColor->set_isblue(false);

    baseID.set_robotid(0);
    baseID.set_allocated_robotcolor(baseColor);

    Velocity baseVel = getVelocity(0.0f, 0.0f, 0.0f, true);
    Velocity baseKickSpeed = getVelocity(0.0f, 0.0f, 0.0f, true);

    AngularSpeed baseAngularSpeed = getAngularSpeed(0.0f, false, true);

    ControlPacket cp;
    cp.set_allocated_robotidentifier(&baseID);
    cp.set_allocated_robotvelocity(&baseVel);
    cp.set_allocated_robotangularspeed(&baseAngularSpeed);
    cp.set_allocated_kickspeed(&baseKickSpeed);
    cp.set_dribbling(false);
    cp.set_w1(0.0f);
    cp.set_w2(0.0f);
    cp.set_w3(0.0f);
    cp.set_w4(0.0f);

    return cp;
}

ControlPacket Utils::getControlPacket(int robotID, bool isBlue) {
    //This method returns a controlPacket prepared for Robot #robotID Team isBlue

    RobotIdentifier robotIdentifier = getRobotID(robotID, isBlue);

    ControlPacket cp = getVoidControlPacket();
    cp.set_allocated_robotidentifier(&robotIdentifier);

    return cp;
}

//General methods
Position Utils::getPosition(float x, float y, float z, bool isInvalid) {
    Position pos;

    pos.set_x(x);
    pos.set_y(y);
    pos.set_z(z);
    pos.set_isinvalid(isInvalid);

    //Here we should check if the position is inside the field

    return pos;
}

Angle Utils::getAngle(float value, bool isInDegrees, bool isInvalid) {
    Angle angle;

    angle.set_value(value);
    angle.set_isindegrees(isInDegrees);
    angle.set_isinvalid(isInvalid);

    //Here we should check if the angle actually exists

    return angle;
}

Velocity Utils::getVelocity(float vx, float vy, float vz, bool isInvalid) {
    Velocity velocity;

    velocity.set_vx(vx);
    velocity.set_vy(vy);
    velocity.set_vz(vz);
    velocity.set_isinvalid(isInvalid);

    //Here we should check if is realistic (not too high i.e.)

    return velocity;
}

Acceleration Utils::getAcceleration(float ax, float ay, float az, bool isInvalid) {
    Acceleration acceleration;

    acceleration.set_ax(ax);
    acceleration.set_ay(ay);
    acceleration.set_az(az);
    acceleration.set_isinvalid(isInvalid);

    //Here we should check if is realistic (not too high i.e.)

    return acceleration;
}

AngularSpeed Utils::getAngularSpeed(float vw, bool isInDegrees, bool isInvalid) {
    AngularSpeed angularSpeed;

    angularSpeed.set_vw(vw);
    angularSpeed.set_isindegrees(isInDegrees);
    angularSpeed.set_isinvalid(isInvalid);

    //Here we should check if is realistic (not too high i.e.)

    return angularSpeed;
}

//Robot info methods
RobotIdentifier Utils::getRobotID(int robotID, bool isBlue) {
    RobotIdentifier robotIdentifier;
    robotIdentifier.set_robotid(robotID);

    Color *color = new Color();
    color->set_isblue(isBlue);

    robotIdentifier.set_allocated_robotcolor(color);

    return robotIdentifier;
}

RobotStatus Utils::getRobotStatus(int robotID, bool isBlue) {
    RobotStatus robotStatus;
    RobotIdentifier robotIdentifier = getRobotID(robotID, isBlue);

    robotStatus.set_allocated_robotidentifier(&robotIdentifier);
    robotStatus.set_batterycharge(0.0f);
    robotStatus.set_capacitorcharge(0.0f);
    robotStatus.set_isdribbling(false);
    robotStatus.set_infrared(false);
    robotStatus.set_kickontouch(false);

    return robotStatus;
}

//getField info methods
FieldLineSegment Utils::getLine(Position p1, Position p2) {
    FieldLineSegment line;
    Position pos1 = getPosition(p1.x(), p1.y(), p1.z(), p1.isinvalid());
    Position pos2 = getPosition(p2.x(), p2.y(), p2.z(), p2.isinvalid());

    //Here we should check if pos1 or pos2 are invalid

    line.set_allocated_p1(&pos1);
    line.set_allocated_p2(&pos2);
    line.set_thickness(0.0f);

    return line;
}

FieldCircularArc Utils::getArc(Position center, float a1, float a2){
    FieldCircularArc arc;
    Position c = getPosition(center.x(), center.y(), center.z(), center.isinvalid());

    //Here we should check if center is invalid

    arc.set_allocated_center(&c);
    arc.set_a1(a1);
    arc.set_a2(a2);
    arc.set_thickness(0.0f);

    return arc;
}

Position Utils::threePoints(const Position &near, const Position &far, float distance, float beta, bool isInDegrees) {
    Angle alpha = getAngle(atan2(far.y() - near.y(), far.x() - near.x()), isInDegrees, false);
    Angle gama = getAngle(alpha.value() + beta, isInDegrees, false);
    Position p = getPosition(near.x() + distance*cos(gama.value()), near.y() + distance*sin(gama.value()), 0.0f, false);

    if (p.isinvalid()) {
        //Here we should verify if p is actually valid. I though something like:
        //p = projectPointAtLine();
    }

    return p;
}

float Utils::distance(const Position &a, const Position &b) {
    return sqrt(pow(a.x()-b.x(),2) + pow(a.y()-b.y(),2));
}

bool Utils::isPointAtLine(const Position &s1, const Position &s2, const Position &point) {
    const Position projectedPoint = Utils::projectPointAtLine(s1, s2, point);
    const float dist = Utils::distance(point, projectedPoint);
    return (dist <= 0.001f);
}

bool Utils::isPointAtSegment(const Position &s1, const Position &s2, const Position &point) {
    const bool isAtLine = Utils::isPointAtLine(s1, s2, point);
    if (!isAtLine) {
        return false;
    } else {
        Position min, max;
        if (s2.x() >= s1.x()) {
            min = getPosition(s1.x(), min.y(), 0.0f, false);
            max = getPosition(s2.x(), max.y(), 0.0f, false);
        } else {
            min = getPosition(s2.x(), min.y(), 0.0f, false);
            max = getPosition(s1.x(), max.y(), 0.0f, false);
        }
        if (s2.y() >= s1.y()) {
            min = getPosition(min.x(), s1.y(), 0.0f, false);
            max = getPosition(max.x(), s2.y(), 0.0f, false);
        } else {
            min = getPosition(min.x(), s2.y(), 0.0f, false);
            max = getPosition(max.x(), s1.y(), 0.0f, false);
        }

        return (point.x() >= min.x() && point.x() <= max.x() && point.y() >= min.y() && point.y() <= max.y());
    }
}

Position Utils::projectPointAtLine(const Position &s1, const Position &s2, const Position &point) {
    const Position a = getPosition(point.x() - s1.x(), point.y() - s1.y(), point.z() - s1.z(), false);
    const Position b = getPosition(s2.x() - s1.x(), s2.y() - s1.y(), s2.z() - s1.z(), false);
    const float bModule = sqrt(pow(b.x(), 2) + pow(b.y(), 2));
    const Position bUnitary = getPosition(b.x() / bModule, b.y() / bModule, b.z(), false);
    const float scalar = Utils::scalarProduct(a, bUnitary);

    return getPosition(s1.x() + (scalar * bUnitary.x()), s1.y() + (scalar * bUnitary.y()), 0.0f, false);
}

Position Utils::projectPointAtSegment(const Position &s1, const Position &s2, const Position &point) {
    const Position projectedPoint = Utils::projectPointAtLine(s1, s2, point);
    if(Utils::isPointAtSegment(s1, s2, projectedPoint)) {
        return projectedPoint;
    }
    else {
        const float d1 = Utils::distance(projectedPoint, s1);
        const float d2 = Utils::distance(projectedPoint, s2);
        return (d1<=d2)? s1 : s2;
    }
}

float Utils::distanceToLine(const Position &s1, const Position &s2, const Position &point) {
    const Position projectedPoint = Utils::projectPointAtLine(s1, s2, point);
    const float distance = Utils::distance(point, projectedPoint);
    return (distance <= 0.001f) ? 0.0f : distance;
}

float Utils::distanceToSegment(const Position &s1, const Position &s2, const Position &point) {
    const Position projectedPoint = Utils::projectPointAtLine(s1, s2, point);
    if(Utils::isPointAtSegment(s1, s2, projectedPoint)) {
        return distanceToLine(s1, s2, point);
    }
    else {
        const float d1 = Utils::distance(point, s1);
        const float d2 = Utils::distance(point, s2);
        return (d1 <= d2)? d1 : d2;
    }
}
