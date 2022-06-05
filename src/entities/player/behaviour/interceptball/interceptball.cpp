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

#include "interceptball.h"

Behaviour_InterceptBall::Behaviour_InterceptBall() {
    _skill_goTo = nullptr;
    _skill_rotateTo = nullptr;
}

QString Behaviour_InterceptBall::name() {
    return "Behaviour_InterceptBall";
}

void Behaviour_InterceptBall::setPosToLook(Position posToLook) {
    _posToLook = posToLook;
}

void Behaviour_InterceptBall::setSpeedFactor(float speedFactor) {
    _speedFactor = speedFactor;
}

void Behaviour_InterceptBall::setUseKickDevice(bool useKickDevice) {
    _useKickDevice = useKickDevice;
}

void Behaviour_InterceptBall::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();
    _skill_rotateTo = new Skill_RotateTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);
    addSkill(SKILL_ROTATETO, _skill_rotateTo);

    // Set internal parameters
    setPosToLook(Utils::getPositionObject(0.0f, 0.0f, 0.0f, true));
    setUseKickDevice(false);
    setSpeedFactor(1.0f);
}

void Behaviour_InterceptBall::run() {
    // If pos to look is invalid, just look to the ball instead
    if(_posToLook.isinvalid()) {
        setPosToLook(getWorldMap()->getBall().ballposition());
    }

    // Taking player pos
    Position playerPos = player()->getPlayerPos();

    // Calculating projection of the ball
    Position objectivePos; // Position where the player should be

    // Unitary velocity vector
    Velocity ballVelocity = getWorldMap()->getBall().ballvelocity();
    float ballVelocityABS = Utils::velocityABS(ballVelocity);
    Position ballPosition = getWorldMap()->getBall().ballposition();

    if(ballVelocityABS <= 0.1f) {
        objectivePos = ballPosition;
    }
    else {
        // Take unitary ball velocity vector
        const Position unitaryBallVelocity = Utils::getPositionObject(ballVelocity.vx()/ballVelocityABS, ballVelocity.vy()/ballVelocityABS);

        // Take ball velocity vector (pos + unitary velocity vector)
        const Position ballVelocityLine = Utils::getPositionObject(ballPosition.x() + unitaryBallVelocity.x(), ballPosition.y() + unitaryBallVelocity.y());

        // Get projection
        objectivePos = Utils::projectPointAtLine(ballPosition, ballVelocityLine, playerPos);

        // Danger radius: If an enemy robot is inside this radius, the player is being marked
        float dangerRadius = 0.5f;

        QList<Robot> adversariesInRadius = Utils::getAdversariesInRadius(playerPos, dangerRadius);

        if (adversariesInRadius.empty() || player()->getPlayerID() == getConstants()->getKeeperID()){
            // Not being marked, just takes the fastest route to the ballVelocityLine projection
            // Pid hacking
            objectivePos = Utils::threePoints(objectivePos, playerPos, 0.01f, M_PI);
        } else {
            // Get the closest enemy to the ball (that is also inside the danger radius)
            Robot robot = adversariesInRadius.first();
            Position closestEnemyPos = robot.robotposition();
            float smallestDistanceToBall = Utils::distance(closestEnemyPos, ballPosition);
            for (Robot robot : adversariesInRadius) {
                Position robotPos = robot.robotposition();
                float distToBall = Utils::distance(robotPos, ballPosition);

                if (distToBall < smallestDistanceToBall){
                    smallestDistanceToBall = distToBall;
                    closestEnemyPos = robotPos;
                }
            }

            // Now that we have the closest to the ball marker, project its position into the ballVelocityLine and tries to stay a little ahead of this position
            Position ClosestEnemyProjection = Utils::projectPointAtLine(ballPosition, ballVelocityLine, closestEnemyPos);
            objectivePos = Utils::threePoints(ClosestEnemyProjection, playerPos, 0.2f, M_PI);
        }

        // Check if use kick bar
        if(_useKickDevice) {
            Position kickDevicePosition = player()->getPlayerKickerDevicePos();
            Position shiftVector = Utils::getPositionObject(
                playerPos.x() - kickDevicePosition.x(),
                objectivePos.y() - playerPos.y()
            );

            objectivePos = Utils::getPositionObject(
                shiftVector.x() + objectivePos.x(),
                shiftVector.y() + objectivePos.y()
            );
        }

//        if(getGUI() != nullptr) {
//            getGUI()->fieldView()->drawCircularArc(FieldCircularArc("tst", objectivePos, 0.02, 0, Angle::twoPi), QColor(255, 0, 0));
//        }
    }

    // If it is not goalkeeper, try to avoid projections inside goal
    if(player()->getPlayerID() != getConstants()->getKeeperID()) {
        if (Utils::isInsideOurArea(objectivePos, 1.05) || Utils::isInsideTheirArea(objectivePos, 1.05)){
            /// TODO: it will be nice if we repulse the robot outside the goal area...
            // Maintain position
            objectivePos = player()->getPlayerPos();
        }
    }

    // Set parameters and run skills
    _skill_goTo->setTargetPosition(objectivePos);
//    _skill_goTo->setSpeedFactor(_speedFactor);
    setSkill(SKILL_GOTO);

    _skill_rotateTo->setTargetPosition(_posToLook);
    _skill_rotateTo->setReferencePosition(player()->getPlayerPos());
    setSkill(SKILL_ROTATETO);

    // Enable dribbling
    player()->playerDribble(true);
}
