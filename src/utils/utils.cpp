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

Constants* Utils::_constants = nullptr;
WorldMap* Utils::_worldMap = nullptr;

Utils::Utils() {

}

Robot Utils::getRobotObject(int ID, bool isBlue) {
    //Returns a blank robot object with ID: robotID; and Color: isBlue
    Robot robot;

    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    robotIdentifier->CopyFrom(Utils::getRobotIdObject(ID, isBlue));
    Position *robotPos = new Position();
    robotPos->CopyFrom(Utils::getPositionObject(0.0f, 0.0f, 0.0f, true));

    Angle *robotAngle = new Angle();
    robotAngle->CopyFrom(Utils::getAngleObject(0.0f, false, true));

    Velocity *robotVel = new Velocity();
    robotVel->CopyFrom(Utils::getVelocityObject(0.0f, 0.0f, 0.0f, true));

    Acceleration *robotAcceleration = new Acceleration();
    robotAcceleration->CopyFrom(Utils::getAccelerationObject(0.0f, 0.0f, 0.0f, true));

    AngularSpeed *robotAngularSpd = new AngularSpeed();
    robotAngularSpd->CopyFrom(Utils::getAngularSpeedObject(0.0f, false, true));

    RobotStatus *robotStats = new RobotStatus();
    robotStats->CopyFrom(Utils::getRobotStatusObject(ID, isBlue));

    robot.set_allocated_robotidentifier(robotIdentifier);
    robot.set_allocated_robotposition(robotPos);
    robot.set_allocated_robotorientation(robotAngle);
    robot.set_allocated_robotacceleration(robotAcceleration);
    robot.set_allocated_robotangularspeed(robotAngularSpd);
    robot.set_allocated_robotstatus(robotStats);

    return robot;
}

Robot Utils::getInvalidRobotObject() {
    return getRobotObject(ROBOT_INVALID_ID, false);
}

Ball Utils::getBallObject() {
    Ball ball;

    Position *ballPos = new Position();
    ballPos->CopyFrom(Utils::getPositionObject(0.0f, 0.0f, 0.0f, true));

    Velocity *ballVel = new Velocity();
    ballVel->CopyFrom(Utils::getVelocityObject(0.0f, 0.0f, 0.0f, true));

    Acceleration *ballAcceleration = new Acceleration();
    ballAcceleration->CopyFrom(Utils::getAccelerationObject(0.0f, 0.0f, 0.0f, true));

    ball.set_allocated_ballposition(ballPos);
    ball.set_allocated_ballvelocity(ballVel);
    ball.set_allocated_ballacceleration(ballAcceleration);

    return ball;
}

