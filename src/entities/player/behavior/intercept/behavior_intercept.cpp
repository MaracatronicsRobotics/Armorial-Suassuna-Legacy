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

#include "behavior_intercept.h"

Behavior_Intercept::Behavior_Intercept() {
    _firstLimitationPoint = Geometry::Vector2D(0.0, 0.0);
    _secondLimitationPoint = Geometry::Vector2D(0.0, 0.0);
    _objectPos = Geometry::Vector2D(0.0, 0.0);
    _objectVel = Geometry::Vector2D(0.0, 0.0);
    _spinEnabled = false;
}

QString Behavior_Intercept::name() {
    return "Behavior_Intercept";
}

void Behavior_Intercept::setInterceptSegment(Geometry::Vector2D firstPoint, Geometry::Vector2D secondPoint) {
    _firstLimitationPoint = firstPoint;
    _secondLimitationPoint = secondPoint;
}

void Behavior_Intercept::configure() {
//    _baseSpeed = getConstants()->playerBaseSpeed();
    _baseSpeed = 45.0f;
//    _desiredLinearError = player()->getLinearError();
    _desiredLinearError = 0.5f;

    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_spin = new Skill_Spin();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_SPIN, _skill_spin);
}

void Behavior_Intercept::run() {
    // Check ball speed (maybe a error)
    if(_objectVel.length() <= 0.01f) {
        _interceptPos = player()->getPosition();
    } else {
        if ((!_firstLimitationPoint.isValid()) || (!_secondLimitationPoint.isValid())) {
            _interceptPos = getOrthogonalProjection();
        } else {
            _interceptPos = getInterceptionPosition();
        }
    }

    // Distances to the intercept position
    float objectDistance = sqrt(powf(_objectPos.x() - _interceptPos.x(),2) + powf(_objectPos.y() - _interceptPos.y(),2));
    float playerDistance = player()->getPosition().dist(_interceptPos);

    Geometry::Vector2D uninatyVelocityVector = Geometry::Vector2D((_interceptPos.x() - player()->getPosition().x()) / playerDistance,
                                   (_interceptPos.y() - player()->getPosition().y()) / playerDistance);
    float playerVelocity = playerDistance * _objectVel.length() / objectDistance;
    _interceptVel = Geometry::Vector2D(playerVelocity * uninatyVelocityVector.x(), playerVelocity * uninatyVelocityVector.y());

    if (_spinEnabled && playerDistance < 0.5f) {
        runSkill(SKILL_SPIN);
    } else {
        _skill_goTo->setTargetPosition(_interceptPos);
        Geometry::Angle orientationToTarget = Geometry::Angle((_interceptPos - player()->getPosition()).angle());
        Geometry::Angle orientationPlayer = player()->getOrientation();
        Geometry::Angle orientationPlayerBack = Geometry::Angle(player()->getOrientation().value() + M_PI);
        float frontAngle = orientationPlayer.shortestAngleDiff(orientationToTarget);
        float backAngle = orientationPlayerBack.shortestAngleDiff(orientationToTarget);

        _skill_goTo->setUseSwappedOrientation(backAngle < frontAngle);
        runSkill(SKILL_GOTO);
    }
}

Geometry::Vector2D Behavior_Intercept::getOrthogonalProjection() {
    // Taking the vector distance from object to player
    Geometry::Vector2D playerVector = Geometry::Vector2D(player()->getPosition().x() - _objectPos.x(), player()->getPosition().y() - _objectPos.y());

    // Taking the numeric proportion from orthogonal projection formula
    float numericProportion = (playerVector.x() * _objectVel.x() + playerVector.y()
                               * _objectVel.y()) / powf(_objectVel.length(), 2);

    // Taking the orthogonal projection position
    float projection_x = _objectPos.x() + numericProportion * _objectVel.x();
    float projection_y = _objectPos.y() + numericProportion * _objectVel.y();

    return Geometry::Vector2D(projection_x, projection_y);
}

Geometry::Vector2D Behavior_Intercept::getInterceptionPosition() {
    // We will create lines/vectors of movement, which means we must take the anlgular coefficiens of them.
    // This may be a problem if the denominator is null. We have to analyse the 4 possible cases in order
    // to predict what to do to interpt the target.
    float trajectory_x = _firstLimitationPoint.x() - _secondLimitationPoint.x();
    float trajectory_y = _firstLimitationPoint.y() - _secondLimitationPoint.y();
    Geometry::Vector2D interceptPosition;

    if (_objectVel.x() == 0.0f && trajectory_x == 0.0f) {
        // Case where both anular coefficents goes to infinite which also means the lines are parallels so
        // the interception is impossible.
        return player()->getPosition();
    }
    else if (_objectVel.x() == 0.0f && trajectory_x != 0.0f) {
        // Case where the object of interception moves on the y direction so the line of movement of the
        // object is 'x = _objectPos.x()'.

        // Taking the angular coefficient
        float trajectoryAngular = trajectory_y / trajectory_x;

        // Taking the linear coefficient
        float trajectoryLinear = _firstLimitationPoint.y() - trajectoryAngular * _firstLimitationPoint.x();

        // Taking the inteception Position
        float intercept_y = trajectoryAngular * _objectPos.x() + trajectoryLinear;
        interceptPosition = Geometry::Vector2D(_objectPos.x(), intercept_y);
    }
    else if (_objectVel.x() != 0.0f && trajectory_x == 0.0f) {
        // Case where our player  moves on the y direction so the line of movement of the object is
        // 'x = _firstLimitationPoint.x()', once our player x position will be the same of this point.

        // Taking the angular coefficient
        float objectAngular = _objectVel.y() / _objectVel.x();

        // Taking the linear coefficient
        float objectLinear = _objectPos.y() - objectAngular * _objectPos.x();

        // Taking the inteception Position
        float intercept_y = objectAngular * _firstLimitationPoint.x() + objectLinear;
        interceptPosition = Geometry::Vector2D(_firstLimitationPoint.x(), intercept_y);
    } else {
        // Taking the angular coefficients
        float objectAngular = _objectVel.y() / _objectVel.x();
        float trajectoryAngular = trajectory_y / trajectory_x;

        // Analysing vector parallelism
        if (trajectoryAngular == objectAngular) {
            return player()->getPosition();
        } else {
            // Taking the linear coefficients
            float objectLinear = _objectPos.y() - objectAngular * _objectPos.x();
            float trajectoryLinear = _firstLimitationPoint.y() - trajectoryAngular * _firstLimitationPoint.x();

            // Taking the inteception Position
            float intercept_x = (objectLinear - trajectoryLinear) / (trajectoryAngular - objectAngular);
            float intercept_y = intercept_x * objectAngular + objectLinear;
            interceptPosition = Geometry::Vector2D(intercept_x, intercept_y);
        }
    }

    float segmentDistance = sqrt(powf(_firstLimitationPoint.x() - _secondLimitationPoint.x(),2)
                                 + powf(_firstLimitationPoint.y() - _secondLimitationPoint.y(),2));
    float firstDistance = sqrt(powf(_firstLimitationPoint.x() - interceptPosition.x(),2)
                               + powf(_firstLimitationPoint.y() - interceptPosition.y(),2));
    float secondDistance = sqrt(powf(_secondLimitationPoint.x() - interceptPosition.x(),2)
                                + powf(_secondLimitationPoint.y() - interceptPosition.y(),2));

    // Checking the limitation
    if (firstDistance + secondDistance > segmentDistance) {
        if (firstDistance < secondDistance) {
            return _firstLimitationPoint;
        } else {
            return _secondLimitationPoint;
        }
    } else {
        return interceptPosition;
    }
}
