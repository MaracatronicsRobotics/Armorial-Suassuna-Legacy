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

#include "behaviour_attacker.h"

#include <utils/freeangles/freeangles.h>
#include <utils/line/line.hh>

#define RECEIVER_INVALID_ID 200

QString Behaviour_Attacker::name() {
    return "Behaviour_Attacker";
}

Behaviour_Attacker::Behaviour_Attacker() {
    _sk_goToLookTo = NULL;
    _sk_push       = NULL;

    receiverDecisionTimer.start();
    aimDecisionTimer.start();
    shootPassDecisionTimer.start();
}

void Behaviour_Attacker::configure() {
    // Skills
    usesSkill(_sk_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_sk_push       = new Skill_PushBall());
    usesSkill(_sk_kick       = new Skill_Kick());

    // Transitions
    addTransition(SKT_POS,  _sk_push, _sk_goToLookTo);
    addTransition(SKT_POS,  _sk_kick, _sk_goToLookTo);

    addTransition(SKT_PUSH, _sk_goToLookTo, _sk_push);
    addTransition(SKT_PUSH, _sk_kick, _sk_push);

    addTransition(SKT_KICK, _sk_goToLookTo, _sk_kick);
    addTransition(SKT_KICK, _sk_push, _sk_kick);

    // Initial Skill
    setInitialSkill(_sk_goToLookTo);

    // Initial state
    _state = STATE_CANTKICK;
    firstAim   = false;
    canShoot   = true;

    _rcvScore = 0.0;
    _bestRcv  = RECEIVER_INVALID_ID;
}

