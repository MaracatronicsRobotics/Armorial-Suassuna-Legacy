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

Utils::Utils(Constants *constants) {
    _constants = constants;

    RobotIdentifier baseID;
    Color *baseColor = new Color();
    baseColor->set_isblue(false);

    baseID.set_robotid(0);
    baseID.set_allocated_robotcolor(baseColor);

    Velocity baseVel = getVelocity(0.0f, 0.0f, 0.0f, true);

    AngularSpeed baseAngularSpeed = getAngularSpeed(0.0f, false, true);

    _baseControlPacket.set_allocated_robotidentifier(&baseID);
    _baseControlPacket.set_allocated_robotvelocity(&baseVel);
    _baseControlPacket.set_allocated_robotangularspeed(&baseAngularSpeed);
    _baseControlPacket.set_allocated_kickspeed(&baseVel);
    _baseControlPacket.set_dribbling(false);
    _baseControlPacket.set_w1(0.0f);
    _baseControlPacket.set_w2(0.0f);
    _baseControlPacket.set_w3(0.0f);
    _baseControlPacket.set_w4(0.0f);
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

    RobotIdentifier baseID = _baseControlPacket.robotidentifier();
    Velocity baseVel = _baseControlPacket.robotvelocity();
    AngularSpeed baseAngularSpd = _baseControlPacket.robotangularspeed();
    Velocity baseKickSpd = _baseControlPacket.kickspeed();

    ControlPacket cp;
    cp.set_allocated_robotidentifier(&baseID);
    cp.set_allocated_robotvelocity(&baseVel);
    cp.set_allocated_robotangularspeed(&baseAngularSpd);
    cp.set_allocated_kickspeed(&baseKickSpd);
    cp.set_dribbling(_baseControlPacket.dribbling());
    cp.set_w1(_baseControlPacket.w1());
    cp.set_w2(_baseControlPacket.w2());
    cp.set_w3(_baseControlPacket.w3());
    cp.set_w4(_baseControlPacket.w4());

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

    return pos;
}

Angle Utils::getAngle(float value, bool isInDegrees, bool isInvalid) {
    Angle angle;

    angle.set_value(value);
    angle.set_isindegrees(isInDegrees);
    angle.set_isinvalid(isInvalid);

    return angle;
}

Velocity Utils::getVelocity(float vx, float vy, float vz, bool isInvalid) {
    Velocity velocity;

    velocity.set_vx(vx);
    velocity.set_vy(vy);
    velocity.set_vz(vz);
    velocity.set_isinvalid(isInvalid);

    return velocity;
}

Acceleration Utils::getAcceleration(float ax, float ay, float az, bool isInvalid) {
    Acceleration acceleration;

    acceleration.set_ax(ax);
    acceleration.set_ay(ay);
    acceleration.set_az(az);
    acceleration.set_isinvalid(isInvalid);

    return acceleration;
}

AngularSpeed Utils::getAngularSpeed(float vw, bool isInDegrees, bool isInvalid) {
    AngularSpeed angularSpeed;

    angularSpeed.set_vw(vw);
    angularSpeed.set_isindegrees(isInDegrees);
    angularSpeed.set_isinvalid(isInvalid);

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

    line.set_allocated_p1(&pos1);
    line.set_allocated_p2(&pos2);
    line.set_thickness(0.0f);

    return line;
}

FieldCircularArc Utils::getArc(Position center, float a1, float a2){
    FieldCircularArc arc;
    Position c = getPosition(center.x(), center.y(), center.z(), center.isinvalid());

    arc.set_allocated_center(&c);
    arc.set_a1(a1);
    arc.set_a2(a2);
    arc.set_thickness(0.0f);

    return arc;
}