Field Utils::getFieldObject() {
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

ControlPacket Utils::controlPacket(int ID, bool isBlue, float vx, float vy
                                   , float vz, float vw, bool isInDegrees
                                   , float vxKick, float vyKick, float vzKick, bool dribleOn) {
    //This method returns a controlPacket prepared for Robot #robotID Team isBlue

    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    robotIdentifier->CopyFrom(Utils::getRobotIdObject(ID, isBlue));

    ControlPacket cp = ControlPacket();
    cp.set_allocated_robotidentifier(robotIdentifier);

    Velocity *robotVel = new Velocity();
    robotVel->CopyFrom(Utils::getVelocityObject(vx, vy, vz, false));

    cp.set_allocated_robotvelocity(robotVel);

    AngularSpeed *angularSpeed = new AngularSpeed();
    angularSpeed->CopyFrom(Utils::getAngularSpeedObject(vw, isInDegrees, false));

    cp.set_allocated_robotangularspeed(angularSpeed);

    Velocity *robotKickSpeed = new Velocity();
    robotKickSpeed->CopyFrom(Utils::getVelocityObject(vxKick, vyKick, vzKick, false));

    cp.set_allocated_kickspeed(robotKickSpeed);

    cp.set_dribbling(dribleOn);

    return cp;
}

// General methods
Position Utils::getPositionObject(float x, float y, float z, bool isInvalid) {
    Position pos;

    pos.set_x(x);
    pos.set_y(y);
    pos.set_z(z);
    pos.set_isinvalid(isInvalid);

    //Here we should check if the position is inside the field

    return pos;
}

Angle Utils::getAngleObject(float value, bool isInDegrees, bool isInvalid) {
    Angle angle;

    angle.set_value(value);
    angle.set_isindegrees(isInDegrees);
    angle.set_isinvalid(isInvalid);

    // Here we should check if the angle actually exists

    return angle;
}

Velocity Utils::getVelocityObject(float vx, float vy, float vz, bool isInvalid) {
    Velocity velocity;

    velocity.set_vx(vx);
    velocity.set_vy(vy);
    velocity.set_vz(vz);
    velocity.set_isinvalid(isInvalid);

    // Here we should check if is realistic (not too high i.e.)

    return velocity;
}

Acceleration Utils::getAccelerationObject(float ax, float ay, float az, bool isInvalid) {
    Acceleration acceleration;

    acceleration.set_ax(ax);
    acceleration.set_ay(ay);
    acceleration.set_az(az);
    acceleration.set_isinvalid(isInvalid);

    //Here we should check if is realistic (not too high i.e.)

    return acceleration;
}

AngularSpeed Utils::getAngularSpeedObject(float vw, bool isInDegrees, bool isInvalid) {
    AngularSpeed angularSpeed;

    angularSpeed.set_vw(vw);
    angularSpeed.set_isindegrees(isInDegrees);
    angularSpeed.set_isinvalid(isInvalid);

    //Here we should check if is realistic (not too high i.e.)

    return angularSpeed;
}

Color Utils::getColorObject(bool isBlue) {
    Color color;

    color.set_isblue(isBlue);

    return color;
}

//Robot info methods
RobotIdentifier Utils::getRobotIdObject(int ID, bool isBlue) {
    RobotIdentifier robotIdentifier;
    robotIdentifier.set_robotid(ID);

    Color *color = new Color();
    color->CopyFrom(getColorObject(isBlue));

    robotIdentifier.set_allocated_robotcolor(color);

    return robotIdentifier;
}

RobotStatus Utils::getRobotStatusObject(int ID, bool isBlue) {
    RobotStatus robotStatus;
    RobotIdentifier *robotIdentifier = new RobotIdentifier();
    robotIdentifier->CopyFrom(getRobotIdObject(ID, isBlue));

    robotStatus.set_allocated_robotidentifier(robotIdentifier);
    robotStatus.set_batterycharge(0.0f);
    robotStatus.set_capacitorcharge(0.0f);
    robotStatus.set_isdribbling(false);
    robotStatus.set_infrared(false);
    robotStatus.set_kickontouch(false);

    return robotStatus;
}

//getField info methods
FieldLineSegment Utils::getFieldLineObject(Position p1, Position p2) {
    FieldLineSegment line;
    Position *pos1 = new Position();
    pos1->CopyFrom(getPositionObject(p1.x(), p1.y(), p1.z(), p1.isinvalid()));
    Position *pos2 = new Position();
    pos2->CopyFrom(getPositionObject(p2.x(), p2.y(), p2.z(), p2.isinvalid()));

    //Here we should check if pos1 or pos2 are invalid

    line.set_allocated_p1(pos1);
    line.set_allocated_p2(pos2);
    line.set_thickness(0.0f);

    return line;
}

FieldCircularArc Utils::getFieldArcObject(Position center, float a1, float a2){
    FieldCircularArc arc;
    Position *c = new Position();
    c->CopyFrom(getPositionObject(center.x(), center.y(), center.z(), center.isinvalid()));

    //Here we should check if center is invalid

    arc.set_allocated_center(c);
    arc.set_a1(a1);
    arc.set_a2(a2);
    arc.set_thickness(0.0f);

    return arc;
}

Position Utils::threePoints(const Position &near, const Position &far, float distance, float beta, bool isInDegrees) {
    Angle alpha = getAngleObject(atan2(far.y() - near.y(), far.x() - near.x()), isInDegrees, false);
    Angle gama = getAngleObject(alpha.value() + beta, isInDegrees, false);
    Position p = getPositionObject(near.x() + distance*cos(gama.value()), near.y() + distance*sin(gama.value()), 0.0f, false);

    if (p.isinvalid()) {
        //Here we should verify if p is actually valid. I though something like:
        //p = projectPointAtLine();
    }

    return p;
}

Position Utils::repulsedPosition(const Position &repulserPosition, float distance, float robotOrientationToRepulser) {
    Position rep = getPositionObject(
        repulserPosition.x() - (distance * cos(robotOrientationToRepulser)),
        repulserPosition.y() - (distance * sin(robotOrientationToRepulser))
    );

    return rep;
}

float Utils::distance(const Position &a, const Position &b) {
    return sqrt(pow(a.x()-b.x(),2) + pow(a.y()-b.y(),2));
}

float Utils::velocityABS(Velocity &vel){
    return sqrt(pow(vel.vx(), 2) + pow(vel.vy(), 2));
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
            min = getPositionObject(s1.x(), min.y(), 0.0f, false);
            max = getPositionObject(s2.x(), max.y(), 0.0f, false);
        } else {
            min = getPositionObject(s2.x(), min.y(), 0.0f, false);
            max = getPositionObject(s1.x(), max.y(), 0.0f, false);
        }
        if (s2.y() >= s1.y()) {
            min = getPositionObject(min.x(), s1.y(), 0.0f, false);
            max = getPositionObject(max.x(), s2.y(), 0.0f, false);
        } else {
            min = getPositionObject(min.x(), s2.y(), 0.0f, false);
            max = getPositionObject(max.x(), s1.y(), 0.0f, false);
        }

        return (point.x() >= min.x() && point.x() <= max.x() && point.y() >= min.y() && point.y() <= max.y());
    }
}