void Behaviour_Attacker::run() {
    if(!canTakeBall())
        _state = STATE_CANTKICK;

    // Guarantee that player will reset if he is the best receiver
    if(_bestRcv == player()->playerId()) _bestRcv = RECEIVER_INVALID_ID;

    switch(_state){
    case STATE_CANTKICK:{
        // Check if can switch state
        if(canTakeBall()){
            _state = STATE_PUSH;
            break;
        }

        // Transition to goToLookTo
        enableTransition(SKT_POS);
        Position lookPosition, desiredPosition;
        if(loc()->isOutsideField(loc()->ball())){
            desiredPosition = player()->position();
            lookPosition = loc()->ball();
        }
        else{
            if(ref()->getGameInfo(player()->team()->teamColor())->theirDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->theirIndirectKick()){
                desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 0.8f, 0.0f);
                lookPosition = loc()->ball();
            }
            else{
                lookPosition = loc()->ball();
                if(loc()->isInsideTheirArea(loc()->ball())) desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->theirGoal(), 1.7f, GEARSystem::Angle::pi);
                else if(loc()->isInsideOurArea(loc()->ball())){
                    desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 1.7f, GEARSystem::Angle::pi);
                    lookPosition = loc()->ourGoal();
                }
                else desiredPosition = WR::Utils::threePoints(loc()->ball(), loc()->ourGoal(), 0.6f, 0.0);
            }
        }

        // Adjusting goToLookTo
        _sk_goToLookTo->setAvoidBall(true);
        _sk_goToLookTo->setDesiredPosition(desiredPosition);
        _sk_goToLookTo->setAimPosition(lookPosition);
    }
    break;
    case STATE_PUSH:{
        if(ref()->getGameInfo(player()->team()->teamColor())->ourIndirectKick()){
            quint8 bestReceiver = getBestReceiver().second;
            if(bestReceiver != RECEIVER_INVALID_ID){
                // Aim to our best receiver
                Position ourReceiverPosition = PlayerBus::ourPlayer(bestReceiver)->position();
                if(WR::Utils::distance(ourReceiverPosition, PlayerBus::ourPlayer(bestReceiver)->nextPosition()) > 0.7f){
                    _sk_goToLookTo->setDesiredPosition(WR::Utils::threePoints(loc()->ball(), PlayerBus::ourPlayer(bestReceiver)->nextPosition(), 0.3f, GEARSystem::Angle::pi));
                    _sk_goToLookTo->setAimPosition(loc()->ball());
                    _sk_goToLookTo->setAvoidBall(true);
                    enableTransition(SKT_POS);
                }else{
                    _sk_kick->setAim(ourReceiverPosition);

                    // Check if the path is obstructed
                    QList<quint8> shootList = {player()->playerId(), bestReceiver};
                    bool isObstructed = loc()->isVectorObstructed(player()->position(), ourReceiverPosition, shootList, getConstants()->getRobotRadius() * 3.0, false);

                    // Adjust kick power based on obstructed path or distance to receiver
                    if(isObstructed) _sk_kick->setPower(std::min(getConstants()->getMaxKickPower(), 0.75f * sqrt((player()->distanceTo(ourReceiverPosition) * 9.8f) / sin(2 * GEARSystem::Angle::toRadians(65.0)))));
                    else             _sk_kick->setPower(std::min(getConstants()->getMaxKickPower(), std::max(getConstants()->getMaxKickPower()/2.0f, 2.0f * player()->distanceTo(ourReceiverPosition))));

                    // Set if is parabolic
                    _sk_kick->setIsChip(isObstructed);
                    enableTransition(SKT_KICK);
                }
            }
            else{
                // If we don't have any receivers, shoot to their goalie
                quint8 theirGoalie = getTheirClosestPlayerToGoal();
                if(theirGoalie == RECEIVER_INVALID_ID){
                    // they don't have any players (?) shoot to their goal anyway
                    _sk_kick->setAim(loc()->theirGoal());
                    _sk_kick->setIsChip(false);
                    _sk_kick->setPower(getConstants()->getMaxKickPower());
                }
                else{
                    // shoot to their goalie
                    _sk_kick->setAim(PlayerBus::theirPlayer(theirGoalie)->position());
                    _sk_kick->setIsChip(false);
                    _sk_kick->setPower(getConstants()->getMaxKickPower());
                }
                enableTransition(SKT_KICK);
            }
        }
        else{
            // This is the game_on / our kickoff / our direct kick situation
            // For kickoff and direct kick always use KICK skill
            // Otherwise, use pushBall skill

            // First get the best position to kick and the angle interval
            aimDecisionTimer.stop();
            if(aimDecisionTimer.timesec() >= AIM_DECISION_TIME || !firstAim){
                if(!firstAim) firstAim = true;
                _aim = getBestAimPosition();
                aimDecisionTimer.start();
            }

            // If don't have any free angles, the path is obstructed or have an unsufficient opening for kick
            // Here you put the pass / shoot decision heuristic =)

            // Taking best receiver and updating their score / id
            getBestReceiver();

            shootPassDecisionTimer.stop();
            if(shootPassDecisionTimer.timesec() >= (SHOOT_PASS_DECISION_TIME * (ref()->getGameInfo(player()->team()->teamColor())->freeKick() ? 4.0 : 1.0))){
                _mlpResult = MLP_result();

                // Calculando a chance do jogador chutar ao gol (supondo posse de bola)
                double _shootingChance = getFutureKickChance(player()->playerId());
                double _passingChance  = _rcvScore;

                // Adaptando a shooting chance e a passing chance com o peso da mlp
                _shootingChance = ((2.0/3.0)*_shootingChance + _mlpResult *(1.0/3.0));
                _passingChance  = ((2.0/3.0)* _passingChance) + ((1.0 - _mlpResult)*(1.0/3.0));

                /*
                if(player()->hasBallPossession() && loc()->ballVelocity().abs() <= 1.0){
                    cout << "A receiver list tem:\n";
                    for(quint8 i=0; i< _receiversList.size(); i++){
                        cout << (int)_receiversList.at(i) << " ";
                    }
                    cout << "\nO best ID deu: " << (int)_bestRcv << "\n";
                    cout << "\t shootin chance: " << _shootingChance << "\n";
                    cout << "\t passin chance: " << _passingChance << "\n";
                    cout << "\t mlp result: " << _mlpResult << "\n";
                    printf("\t SHOOTINCHANCE DO PF: %lf\n", _shootingChance);
                }
                */

                canShoot = !(_passingChance > _shootingChance);

                shootPassDecisionTimer.start();
            }

            if(!canShoot){
                // Get the best receiver to make a pass
                if(_bestRcv != RECEIVER_INVALID_ID){
                    // Aim to our best receiver
                    Position ourReceiverKickDevice = WR::Utils::getPlayerKickDevice(_bestRcv);

                    // Check if the path is obstructed
                    QList<quint8> shootList = {player()->playerId(), _bestRcv};
                    bool isObstructed = loc()->isVectorObstructed(player()->position(), ourReceiverKickDevice, shootList, getConstants()->getRobotRadius() * 3.0, false);
                    float power;
                    // Adjust kick power based on obstructed path or distance to receiver
                    if(isObstructed) power = std::min(getConstants()->getMaxKickPower(), 0.75f * sqrt((player()->distanceTo(ourReceiverKickDevice) * 9.8f) / sin(2 * GEARSystem::Angle::toRadians(65.0))));
                    else             power = std::min(getConstants()->getMaxKickPower(), std::max(getConstants()->getMaxKickPower()/2.0f, 2.0f * player()->distanceTo(ourReceiverKickDevice)));

                    // Set if is parabolic and make it shoot when sufficiently aligned to the receiver
                    if(ref()->getGameInfo(player()->team()->teamColor())->ourDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->ourKickoff()){
                        _sk_kick->setAim(ourReceiverKickDevice);
                        _sk_kick->setIsChip(isObstructed);
                        _sk_kick->setPower(power);

                        enableTransition(SKT_KICK);
                    }else{
                        _sk_push->setIsParabolic(isObstructed);
                        Position rcvPos = WR::Utils::getPlayerKickDevice(_bestRcv);
                        _sk_push->setAim(rcvPos);
                        if(WR::Utils::distance(PlayerBus::ourPlayer(_bestRcv)->position(), PlayerBus::ourPlayer(_bestRcv)->nextPosition()) <= 0.7f){
                            _sk_push->setDestination(Position(false, 0.0, 0.0, 0.0));
                            _sk_push->setKickPower(power);
                            _sk_push->shootWhenAligned(true);
                        }
                        else{
                            _sk_push->setDestination(WR::Utils::threePoints(loc()->ball(), PlayerBus::ourPlayer(_bestRcv)->nextPosition(), 0.5f, 0.0));
                            if(_sk_push->getPushedDistance() >= 0.8f * _sk_push->getMaxPushDistance()){
                                _sk_push->setDestination(Position(false, 0.0, 0.0, 0.0));
                                _sk_push->setKickPower(power);
                                _sk_push->shootWhenAligned(true);
                            }
                            else{
                                _sk_push->shootWhenAligned(false);
                            }
                        }

                        enableTransition(SKT_PUSH);
                    }
                }
                else{
                    // If we don't have any receivers available, shot to their goal even if in bad conditions

                    // Check if the position to aim isn't unknown
                    Position aimPos;
                    if(!_aim.second.isUnknown()) aimPos = (_aim.second);
                    else                         aimPos = (loc()->theirGoal());

                    if(ref()->getGameInfo(player()->team()->teamColor())->ourDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->ourKickoff()){
                        _sk_kick->setAim(aimPos);
                        _sk_kick->setIsChip(false);
                        _sk_kick->setPower(getConstants()->getMaxKickPower());

                        enableTransition(SKT_KICK);
                    }
                    else{
                        _sk_push->setDestination(Position(false, 0.0, 0.0, 0.0));
                        _sk_push->setAim(aimPos);
                        _sk_push->setIsParabolic(false);
                        _sk_push->setKickPower(getConstants()->getMaxKickPower());
                        _sk_push->shootWhenAligned(true);

                        enableTransition(SKT_PUSH);
                    }
                }
            }else{
                // Debug to UI
                CoachView::drawTriangle(player()->position(), loc()->theirGoalLeftPost(), loc()->theirGoalRightPost(), RGBA(178, 34, 34, 0.4, MRCConstants::robotZ + 0.01));
                CoachView::drawLine(player()->position(), _aim.second.isUnknown() ? loc()->theirGoal() : _aim.second, RGBA(106, 90, 205, 1.0, MRCConstants::robotZ + 0.02));

                Position aim = (_aim.second.isUnknown()) ? loc()->theirGoal() : _aim.second;

                // If we have an sufficient opening to their goal, make an direct kick to it
                if(ref()->getGameInfo(player()->team()->teamColor())->ourDirectKick() || ref()->getGameInfo(player()->team()->teamColor())->ourKickoff()){
                    _sk_kick->setAim(aim);
                    _sk_kick->setIsChip(false);
                    _sk_kick->setPower(getConstants()->getMaxKickPower());

                    enableTransition(SKT_KICK);
                }
                else{
                    _sk_push->setDestination(Position(false, 0.0, 0.0, 0.0));
                    _sk_push->setAim(aim);
                    _sk_push->setIsParabolic(false);
                    _sk_push->setKickPower(getConstants()->getMaxKickPower());
                    _sk_push->shootWhenAligned(true);

                    enableTransition(SKT_PUSH);
                }
            }
        }
    }
    break;
    }
}

