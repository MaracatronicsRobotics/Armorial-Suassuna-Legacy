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

#include "worldmapupdater.h"

#include <utils/basics/color.hh>
#include <utils/utils.hh>
#include <utils/fields/fields.hh>
#include <utils/fields/wrfields.hh>

#include <entity/coachview/coachview.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/coachview/mainwindow.h>

WorldMapUpdater::WorldMapUpdater(Controller *ctr, Fields::Field *defaultField) {
    _ctr = ctr;
    _defaultField = defaultField;
    // Initialize
    _lastBallPosition.setUnknown();
}

WorldMapUpdater::~WorldMapUpdater() {

}

void WorldMapUpdater::update(WorldMap *wm) {
    // Update field geometry
    updateFieldGeometry(wm);

    // Update ball and players
    updateBall(wm);
    updateTeam(wm, Colors::YELLOW);
    updateTeam(wm, Colors::BLUE);

    // Ball possession
    updateBallPossession(wm);
}

void WorldMapUpdater::updateFieldGeometry(WorldMap *wm) {
    // Set constant positions
    wm->setFieldCenter(_ctr->fieldCenter());

    // Update field corners
    wm->setFieldBottomLeftCorner(_ctr->fieldBottomLeftCorner());
    wm->setFieldBottomRightCorner(_ctr->fieldBottomRightCorner());
    wm->setFieldTopLeftCorner(_ctr->fieldTopLeftCorner());
    wm->setFieldTopRightCorner(_ctr->fieldTopRightCorner());

    // Update goal/posts positions
    wm->setRightGoalPosts(_ctr->rightGoal().leftPost(), _ctr->rightGoal().rightPost());
    wm->setLeftGoalPosts(_ctr->leftGoal().leftPost(), _ctr->leftGoal().rightPost());

    // Update goal area
    wm->setGoalArea(_ctr->rightGoal().getAreaLength(), _ctr->rightGoal().getAreaWidth(), _ctr->rightGoal().getAreaRoundedRadius());
    wm->setGoalDepth(_ctr->rightGoal().getDepth());

    // Penalty marks
    wm->setRightPenaltyMark(_ctr->rightPenaltyMark());
    wm->setLeftPenaltyMark(_ctr->leftPenaltyMark());

    // Center radius
    wm->setFieldCenterRadius(_ctr->fieldCenterRadius());
}

void WorldMapUpdater::updateBall(WorldMap *wm) {
    if(_ctr->balls().size()<=0)
        return;

    // Ball position (with unknown position check)
    const Position ctrBall = _ctr->ballPosition(0);
    Position posBall = ctrBall;
    if(posBall.isUnknown()) {
        if(_lastBallPosition.isUnknown())
            posBall.setPosition(0.0, 0.0, 0.0);
        else
            posBall = _lastBallPosition;
    }
    _lastBallPosition = posBall;
    wm->setBallPosition(0, posBall);

    // Ball velocity (with unknown position check)
    Velocity velBall = (ctrBall.isUnknown()? Velocity(true,0,0) : _ctr->ballVelocity(0));
    wm->setBallVelocity(0, velBall);
}

void WorldMapUpdater::updateTeam(WorldMap *wm, quint8 teamId) {
    const QList<quint8> ctrPlayers = _ctr->players(teamId);
    QList<quint8>::const_iterator it;
    //printf("Team id = %d\n", teamId);

    for(it=ctrPlayers.constBegin(); it!=ctrPlayers.end(); it++) {
        const quint8 player = *it;
        // Pos, ori and vel
        wm->setPlayerPosition(teamId, player, _ctr->playerPosition(teamId, player));
        wm->setPlayerOrientation(teamId, player, _ctr->playerOrientation(teamId, player));
        wm->setPlayerVelocity(teamId, player, _ctr->playerVelocity(teamId, player));
        wm->setPlayerAngularSpeed(teamId, player, _ctr->playerAngularSpeed(teamId, player));
        wm->setKickEnabled(teamId, player, _ctr->kickEnabled(teamId, player));
        wm->setDribbleEnabled(teamId, player, _ctr->dribbleEnabled(teamId, player));
        wm->setBatteryCharge(teamId, player, _ctr->batteryCharge(teamId, player));
        wm->setCapacitorCharge(teamId, player, _ctr->capacitorCharge(teamId, player));
    }

}

