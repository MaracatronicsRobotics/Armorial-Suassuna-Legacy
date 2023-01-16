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


#include "visionobject.h"

#include <src/common/utils/utils.h>

VisionObject::VisionObject() {
    _valid = false;
    _confidence = 0.0f;
}

float VisionObject::getConfidence() {
    return _confidence;
}

bool VisionObject::isObjectValid() {
    return _valid;
}

void VisionObject::updateObject(const float &confidence, const Geometry::Vector2D &pos, const Geometry::Angle &orientation) {
    // Update confidence
    _confidence = confidence;

    // If noise filter is not initialized
    if(!_noiseFilter.isInitialized()) {
        // Init noise
        _noiseFilter.startNoise();

        // Set invalid
        setInvalid();
    }
    else {
        // If object is safe (survived at noise filter)
        if(isObjectSafe()) {
           // Reset loss filter
           _lossFilter.startLoss();

           // Set position and orientation
           setPosition(pos);
           setOrientation(orientation);

           // Set as valid
           setValid();
        }
        // If object is not safe (noise filter running)
        else {
            // Mark as invalid
            setInvalid();

            // Reset loss
            _lossFilter.startLoss();
        }
    }
}

void VisionObject::updateObject() {
    // If loss filter is not initialized
    if(!_lossFilter.isInitialized()) {
        // Just start loss
        _lossFilter.startLoss();
    }
    // If loss filter is already initialized
    else {
        // If object is really lost
        if(isObjectLoss()) {
            // Reset noise and set invalid
            _noiseFilter.startNoise();
            setInvalid();
        }
        // If object is not lost already and is safe
        else if(!isObjectLoss() && isObjectSafe()){
            /// TODO: Update positions, orientations, velocity and confidence based on kalman
        }
    }
}

Armorial::Robot VisionObject::convertToRobotDto(const Common::Enums::Color& teamColor, const quint8& robotId) {
    // Create Armorial::Robot instance
    Armorial::Robot robot;

    // Create Armorial::Color with the given team color
    Armorial::Color* color = new Armorial::Color();
    color->set_isblue(Suassuna::Utils::isColorBlue(teamColor));

    // Fill identifier data
    Armorial::RobotIdentifier* robotIdentifier = new Armorial::RobotIdentifier();
    robotIdentifier->set_robotid(robotId);
    robotIdentifier->set_allocated_robotcolor(color);
    robot.set_allocated_robotidentifier(robotIdentifier);

    // Fill position data
    Armorial::Position* robotPosition = new Armorial::Position();
    robotPosition->set_x(this->getPosition().x());
    robotPosition->set_y(this->getPosition().y());
    robotPosition->set_isinvalid(!isObjectValid());
    robot.set_allocated_robotposition(robotPosition);

    // Fill velocity data
    Armorial::Velocity* robotVelocity = new Armorial::Velocity();
    robotVelocity->set_vx(this->getVelocity().x());
    robotVelocity->set_vy(this->getVelocity().y());
    robotVelocity->set_isinvalid(!isObjectValid());
    robot.set_allocated_robotvelocity(robotVelocity);

    // Fill acceleration data
    Armorial::Acceleration* robotAcceleration = new Armorial::Acceleration();
    robotAcceleration->set_ax(this->getAcceleration().x());
    robotAcceleration->set_ay(this->getAcceleration().y());
    robotAcceleration->set_isinvalid(!isObjectValid());
    robot.set_allocated_robotacceleration(robotAcceleration);

    // Fill orientation data
    Armorial::Angle* robotOrientation = new Armorial::Angle();
    robotOrientation->set_value(this->getOrientation().value());
    robotOrientation->set_isinvalid(!isObjectValid());
    robot.set_allocated_robotorientation(robotOrientation);

    // Fill angular speed data
    Armorial::AngularSpeed* robotAngularSpeed = new Armorial::AngularSpeed();
    robotAngularSpeed->set_vw(this->getAngularSpeed());
    robotAngularSpeed->set_isinvalid(!isObjectValid());
    robot.set_allocated_robotangularspeed(robotAngularSpeed);

    // Return filled robot
    return robot;
}

Armorial::Ball VisionObject::convertToBallDto() {
    // Create Armorial::Ball instance
    Armorial::Ball ball;

    // Fill position data
    Armorial::Position* ballPosition = new Armorial::Position();
    ballPosition->set_x(this->getPosition().x());
    ballPosition->set_y(this->getPosition().y());
    ballPosition->set_isinvalid(!isObjectValid());
    ball.set_allocated_ballposition(ballPosition);

    // Fill velocity data
    Armorial::Velocity* ballVelocity = new Armorial::Velocity();
    ballVelocity->set_vx(this->getVelocity().x());
    ballVelocity->set_vy(this->getVelocity().y());
    ballVelocity->set_isinvalid(!isObjectValid());
    ball.set_allocated_ballvelocity(ballVelocity);

    // Fill acceleration data
    Armorial::Acceleration* ballAcceleration = new Armorial::Acceleration();
    ballAcceleration->set_ax(this->getAcceleration().x());
    ballAcceleration->set_ay(this->getAcceleration().y());
    ballAcceleration->set_isinvalid(!isObjectValid());
    ball.set_allocated_ballacceleration(ballAcceleration);

    // Return filled ball
    return ball;
}

bool VisionObject::isObjectSafe() {
    return (_noiseFilter.checkNoise());
}

bool VisionObject::isObjectLoss() {
    return (_lossFilter.checkLoss());
}

void VisionObject::setValid() {
    _valid = true;
}

void VisionObject::setInvalid() {
    _valid = false;
}