bool Behaviour_Attacker::canTakeBall(){
    return !(!player()->canKickBall() || loc()->isInsideOurArea(loc()->ball()) || loc()->isInsideTheirArea(loc()->ball()) || loc()->isOutsideField(loc()->ball()));
}

std::pair<float, quint8> Behaviour_Attacker::getBestReceiver(){
    receiverDecisionTimer.stop();

    float receiverTime = RECEIVER_DECISION_TIME;

    // If free kick (direct, indirect) or kickoff, needs to wait more to change the choose.
    if(ref()->getGameInfo(player()->team()->teamColor())->freeKick() || ref()->getGameInfo(player()->team()->teamColor())->kickoff())
        receiverTime = RECEIVER_DECISION_TIME_AT_FOUL;

    if(receiverDecisionTimer.timesec() >= receiverTime || _bestRcv == RECEIVER_INVALID_ID){
        receiversListMutex.lock();
        /*
        quint8 bestId = RECEIVER_INVALID_ID;
        QList<quint8> list = _receiversList;
        float largestRecAngle;
        for(int x = 0; x < list.size(); x++){
            if(list.at(x) == player()->playerId()) continue;
            if(PlayerBus::ourPlayerAvailable(list.at(x))){
                QList<Obstacle> recObstacles = FreeAngles::getObstacles(loc()->theirGoal(), 4);
                for(int i=0; i<recObstacles.size(); i++) {
                    Obstacle obst = recObstacles.at(i);
                    if(obst.team()==player()->teamId() && obst.id()==list.at(x)){
                        recObstacles.removeAt(i--);
                    }
                }
                Position post1 = loc()->theirGoalLeftPost();
                Position post2 = loc()->theirGoalRightPost();
                float angleToPost1 = WR::Utils::getAngle(PlayerBus::ourPlayer(list.at(x))->position(), post1);
                float angleToPost2 = WR::Utils::getAngle(PlayerBus::ourPlayer(list.at(x))->position(), post2);

                if(angleToPost1 > angleToPost2) std::swap(post1, post2);

                QList<FreeAngles::Interval> recFreeAngles = FreeAngles::getFreeAngles(PlayerBus::ourPlayer(list.at(x))->position(), post1, post2, recObstacles);
                float largestAngle = -1;
                for(int i=0; i<recFreeAngles.size(); i++) {
                    float angI = recFreeAngles.at(i).angInitial();
                    float angF = recFreeAngles.at(i).angFinal();
                    WR::Utils::angleLimitZeroTwoPi(&angI);
                    WR::Utils::angleLimitZeroTwoPi(&angF);
                    float dif = angF - angI;
                    WR::Utils::angleLimitZeroTwoPi(&dif);

                    if(dif>largestAngle) {
                        largestAngle = dif;
                    }
                }
                if(largestAngle > largestRecAngle){
                    largestRecAngle = largestAngle;
                    bestId = list.at(x);
                }
            }
        }
        */

        double _actualPassingChance,_bestPassingChance = 0.0;
        quint8 _bestId;
        for(quint8 i = 0;i < _receiversList.size();i++){
            if(player()->playerId() == _receiversList.at(i)) continue;
            if(PlayerBus::ourPlayerAvailable(_receiversList.at(i))){
                _actualPassingChance = getFinalPassingChance(_receiversList.at(i));
                if(_actualPassingChance > _bestPassingChance){
                    _bestPassingChance = _actualPassingChance;
                    _bestId = _receiversList.at(i);
                }
            }
        }

        receiverDecisionTimer.start();
        _bestRcv = _bestId;
        _rcvScore = _bestPassingChance;

        receiversListMutex.unlock();
    }

    return std::make_pair(_rcvScore, _bestRcv);
}

quint8 Behaviour_Attacker::getTheirClosestPlayerToGoal(){
    QList<Player*> opPlayers = loc()->getOpPlayers().values();
    QList<Player*>::iterator it;

    float dist = 999.0f;
    quint8 id = RECEIVER_INVALID_ID;

    for(it = opPlayers.begin(); it != opPlayers.end(); it++){
        float distTheirGoal = (*it)->distanceTo(loc()->theirGoal());
        if(distTheirGoal < dist){
            dist = distTheirGoal;
            id = (*it)->playerId();
        }
    }

    return id;
}