void WorldMapUpdater::updateBallPossession(WorldMap *wm) {
    const Position posBall = wm->ballPosition(0);
    const QList<quint8> teams = wm->teams();
    qint8 closestTeam = -1, closestPlayer = -1;
    float minDist = 999;
    float offSetDist = 0.25f;

    // Iterate teams
    QList<quint8>::const_iterator itTeam;
    for(itTeam=teams.constBegin(); itTeam!=teams.constEnd(); itTeam++) {
        const quint8 team = *itTeam;
        const QList<quint8> players = wm->players(team);
        // Iterate players
        QList<quint8>::const_iterator itPlayer;
        for(itPlayer=players.constBegin(); itPlayer!=players.constEnd(); itPlayer++) {
            const quint8 player = *itPlayer;
            if(wm->playerPosition(team, player).isUnknown())
                continue;
            // Find closest player
            float dist = WR::Utils::distance(posBall, wm->playerPosition(team, player));
            if(dist<minDist && dist < offSetDist) {
                minDist = dist;
                closestTeam = team;
                closestPlayer = player;
            }
        }
    }

    // Iterate teams
    for(itTeam=teams.constBegin(); itTeam!=teams.constEnd(); itTeam++) {
        const quint8 team = *itTeam;
        const QList<quint8> players = wm->players(team);
        // Iterate players
        QList<quint8>::const_iterator itPlayer;
        for(itPlayer=players.constBegin(); itPlayer!=players.constEnd(); itPlayer++) {
            const quint8 player = *itPlayer;
            if(wm->playerPosition(team, player).isUnknown())
                continue;
            // Update WorldMap with ball possession
            if(wm->ballPossession(team, player))
                wm->setBallPossession(team, player, false);
            if(player==closestPlayer && team==closestTeam)
                wm->setBallPossession(team, player, true);
        }
    }
}

void WorldMapUpdater::setDefaultFieldGeometry(WorldMap *wm) {
    // Load default dimensions based on competition
    _defaultField = new Fields::SSL2015();
    const float fieldX = _defaultField->maxX();
    const float fieldY = _defaultField->maxY();
    const float goalY = _defaultField->goalWidth()/2;
    const float penaltyDist = _defaultField->defenseRadius();

    // Set constant positions
    wm->setFieldCenter(Position(true, 0.0, 0.0, 0.0));

    // Update field corners
    wm->setFieldBottomLeftCorner(Position(true, -fieldX, -fieldY, 0.0));
    wm->setFieldBottomRightCorner(Position(true, fieldX, -fieldY, 0.0));
    wm->setFieldTopLeftCorner(Position(true, -fieldX, fieldY, 0.0));
    wm->setFieldTopRightCorner(Position(true, fieldX, fieldY, 0.0));

    // Update goal/posts positions
    wm->setRightGoalPosts(Position(true, fieldX, -goalY, 0.0), Position(true, fieldX, goalY, 0.0));
    wm->setLeftGoalPosts(Position(true, -fieldX, goalY, 0.0), Position(true, -fieldX, -goalY, 0.0));

    // Penalty marks
    Position fieldCenter(true, 0.0, 0.0, 0.0);
    Position rightGoal(true, fieldX, 0.0, 0.0);
    Position leftGoal(true, -fieldX, 0.0, 0.0);
    wm->setRightPenaltyMark(WR::Utils::threePoints(rightGoal, fieldCenter, penaltyDist, 0.0));
    wm->setLeftPenaltyMark(WR::Utils::threePoints(leftGoal, fieldCenter, penaltyDist, 0.0));
}