Position Utils::projectPointAtLine(const Position &s1, const Position &s2, const Position &point) {
    const Position a = getPositionObject(point.x() - s1.x(), point.y() - s1.y(), point.z() - s1.z(), false);
    const Position b = getPositionObject(s2.x() - s1.x(), s2.y() - s1.y(), s2.z() - s1.z(), false);
    const float bModule = sqrt(pow(b.x(), 2) + pow(b.y(), 2));
    const Position bUnitary = getPositionObject(b.x() / bModule, b.y() / bModule, b.z(), false);
    const float scalar = Utils::scalarProduct(a, bUnitary);

    return getPositionObject(s1.x() + (scalar * bUnitary.x()), s1.y() + (scalar * bUnitary.y()), 0.0f, false);
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

float Utils::scalarProduct(const Position &A, const Position &B) {
    return A.x()*B.x() + A.y()*B.y();
}

float Utils::crossProduct(const Position &A, const Position &B) {
    return A.x()*B.y() - A.y()*B.x();
}

float Utils::getPerpendicularCoefficient(const Position &s1, const Position &s2) {
    float coefAngular= (s2.y() - s1.y())/(s2.x() - s1.x());
    float perpendicularCoef = -(1.0f/coefAngular);
    return perpendicularCoef;
}

Position Utils::hasInterceptionSegments(const Position &s1, const Position &s2, const Position &s3, const Position &s4) {
    float denominador = (s4.y() - s3.y()) * (s2.x() - s1.x()) - (s4.x() - s3.x()) * (s2.y()-s1.y());

    if (denominador!=0.0f){ // segmentos se interceptam
        float u = ((s4.x() - s3.x())*(s1.y() - s3.y()) - (s4.y() - s3.y())*(s1.x() - s3.x()));
        float v = ((s2.x() - s1.x())*(s1.y() - s3.y()) - (s2.y() - s1.y())*(s1.x() - s3.x()));

        if (u !=0.0f) {

            float xIntersec = s1.x() + (u/denominador)*(s2.x()-s1.x());
            float yIntersec = s1.y() + (u/denominador)*(s2.y()-s1.y());
            return getPositionObject(xIntersec, yIntersec, 0.0f, false);

        } else if (v !=0.0f) {

            float xIntersec = s3.x() + (v/denominador) * (s4.x()-s3.x());
            float yIntersec = s3.y() + (v/denominador) * (s4.y()-s3.y());

            return getPositionObject(xIntersec, yIntersec, 0.0f, false);
        } else { // caso especial -> as linhas sao coincidentes

            return getPositionObject(s1.x(), s1.y(), 0.0f, false);

        }
    } else {
        //Os segmentos sao paralelos
        return getPositionObject(0.0f, 0.0f, 0.0f, true);
    }
}

float Utils::getAngle(const Position &a, const Position &b) {
    return atan2(b.y()-a.y(), b.x()-a.x());
}

float Utils::angleDiff(const float A, const Angle &B) {
    return angleDiff(A, B.value());
}

float Utils::angleDiff(const Angle &A, const float B) {
    return angleDiff(A.value(), B);
}

float Utils::angleDiff(const Angle &A, const Angle &B) {
    return angleDiff(A.value(), B.value());
}

float Utils::angleDiff(const float A, const float B) {
    float diff = fabs(B - A);
    if(diff > M_PI) {
        diff = (2.0 * M_PI) - diff;
    }
    return diff;
}

void Utils::angleLimitZeroTwoPi(float *angle) {
    while(*angle < 0) {
        *angle += (2.0 * M_PI);
    }

    while(*angle > (2.0 * M_PI)) {
        *angle -= (2.0 * M_PI);
    }
}

float Utils::degToRad(float angleInDeg) {
    return (angleInDeg*toRad);
}

float Utils::radToDeg(float angleInRad) {
    return (angleInRad*toDeg);
}

Position Utils::vectorSum(const Position &v1, const Position &v2, float m) {
    return getPositionObject(v1.x() + m * v2.x(), v1.y() + m * v2.y(), 0.0f, false);
}

Position Utils::vectorSum(const Position &v1, const Velocity &v2, float m) {
    return getPositionObject(v1.x() + m * v2.vx(), v1.y() + m * v2.vy(), 0.0f, false);
}

Position Utils::vectorSum(const Velocity &v1, const Position &v2, float m) {
    return getPositionObject(v1.vx() + m * v2.x(), v1.vy() + m * v2.y(), 0.0f, false);
}

Velocity Utils::vectorSum(const Velocity &v1, const Velocity &v2, float m) {
    return getVelocityObject(v1.vx() + m * v2.vx(), v1.vy() + m * v2.vy(), 0.0f, false);

}

float Utils::getVelocityAbs(const Velocity &v) {
    return sqrt(pow(v.vx(), 2) + pow(v.vy(), 2));
}

float Utils::getVelocityArg(const Velocity &v) {
    return atan2(v.vy(), v.vx());
}

bool Utils::checkInterval(double value, double minValue, double maxValue) {
    return (minValue<value && value<maxValue);
}

void Utils::limitValue(float *value, float minValue, float maxValue) {
    if(*value > maxValue) {
        *value = maxValue;
    }
    else if(*value < minValue) {
        *value = minValue;
    }
}

void Utils::limitMinValue(float *value, float minValue) {
    if(minValue == 0) {
        return;
    }

    if((*value) > 0 && (*value) < minValue) {
        (*value) = minValue;
    }
    else if((*value) < 0 && (*value) > -minValue) {
        (*value) = -minValue;
    }
}

bool Utils::approximateToZero(float *value, float error) {
    if(checkInterval((*value), -error, error)) {
        (*value) = 0;
        return true;
    } else {
        return false;
    }
}

bool Utils::isInsideOurField(const Position &pos) {

    return ((getConstants()->getTeamSide().isRight() && pos.x()>=0.0f) || (getConstants()->getTeamSide().isLeft() && pos.x() <= 0.0f));
}
bool Utils::isInsideTheirField(const Position &pos) {
    return (isInsideOurField(pos)==false);
}
bool Utils::isInsideOurArea(const Position &pos, float factor) {
    double x_offset = getConstants()->getTeamSide().isLeft() ? (1.0 * factor) : (-1.0 * factor);
    double y_offset = getConstants()->getTeamSide().isLeft() ? (-0.5 * factor) : (0.5 * factor);

    Position ourGoalLeftPost = getWorldMap()->getLocations()->ourGoalLeftPost();
    Position ourGoalRightPost = getWorldMap()->getLocations()->ourGoalRightPost();

    Position test = getPositionObject(ourGoalLeftPost.x(), ourGoalLeftPost.y() - y_offset);
    Position ourGoalRightDeslocatedPost = getPositionObject(ourGoalRightPost.x() + x_offset, ourGoalRightPost.y() + y_offset );

    return _isInsideArea(pos, factor, test, ourGoalRightDeslocatedPost);
}
bool Utils::isInsideTheirArea(const Position &pos, float factor) {
    double x_offset = getConstants()->getOppositeSide().isLeft() ? (1.0 * factor) : (-1.0 * factor);
    double y_offset = getConstants()->getOppositeSide().isLeft() ? (-0.5 * factor) : (0.5 * factor);

    Position theirGoalLeftPost = getWorldMap()->getLocations()->theirGoalLeftPost();
    Position theirGoalRightPost = getWorldMap()->getLocations()->theirGoalRightPost();

    Position test = getPositionObject(theirGoalLeftPost.x(), theirGoalLeftPost.y() - y_offset);
    Position theirGoalRightDeslocatedPost = getPositionObject(theirGoalRightPost.x() + x_offset, theirGoalRightPost.y() + y_offset);

    return _isInsideArea(pos, factor, test, theirGoalRightDeslocatedPost);
}
bool Utils::isOutsideField(const Position &pos, float factor) {
    float fieldMaxX = getWorldMap()->getLocations()->fieldMaxX();
    float fieldMaxY = getWorldMap()->getLocations()->fieldMaxY();
    return _isOutsideField(pos, factor*fieldMaxX, factor*fieldMaxY);
}
bool Utils::isOutsideField(const Position &pos, const float dx, const float dy) {
    float fieldMaxX = getWorldMap()->getLocations()->fieldMaxX();
    float fieldMaxY = getWorldMap()->getLocations()->fieldMaxY();
    return _isOutsideField(pos, fieldMaxX+dx, fieldMaxY+dy);
}
bool Utils::isInsideField(const Position &pos, float factor) {
    return (!isOutsideField(pos, factor));
}
bool Utils::isInsideField(const Position &pos, float dx, float dy) {
    return (!isOutsideField(pos, dx, dy));
}

QList<Robot> Utils::getAdversariesInRadius(const Position &center, float radius){

    // List of adversaries inside the radius, containing their ids and position
    QList<Robot> adversariesInsideRadiusList;

    Color theirColor;
    theirColor.set_isblue(getConstants()->isTeamBlue() == false);

    // List of all adversaries
    QList<Robot> allAdversariesList = getWorldMap()->getRobots(theirColor);

    for (Robot adversary : allAdversariesList){
        // Distance between the adversary and the center
        Position adversaryPosition = adversary.robotposition();
        float distanceEnemyCenter = Utils::distance(adversaryPosition, center);

        if (distanceEnemyCenter <= radius){
            // If the enemy is inside radius, put it in the enemies inside radius list
            adversariesInsideRadiusList.push_back(adversary);
        }
    }

    return adversariesInsideRadiusList;
}

bool Utils::_isInsideArea(const Position &pos, float factor, const Position &goalLeftPost, const Position &goalRightDeslocatedPost) {
    // rectangle
    double min_x = std::min(goalLeftPost.x() * factor, goalRightDeslocatedPost.x() * factor);
    double max_x = std::max(goalLeftPost.x() * factor, goalRightDeslocatedPost.x() * factor);

    double min_y = std::min(goalLeftPost.y() * factor, goalRightDeslocatedPost.y() * factor);
    double max_y = std::max(goalLeftPost.y() * factor, goalRightDeslocatedPost.y() * factor);

    bool x = (pos.x() >= min_x && pos.x() <= max_x);
    bool y = (pos.y() >= min_y && pos.y() <= max_y);

    return x && y;

    /*
    return( (pos.x() <= std::max(goalLeftPost.x() * factor, goalRightDeslocatedPost.x() * factor)) && (pos.x() >= std::min(goalLeftPost.x() * factor, goalRightDeslocatedPost.x() * factor)) &&
                (pos.y() <= std::max(goalLeftPost.y() * factor, goalRightDeslocatedPost.y() * factor)) && (pos.y() >= std::min(goalLeftPost.y() * factor, goalRightDeslocatedPost.y() * factor)) );
    */
}
bool Utils::_isOutsideField(const Position &pos, const float maxX, const float maxY) {
    if(fabs(pos.x()) > maxX)
        return true;
    if(fabs(pos.y()) > maxY)
        return true;
    return false;
}

void Utils::setConstants(Constants *constants){
    _constants = constants;
}

Constants* Utils::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at Utils") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}

void Utils::setWorldMap(WorldMap *worldMap){
    _worldMap = worldMap;
}

WorldMap* Utils::getWorldMap() {
    if(_worldMap == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("WorldMap with nullptr value at Utils") + '\n';
    }
    else {
        return _worldMap;
    }

    return nullptr;
}