std::pair<float, Position> Behaviour_Attacker::getBestAimPosition(){
    // Margin to avoid select the post as aim (or outside it)
    float postMargin = 0.05*loc()->fieldDefenseWidth()/2;

    // Adjust margin
    if(loc()->theirSide().isRight()) {
        postMargin = -postMargin;
    }

    // shift the position of the post to the center of the goal
    Position theirGoalRightPost = loc()->theirGoalRightPost();
    theirGoalRightPost.setPosition(theirGoalRightPost.x(),
                                   theirGoalRightPost.y() + postMargin,
                                   theirGoalRightPost.z());
    Position theirGoalLeftPost = loc()->theirGoalLeftPost();
    theirGoalLeftPost.setPosition(theirGoalLeftPost.x(),
                                  theirGoalLeftPost.y() - postMargin,
                                  theirGoalLeftPost.z());
    Position posTheirGoal = loc()->theirGoal();

    // get obstacles
    QList<Obstacle> obstacles = FreeAngles::getObstacles(loc()->ball());

    // Shift the obstacles
    QList<Obstacle>::iterator obst;

    // removing attacker from obstacles (avoid problems when he is rotating)
    for(int x = 0; x < obstacles.size(); x++){
        if(obstacles[x].id() == player()->playerId() && obstacles[x].team() == player()->teamId()){
            obstacles.removeAt(x);
            break;
        }
    }

    for(obst = obstacles.begin(); obst != obstacles.end(); obst++) {
        obst->radius() = 1.2 * getConstants()->getRobotRadius();
        // access the robot=
        PlayerAccess *robot = NULL;

        if(player()->opTeamId() == obst->team()) {
            robot = PlayerBus::theirPlayer(obst->id());
        } else {
            robot = PlayerBus::ourPlayer(obst->id());
        }

        Velocity robotVel;
        if(robot != NULL) {
            robotVel = robot->velocity();
        }

        if(robotVel.abs() > 2*0.015) {
            float time = WR::Utils::distance(loc()->ball(), obst->position())/8.0f;
            if(robotVel.abs()*time > 0.2f) {
                time = 0.2f/robotVel.abs();
            }
            obst->position() = WR::Utils::vectorSum(obst->position(), robotVel, time);
        }
    }

    // get free angle with the shifted obstacles
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(loc()->ball(), theirGoalRightPost, theirGoalLeftPost, obstacles);

    // Get the largest

    float largestAngle=0, largestMid=0;

    if(freeAngles.size()==0) { // Without free angles
        return std::make_pair(0.0, Position(false, 0.0, 0.0, 0.0));
    } else {
        for(int i=0; i<freeAngles.size(); i++) {
            float angI = freeAngles.at(i).angInitial();
            float angF = freeAngles.at(i).angFinal();
            WR::Utils::angleLimitZeroTwoPi(&angI);
            WR::Utils::angleLimitZeroTwoPi(&angF);
            float dif = angF - angI;
            WR::Utils::angleLimitZeroTwoPi(&dif);

            if(dif>largestAngle) {
                largestAngle = dif;
                largestMid = angF - dif/2;
            }
        }
    }

    // With free angles

    // Triangle
    float x = posTheirGoal.x() - loc()->ball().x();
    float tg = tan(largestMid);
    float y = tg*x;

    // Impact point
    float pos_y = loc()->ball().y() + y;
    Position impactPos(true, posTheirGoal.x(), pos_y, 0.0);

    // Check if impact pos has enough space for the ball
    bool obstructedWay = loc()->isVectorObstructed(loc()->ball(), impactPos, player()->playerId(), getConstants()->getBallRadius()*1.5, false);

    if(obstructedWay) {
        return std::make_pair(0.0, Position(false, 0.0, 0.0, 0.0));
    }

    return std::make_pair(largestAngle, impactPos);
}

void Behaviour_Attacker::clearReceivers(){
    receiversListMutex.lock();
    _receiversList.clear();
    receiversListMutex.unlock();
}

void Behaviour_Attacker::addReceiver(quint8 id){
    receiversListMutex.lock();
    _receiversList.push_back(id);
    receiversListMutex.unlock();
}

/////// SCORE / PASS HEURISTIC FUNCTIONS
std::pair<float, Position> Behaviour_Attacker::getBestAssumedPosition(quint8 _id){
    // Margin to avoid select the post as aim (or outside it)
    float postMargin = 0.05*loc()->fieldDefenseWidth()/2;

    // Adjust margin
    if(loc()->theirSide().isRight()) {
        postMargin = -postMargin;
    }

    // shift the position of the post to the center of the goal
    Position theirGoalRightPost = loc()->theirGoalRightPost();
    theirGoalRightPost.setPosition(theirGoalRightPost.x(),
                                   theirGoalRightPost.y() + postMargin,
                                   theirGoalRightPost.z());
    Position theirGoalLeftPost = loc()->theirGoalLeftPost();
    theirGoalLeftPost.setPosition(theirGoalLeftPost.x(),
                                  theirGoalLeftPost.y() - postMargin,
                                  theirGoalLeftPost.z());
    Position posTheirGoal = loc()->theirGoal();

    // get obstacles
    QList<Obstacle> obstacles = FreeAngles::getObstacles(loc()->ball());

    // Shift the obstacles
    QList<Obstacle>::iterator obst;

    // removing attacker from obstacles (avoid problems when he is rotating)
    for(int x = 0; x < obstacles.size(); x++){
        if(obstacles[x].id() == _id && obstacles[x].team() == player()->teamId()){
            obstacles.removeAt(x);
            break;
        }
    }

    for(obst = obstacles.begin(); obst != obstacles.end(); obst++) {
        obst->radius() = 1.2 * getConstants()->getRobotRadius();
        // access the robot=
        PlayerAccess *robot = NULL;

        if(player()->opTeamId() == obst->team()) {
            robot = PlayerBus::theirPlayer(obst->id());
        } else {
            robot = PlayerBus::ourPlayer(obst->id());
        }

        Velocity robotVel;
        if(robot != NULL) {
            robotVel = robot->velocity();
        }

        if(robotVel.abs() > 2*0.015) {
            float time = WR::Utils::distance(PlayerBus::ourPlayer(_id)->position(), obst->position())/8.0f;
            if(robotVel.abs()*time > 0.2f) {
                time = 0.2f/robotVel.abs();
            }
            obst->position() = WR::Utils::vectorSum(obst->position(), robotVel, time);
        }
    }

    // get free angle with the shifted obstacles
    QList<FreeAngles::Interval> freeAngles = FreeAngles::getFreeAngles(PlayerBus::ourPlayer(_id)->position(), theirGoalRightPost, theirGoalLeftPost, obstacles);

    // Get the largest

    float largestAngle=0, largestMid=0;

    if(freeAngles.size()==0) { // Without free angles
        return std::make_pair(0.0, Position(false, 0.0, 0.0, 0.0));
    } else {
        for(int i=0; i<freeAngles.size(); i++) {
            float angI = freeAngles.at(i).angInitial();
            float angF = freeAngles.at(i).angFinal();
            WR::Utils::angleLimitZeroTwoPi(&angI);
            WR::Utils::angleLimitZeroTwoPi(&angF);
            float dif = angF - angI;
            WR::Utils::angleLimitZeroTwoPi(&dif);

            if(dif>largestAngle) {
                largestAngle = dif;
                largestMid = angF - dif/2;
            }
        }
    }

    // With free angles

    // Triangle
    float x = posTheirGoal.x() - PlayerBus::ourPlayer(_id)->position().x();
    float tg = tan(largestMid);
    float y = tg*x;

    // Impact point
    float pos_y = PlayerBus::ourPlayer(_id)->position().y() + y;
    Position impactPos(true, posTheirGoal.x(), pos_y, 0.0);

    // Check if impact pos has enough space for the ball
    bool obstructedWay = loc()->isVectorObstructed(PlayerBus::ourPlayer(_id)->position(), impactPos, _id, getConstants()->getBallRadius()*1.5, false);

    if(obstructedWay) {
        return std::make_pair(0.0, Position(false, 0.0, 0.0, 0.0));
    }

    return std::make_pair(largestAngle, impactPos);
}

double Behaviour_Attacker::getFutureKickChance(quint8 _id){

    std::pair<float, Position> bestAim = getBestAssumedPosition(_id);

    //Must know 100% free goal to make comparison

    /* //forma antiga:
     *
    _firstSide = WR::Utils::distance(loc()->ball(), loc()->theirGoalRightPost());
    _secondSide = WR::Utils::distance(loc()->ball(), loc()->theirGoalLeftPost());
    _oppositeSide = abs((loc()->theirGoalLeftPost().y() - loc()->theirGoalRightPost().y()));
    double a= _firstSide, b = _secondSide, c = _oppositeSide;
    _cosAngle = (pow(a,2) + pow(b,2) - pow(c,2))/(2*a*b);
    _angle = acos(_cosAngle);
    */

    // Margin to avoid select the post as aim (or outside it)
    float postMargin = 0.05*loc()->fieldDefenseWidth()/2;

    // Adjust margin
    if(loc()->theirSide().isRight()) {
        postMargin = -postMargin;
    }

    // shift the position of the post to the center of the goal

    Position theirGoalRightPost = loc()->theirGoalRightPost();
    theirGoalRightPost.setPosition(theirGoalRightPost.x(),
                                   theirGoalRightPost.y() + postMargin,
                                   theirGoalRightPost.z());

    Position theirGoalLeftPost = loc()->theirGoalLeftPost();
    theirGoalLeftPost.setPosition(theirGoalLeftPost.x(),
                                  theirGoalLeftPost.y() - postMargin,
                                  theirGoalLeftPost.z());
    Position posTheirGoal = loc()->theirGoal();

    float minPosAngle = WR::Utils::getAngle(PlayerBus::ourPlayer(_id)->position(), theirGoalRightPost);
    float maxPosAngle = WR::Utils::getAngle(PlayerBus::ourPlayer(_id)->position(), theirGoalLeftPost);
    _angle = abs(maxPosAngle - minPosAngle);
    WR::Utils::angleLimitZeroTwoPi(&_angle);

    _distScore = 0;

    if(abs(WR::Utils::distance(loc()->theirGoal(),PlayerBus::ourPlayer(_id)->position()))< 4.5){
        _distScore = ((pow ( ( WR::Utils::distance(PlayerBus::ourPlayer(_id)->position(),loc()->theirGoal()) - 4.5 ),2)/9) + abs(( WR::Utils::distance(PlayerBus::ourPlayer(_id)->position(),loc()->theirGoal()) - 4.5 )/3))/2;
    }
    if(abs(WR::Utils::distance(loc()->theirGoal(),PlayerBus::ourPlayer(_id)->position()))< 1.5){
        _distScore = 1.0;
    }

    _angleScore =  bestAim.first / _angle;

    if(_angleScore > 1.0){
        //std::cout << "ESTRANHO : " << _angleScore << "\n";
        _angleScore = 1.0;
    }

    if(bestAim.first <= GEARSystem::Angle::toRadians(10.0f)){
        _angleScore = 0;
    }

    double _shootingChance = (_distScore + _angleScore)/2.0;

    if(_angleScore == 0) return 0;

    return _shootingChance;
}


double Behaviour_Attacker::getsizebar(Point2d recpos, Point2d directbarra){
    return 0.5;
}

std::pair<Point2d, Point2d> Behaviour_Attacker::calc_freeangles_points(quint8 bestReceiver){

    Point2d directionkick, recpos, directbarra, p1,p2, playpos;
    playpos.setCoords(player()->position().x(),player()->position().y());


    //lembrar se n tiver availble
    //pegando a direção do chute estimada
    directionkick.setCoords(PlayerBus().ourPlayer(bestReceiver)->position().x() - player()->position().x(), PlayerBus().ourPlayer(bestReceiver)->position().y() - player()->position().y());
    recpos.setCoords(PlayerBus().ourPlayer(bestReceiver)->position().x(),PlayerBus().ourPlayer(bestReceiver)->position().y());

    //normalizando
    directionkick.setCoords(directionkick.x()/directionkick.norm(),directionkick.y()/directionkick.norm());


    // rotacionando 90º
    double angulo = acos(directionkick.x());
    double pi = acos(-1);
    if(directionkick.y() < 0)angulo = (2*(acos(-1))) - angulo;
    angulo += pi/2;
    while(angulo > 2*pi) angulo -= 2*pi;

    directbarra.setCoords(cos(angulo),sin(angulo));
    p1.setCoords(recpos.x() + (directbarra.x() * getsizebar(recpos, directbarra)), recpos.y() + (directbarra.y() * getsizebar(recpos, directbarra)));
    directbarra.setCoords(-directbarra.x(), -directbarra.y());
    p2.setCoords(recpos.x() + (directbarra.x() * getsizebar(recpos, directbarra)), recpos.y() + (directbarra.y() * getsizebar(recpos, directbarra)));

    double ang1,ang2;
    Point2d vetor1(p1.x()-playpos.x(),p1.y()-playpos.y()), vetor2(p2.x()-playpos.x(),p2.y()-playpos.y());
    ang1 = acos(vetor1.x());

    if(vetor1.y() < 0)ang1 = (2*pi) - ang1;
    ang2 = acos(vetor2.x());
    if(vetor2.y() < 0)ang2 = (2*pi) - ang2;
    if(ang1 > ang2){
        //cout << "IHUUUUU\n";
    }
    else{
        //cout << "IHAAAAA\n";
        swap(p1,p2);
    }
    return make_pair(p1,p2);
}



float Behaviour_Attacker::getPlayerPassingChance(quint8 _id){
    if(PlayerBus::ourPlayerAvailable(_id)){
        _recDist = player()->distanceTo(PlayerBus::ourPlayer(_id)->position());
    }else{
        return -1.0;
    }

    if(_recDist <= 1.5){
        _receiverDistScore = 1.0;
    }else if(_recDist <= 4.5){
        _receiverDistScore = (pow((_recDist - 4.5),2.0)/9.0);
    }else{
        _receiverDistScore = 0.0;
    }

    PFA = calc_freeangles_points(_id);
    Position iniFA, fimFA;
    iniFA.setPosition(PFA.first.x(),PFA.first.y(),0);
    fimFA.setPosition(PFA.second.x(),PFA.second.y(),0);

    _receiverAngleUp = Position(true,PFA.first.x(), PFA.first.y(), 0.0);
    _receiverAngleDown = Position(true,PFA.second.x(), PFA.second.y(), 0.0);
    float radius = _recDist;
    // Generates obstacle list, removing the calling player
        QList<Obstacle> obstacles = FreeAngles::getObstacles(player()->position(), radius);
        for(int i=0; i<obstacles.size(); i++) {
            Obstacle obst = obstacles.at(i);
            if(obst.team()==player()->teamId() && (obst.id()==player()->playerId() || obst.id() == _id))
                obstacles.removeAt(i);
        }

        float angInit = WR::Utils::getAngle(player()->position(), iniFA);
        float angEnd  = WR::Utils::getAngle(player()->position(), fimFA);
        float angleDiff = WR::Utils::angleDiff(angInit, angEnd);
        if(angInit > angEnd) std::swap(iniFA, fimFA);

        // Calc free angles
        QList<FreeAngles::Interval> goalInterval = FreeAngles::getFreeAngles(player()->position(), iniFA, fimFA, obstacles);

    float goalLargestAngle = 0.0f;
        float goalLargestMid = 0.0f;
        if(goalInterval.empty()){
            //std::cout << "SEM ANGULO DISPONIVEL\n";
        }
        else{
            for(int x = 0; x < goalInterval.size(); x++){
                float angI = goalInterval.at(x).angInitial();
                float angF = goalInterval.at(x).angFinal();

                WR::Utils::angleLimitZeroTwoPi(&angI);
                WR::Utils::angleLimitZeroTwoPi(&angF);

                float dif  = angF - angI;

                WR::Utils::angleLimitZeroTwoPi(&dif);

                if(dif > goalLargestAngle){
                    goalLargestAngle = dif;
                    goalLargestMid = (angF + angI) / 2.0f;
                }
            }
        }

    _angleTotal = angleDiff;
    _angleDisp = goalLargestAngle;
    _receiverAngleScore = _angleDisp/_angleTotal;
    if(0 && player()->hasBallPossession()){
        std::cout << "Para o id " << int(_id) << ":\n";
        std::cout << "\t DistScore: " << _receiverDistScore << "\n";
        std::cout << "\t AngleScore: " << _receiverAngleScore << " com angulos: " << _angleDisp << " " << _angleTotal << "\n";
        std::cout << "\t PassingChance total: " << (_receiverDistScore + _receiverAngleScore)/2.0 << "\n";
    }

    return (_receiverDistScore+_receiverAngleScore)/2.0;
}

float Behaviour_Attacker::getFinalPassingChance(quint8 _id){
    float _playerToReceiver = getPlayerPassingChance(_id);
    float _receivertoGoal = getFutureKickChance(_id);
    return (_playerToReceiver + _receivertoGoal)/2.0;
}

float Behaviour_Attacker::MLP_result(){

    playerInfo kicker, allyGoalie, oppGoalie, ally0, ally1, opp0, opp1;

    //ENCONTRAR KICKER (vamos precisar do id e das coordenadas)
    //CORRIGIR COORDENADAS CONSIDERANDO QUE O TIME ADVERSARIO EH O DA DIREITA E O NOSSO O DA ESQUERDA:
    //cout << "STEP 1\n";
    kicker.id = player()->playerId();
    kicker.position = player()->position();
    kicker.distToKicker = 0;
    kicker.valid = true;
    //cout << "STEP 2\n";
    if(player()->team()->fieldSide().isLeft()){
        kicker = calc(kicker);
    }
    //cout << "STEP 3\n";
    if(kicker.valid){
        //cout << "STEP 3.1\n";
        //ENCONTRAR OPPGOALIE
        oppGoalie = OppGoalie(kicker);

        //cout << "STEP 3.2\n";
        //ENCONTRAR ALLYGOALIE
        allyGoalie = AllyGoalie(kicker);

        //cout << "STEP 3.3\n";
        //ENCONTRAR OPP0 E OPP1
        playerInfo opp[2];
        Opp(kicker, oppGoalie, opp);
        //cout << "Fazendo 5\n";
        opp0 = opp[0];
        opp1 = opp[1];
        //cout << "Fazendo 6\n";
        //cout << "STEP 3.4\n";
        //ENCONTRAR ALLY0 E ALLY1
        playerInfo ally[2];
        Ally(kicker, allyGoalie, ally);
        ally0 = ally[0];
        ally1 = ally[1];

        //ENTRADA DA MLP
        /*
         * ordem das entradas:
         * dados do kicker, dados do ally0, dados do ally1, dados do oppGoalie,
         * dados do opp0(podendo ser o oponente entre o gol e o atacante mais proximo do atacante ou somente o oponente mais proximo ao atacante),
         * dados do opp1(podendo ser o oponente mais proximo ou o segundo mais proximo do atacante);
        */
        //cout << "STEP 3.5\n";
        float inMlp[inLength] = {kicker.position.x(), kicker.position.y(), ally0.position.x(), ally0.position.y(),
                                     ally1.position.x(), ally1.position.y(), oppGoalie.position.x(), oppGoalie.position.y(),
                                     opp0.position.x(), opp0.position.y(), opp1.position.x(), opp1.position.y()};

        /*//Printando as posicões
        cout << "Posições passadas pela mlp:\n";
        cout << "\t" << kicker.position.x() << " " << kicker.position.y() << "\n";
        cout << "\t" << ally0.position.x() << " " << ally0.position.y() << "\n";
        cout << "\t" << ally1.position.x() << " " << ally1.position.y() << "\n";
        cout << "\t" << oppGoalie.position.x() << " " << oppGoalie.position.y() << "\n";
        cout << "\t" << opp0.position.x() << " " << opp0.position.y() << "\n";
        cout << "\t" << opp1.position.x() << " " << opp1.position.y() << "\n";
        *///printado

        //RESPOSTA DA MLP
        /*
         * pode ser 1 = chute a gol
         * pode ser 0 = passe
        */
        //cout << "STEP 3.6\n";
        float* mlpDecision;



        mlpDecision = MLP::forward(inMlp);
        //cout << "[MLP OUTPUT] ";
        for(int i=0;i<outputLength;i++){
          //  cout << mlpDecision[i] << " ";
        }
        //cout << "\n";

        return mlpDecision[0];

    }


    return -1;
}

//CONVERTER COORDENADAS
playerInfo Behaviour_Attacker :: calc(playerInfo jogador){
    float x, y, z;

    x = jogador.position.x();
    y = jogador.position.y();
    z = jogador.position.z();
    jogador.position.setPosition(-1*x, -1*y, z);

    return jogador;
}

//ENCONTRAR OPPGOALIE
playerInfo Behaviour_Attacker :: OppGoalie(playerInfo kicker){
    //inicializando oppGoalie
    playerInfo oppGoalie;
    oppGoalie.valid = false;

    float  menor_dist = 9999.0;
    float  dist;
    for(quint8 id = 0; id < getConstants()->getQtPlayers(); id++){
        if(PlayerBus::theirPlayerAvailable(id)){    //verifica se o id é valido
            dist = PlayerBus::theirPlayer(id)->distOurGoal();   //gol do time adversario
            if(dist < menor_dist){
                menor_dist = dist;
                oppGoalie.valid = true;
                oppGoalie.id = PlayerBus::theirPlayer(id)->playerId();
                oppGoalie.position = PlayerBus::theirPlayer(id)->position();
                //padronizando fieldSide
                if(player()->team()->fieldSide().isLeft()){
                    oppGoalie = calc(oppGoalie);
                }
                oppGoalie.distToKicker = PlayerBus::theirPlayer(id)->distanceTo(kicker.position);
            }
        }
    }

    //se o oppGoalie nao estiver no lado do seu time ou se nao tiver sido identificado
    if(oppGoalie.position.x() < 0 || !oppGoalie.valid){
        Position defaultPos(true, 18, 0, 0);
        oppGoalie.position = defaultPos;
        oppGoalie.distToKicker = 100;
        oppGoalie.id = 99;
    }

    return oppGoalie;
}

//ENCONTRAR ALLYGOALIE (PARA NAO USA-LO COMO POSSIVEL RECEIVER FUTURAMENTE)
playerInfo Behaviour_Attacker :: AllyGoalie(playerInfo kicker){
    //inicializando allygoalie
    playerInfo allyGoalie;
    allyGoalie.valid = false;

    float  menor_dist = 9999.0;
    float  dist;
    for(quint8 id = 0; id < getConstants()->getQtPlayers(); id++){
        if(PlayerBus::ourPlayerAvailable(id)){    //verifica se o id é valido
            dist = PlayerBus::ourPlayer(id)->distOurGoal();   //gol do time
            if(dist < menor_dist){
                menor_dist = dist;
                allyGoalie.valid = true;
                allyGoalie.id = PlayerBus::ourPlayer(id)->playerId();
                allyGoalie.position = PlayerBus::ourPlayer(id)->position();
                //padronizando fieldSide
                if(player()->team()->fieldSide().isLeft()){
                    allyGoalie = calc(allyGoalie);
                }
                allyGoalie.distToKicker = PlayerBus::ourPlayer(id)->distanceTo(kicker.position);
            }
        }
    }

    //se o allyGoalie nao estiver no lado do seu time ou se nao tiver sido identificado
    if(allyGoalie.position.x() > 0 || !allyGoalie.valid){
        Position defaultPos(true, -18, 0, 0);
        allyGoalie.position = defaultPos;
        allyGoalie.distToKicker = 100;
        allyGoalie.id = 99;
    }
    return allyGoalie;
}


//ENCONTRAR 2 ALIADOS MAIS PROXIMOS AO NOSSO ATACANTE
void Behaviour_Attacker :: Ally(playerInfo kicker, playerInfo allyGoalie, playerInfo ally[]){
    ally[0].valid = false;
    ally[1].valid = false;

    float minordist0 = 100, minordist1 = 100;
    playerInfo jogador;

    for(quint8 id = 0; id < getConstants()->getQtPlayers(); id++){
        if(PlayerBus::ourPlayerAvailable(id) && id != kicker.id && id != allyGoalie.id){
            jogador.valid = true;
            jogador.id = PlayerBus::ourPlayer(id)->playerId();
            jogador.position = PlayerBus::ourPlayer(id)->position();
            if(player()->team()->fieldSide().isLeft()){
                jogador = calc(jogador);
            }
            jogador.distToKicker = PlayerBus::ourPlayer(id)->distanceTo(kicker.position);
            //conferindo quem sao os dois aliados mais proximos ao nosso atacante que nao estejam muito atras dele
            if(jogador.position.x() < (kicker.position.x() - 1.0)){
                if(jogador.distToKicker < minordist0){
                    ally[1] = ally[0];
                    minordist1 = ally[1].distToKicker;
                    ally[0] = jogador;
                    minordist0 = ally[0].distToKicker;
                }else if(jogador.distToKicker < minordist1){
                    ally[1] = jogador;
                    minordist1 = ally[1].distToKicker;
                }
            }
        }
    }

    for(int i=0;i<2;i++){
        if(!ally[i].valid){
            ally[i].id = 99;
            Position defaultPos(true, -18, 0, 0);
            ally[i].position = defaultPos;
            ally[i].distToKicker = 100;
        }
    }

}

//ENCONTRAR 2 OPONENTES MAIS PROXIMOS (PODENDO O PRIMEIRO SER O OPP MAIS PROXIMO AO ATACANTE QUE ESTEJA ENTRE ELE E O GOL ADVERSARIO)
void Behaviour_Attacker :: Opp(playerInfo kicker, playerInfo oppGoalie, playerInfo opp[]){
    //cout << "Fazendo 1\n";
    playerInfo oppObst;
    opp[0].valid = false;
    opp[1].valid = false;
    oppObst.valid = false;

    //cout << "Fazendo 2\n";
    float minordist0 = 100, minordist1 = 100, minordistobst = 100;
    opp[0].distToKicker = 1000;
    opp[1].distToKicker = 1000;

    playerInfo jogador;

    for(quint8 id = 0; id < getConstants()->getQtPlayers(); id++){
        if(PlayerBus::theirPlayerAvailable(id) /* && id != kicker.id */ && id != oppGoalie.id){
            jogador.valid = true;
            jogador.id = PlayerBus::theirPlayer(id)->playerId();
            jogador.position = PlayerBus::theirPlayer(id)->position();
            if(player()->team()->fieldSide().isLeft()){
                jogador = calc(jogador);
            }
            jogador.distToKicker = PlayerBus::theirPlayer(id)->distanceTo(kicker.position);
            //conferindo quem sao os dois oponentes mais proximos ao nosso atacante que nao estejam muito atras dele
            //conferindo quem eh o oponente mais proximo ao nosso atacante que nao estejam muito atras dele e que esteja entre ele e o gol adversario
            //conferir pro outro lado tbm
            if(jogador.position.x() < (kicker.position.x())){

                if(isObst(kicker, jogador) && jogador.distToKicker < minordistobst){
                    oppObst = jogador;
                    minordistobst = oppObst.distToKicker;
                }
                if(jogador.distToKicker < minordist0){
                    opp[1] = opp[0];
                    minordist1 = opp[1].distToKicker;
                    opp[0] = jogador;
                    minordist0 = opp[0].distToKicker;
                }else if(jogador.distToKicker < minordist1){
                    opp[1] = jogador;
                    minordist1 = opp[1].distToKicker;
                }
            }
        }
    }

    //cout << "Fazendo 3\n";
    for(int i=0;i<2;i++){
        if(!opp[i].valid){
            opp[i].id = 99;
            Position defaultPos(true, 18, 0, 0);
            opp[i].position = defaultPos;
            opp[i].distToKicker = 100;
        }
    }

    //cout << "Fazendo 4\n";
    if(oppObst.valid){
        if(oppObst.id != opp[0].id){
            opp[1] = opp[0];
            opp[0] = oppObst;
        }else{
            opp[0] = oppObst;
            //opp[1] nao muda
        }
    }

    //return opp;
}

bool Behaviour_Attacker::isObst(playerInfo kicker, playerInfo jogador){
    Point2d kickerPos;
    kickerPos.setCoords(kicker.position.x(),kicker.position.y());

    Point2d goalLeft, goalRight;

    //padronizar gol adversario porque as coordenadas foram corrigidas p/ considerar que o gol oponente eh o gol direito
    if(player()->team()->fieldSide().isLeft()){

        goalLeft.setCoords(loc()->ourGoalLeftPost().x(), loc()->ourGoalLeftPost().y());
        //goalLeft.x = _ourTeam->loc()->ourGoalLeftPost().x();
        //goalLeft.y = _ourTeam->loc()->ourGoalLeftPost().y();

        goalRight.setCoords(loc()->ourGoalRightPost().x(),loc()->ourGoalRightPost().y());
        //goalRight.x = _ourTeam->loc()->ourGoalRightPost().x();
        //goalRight.y = _ourTeam->loc()->ourGoalRightPost().y();
    }else{
        goalLeft.setCoords(loc()->theirGoalLeftPost().x(), loc()->theirGoalLeftPost().y());
        //goalLeft.x = _theirTeam->loc()->ourGoalLeftPost().x();
        //goalLeft.y = _theirTeam->loc()->ourGoalLeftPost().y();

        goalRight.setCoords(loc()->theirGoalRightPost().x(),loc()->theirGoalRightPost().y());
        //goalRight.x = _theirTeam->loc()->ourGoalRightPost().x();
        //goalRight.y = _theirTeam->loc()->ourGoalRightPost().y();
    }

    Triangle T;
    T.A = kickerPos;
    T.B = goalLeft;
    T.C = goalRight;
    T.calcSidesAndArea();

    Point2d oppPos;
    oppPos.setCoords(jogador.position.x(),jogador.position.y());
    //oppPos.x = jogador.position.x();
    //oppPos.y = jogador.position.y();

    Triangle T1;
    T1.A = kickerPos;
    T1.B = goalLeft;
    T1.C = oppPos;
    T1.calcSidesAndArea();

    Triangle T2;
    T2.A = kickerPos;
    T2.B = goalRight;
    T2.C = oppPos;
    T2.calcSidesAndArea();

    Triangle T3;
    T3.A = goalLeft;
    T3.B = goalRight;
    T3.C = oppPos;
    T3.calcSidesAndArea();

    //comparando areas (floats)
    bool oppIsObst;
    static constexpr auto epsilon = 1.0e-01f;
    oppIsObst = (qAbs(T.area - (T1.area+T2.area+T3.area)) <= epsilon);
    if(oppIsObst == false){
        oppIsObst = qAbs(T.area - (T1.area+T2.area+T3.area)) <= (epsilon * qMax(qAbs(T.area), qAbs(T1.area+T2.area+T3.area)));
    }
    //se o oponente estiver entre o kicker e o gol
    return oppIsObst;
}
