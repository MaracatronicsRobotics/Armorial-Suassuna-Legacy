/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
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
#include "locations.h"
#include <entity/contromodule/mrcteam.h>
#include <utils/utils.hh>
#include <entity/player/player.h>

Locations::Locations(MRCTeam *team) {
    _team = team;
}
WorldMap* Locations::wm() const {
    return _team->wm();
}
FieldSide Locations::ourSide() const {
    return _team->fieldSide();
}
FieldSide Locations::theirSide() const {
    return ourSide().isRight() ? Sides::LEFT : Sides::RIGHT;
}
Position Locations::ourFieldTopCorner() const {
    return ourSide().isRight() ? wm()->fieldTopRightCorner() : wm()->fieldTopLeftCorner();
}
Position Locations::ourFieldBottomCorner() const {
    return ourSide().isRight() ? wm()->fieldBottomRightCorner() : wm()->fieldBottomLeftCorner();
}
Position Locations::ourGoal() const {
    return Position(true, (ourSide().isRight()?wm()->rightGoal():wm()->leftGoal()).leftPost().x(), 0.0, 0.0);
}
Position Locations::ourGoalRightPost() const {
    return (ourSide().isRight()?wm()->rightGoal():wm()->leftGoal()).rightPost();
}
Position Locations::ourGoalRightMidPost() const {
    return Position(true, ourGoalRightPost().x(), ourGoalRightPost().y()/2, 0.0);
}
Position Locations::ourGoalLeftPost() const {
    return (ourSide().isRight()?wm()->rightGoal():wm()->leftGoal()).leftPost();
}
Position Locations::ourGoalLeftMidPost() const {
    return Position(true, ourGoalLeftPost().x(), ourGoalLeftPost().y()/2, 0.0);
}
Position Locations::ourPenaltyMark() const {
    return ourSide().isRight() ? wm()->rightPenaltyMark() : wm()->leftPenaltyMark();
}
Position Locations::theirFieldTopCorner() const {
    return ourSide().isRight() ? wm()->fieldTopLeftCorner() : wm()->fieldTopRightCorner();
}
Position Locations::theirFieldBottomCorner() const {
    return ourSide().isRight() ? wm()->fieldBottomLeftCorner() : wm()->fieldBottomLeftCorner();
}
Position Locations::theirGoal() const {
    return Position(true, (ourSide().isRight()?wm()->leftGoal():wm()->rightGoal()).leftPost().x(), 0.0, 0.0);
}
Position Locations::theirGoalRightPost() const {
    return (ourSide().isRight()?wm()->leftGoal():wm()->rightGoal()).rightPost();
}
Position Locations::theirGoalRightMidPost() const {
    return Position(true, theirGoalRightPost().x(), theirGoalRightPost().y()/2, 0.0);
}
Position Locations::theirGoalLeftPost() const {
    return (ourSide().isRight()?wm()->leftGoal():wm()->rightGoal()).leftPost();
}
Position Locations::theirGoalLeftMidPost() const {
    return Position(true, theirGoalLeftPost().x(), theirGoalLeftPost().y()/2, 0.0);
}
Position Locations::theirPenaltyMark() const {
    return ourSide().isRight() ? wm()->leftPenaltyMark() : wm()->leftPenaltyMark();
}
float Locations::fieldMinX() const {
    return -fabs(ourFieldTopCorner().x());
}
float Locations::fieldMaxX() const {
    return fabs(ourFieldTopCorner().x());
}
float Locations::fieldMinY() const {
    return -fabs(ourFieldTopCorner().y());
}
float Locations::fieldMaxY() const {
    return fabs(ourFieldTopCorner().y());
}
float Locations::fieldLength() const {
    return 2*fieldMaxX();
}
float Locations::fieldWidth() const {
    return 2*fieldMaxY();
}
float Locations::fieldCenterRadius() const {
    return wm()->fieldCenterRadius();
}
float Locations::fieldDefenseRadius() const {
    return fieldDefenseWidth();
}
float Locations::fieldDefenseStretch() const {
    return fieldDefenseLength()-2*fieldDefenseRoundedRadius();
}
float Locations::fieldGoalDepth() const {
    return wm()->rightGoal().getDepth();
}
float Locations::fieldDefenseLength() const {
    return wm()->rightGoal().getAreaLength();
}
float Locations::fieldDefenseWidth() const {
    return wm()->rightGoal().getAreaWidth();
}
float Locations::fieldDefenseRoundedRadius() const {
    return wm()->rightGoal().getAreaRoundedRadius();
}
Position Locations::fieldCenter() const {
    return wm()->fieldCenter();
}
Position Locations::fieldRightTopCorner() const {
    return Position(true, this->fieldMaxX(), this->fieldMaxY(), 0.0);
}
Position Locations::fieldRightBottomCorner() const {
    return Position(true, this->fieldMaxX(), this->fieldMinY(), 0.0);
}
Position Locations::fieldLeftTopCorner() const {
    return Position(true, this->fieldMinX(), this->fieldMaxY(), 0.0);
}
Position Locations::fieldLeftBottomCorner() const {
    return Position(true, this->fieldMinX(), this->fieldMinY(), 0.0);
}
Position Locations::ball() const {
    return wm()->ballPosition(0);
}
Velocity Locations::ballVelocity() const {
    return wm()->ballVelocity(0);
}
float Locations::distBallOurGoal() const {
    return WR::Utils::distance(ourGoal(), ball());
}
float Locations::distBallOurRightPost() const {
    return WR::Utils::distance(ourGoalRightPost(), ball());
}
float Locations::distBallOurRightMidPost() const {
    return WR::Utils::distance(ourGoalRightMidPost(), ball());
}
float Locations::distBallOurLeftPost() const {
    return WR::Utils::distance(ourGoalLeftPost(), ball());
}
float Locations::distBallOurLeftMidPost() const {
    return WR::Utils::distance(ourGoalLeftMidPost(), ball());
}
float Locations::distBallTheirGoal() const {
    return WR::Utils::distance(theirGoal(), ball());
}
float Locations::distBallTheirRightPost() const {
    return WR::Utils::distance(theirGoalRightPost(), ball());
}
float Locations::distBallTheirRightMidPost() const {
    return WR::Utils::distance(theirGoalRightMidPost(), ball());
}
float Locations::distBallTheirLeftPost() const {
    return WR::Utils::distance(theirGoalLeftPost(), ball());
}
float Locations::distBallTheirLeftMidPost() const {
    return WR::Utils::distance(theirGoalLeftMidPost(), ball());
}
bool Locations::isInsideOurField(const Position &pos) {
    return ((ourSide().isRight() && pos.x()>=0) || (ourSide().isLeft() && pos.x()<=0));
}
bool Locations::isInsideTheirField(const Position &pos) {
    return (isInsideOurField(pos)==false);
}
bool Locations::isInsideOurArea(const Position &pos, float factor) {
    return _isInsideArea(pos, factor, ourGoal(), ourGoalLeftMidPost(), ourGoalRightMidPost());
}
bool Locations::isInsideTheirArea(const Position &pos, float factor) {
    return _isInsideArea(pos, factor, theirGoal(), theirGoalLeftMidPost(), theirGoalRightMidPost());
}
bool Locations::isOutsideField(const Position &pos, float factor) {
    return _isOutsideField(pos, factor*fieldMaxX(), factor*fieldMaxY());
}
bool Locations::isOutsideField(const Position &pos, const float dx, const float dy) {
    return _isOutsideField(pos, fieldMaxX()+dx, fieldMaxY()+dy);
}
bool Locations::isInsideField(const Position &pos, float factor) {
    return (!isOutsideField(pos, factor));
}
bool Locations::isInsideField(const Position &pos, float dx, float dy) {
    return (!isOutsideField(pos, dx, dy));
}
bool Locations::_isInsideArea(const Position &pos, float factor, const Position &goal, const Position &goalLeftMidPost, const Position &goalRightMidPost) {
    // Distance to goal center, mid right post and mid left post
    float distGoal = WR::Utils::distance(pos, goal);
    float distLeftMidPost = WR::Utils::distance(pos, goalLeftMidPost);
    float distRightMidPost = WR::Utils::distance(pos, goalRightMidPost);
    float areaRadius = factor*fieldDefenseRadius();
    return (distGoal<areaRadius || distLeftMidPost<areaRadius || distRightMidPost<areaRadius);

    return false;
}
bool Locations::_isOutsideField(const Position &pos, const float maxX, const float maxY) {
    if(fabs(pos.x()) > maxX)
        return true;
    if(fabs(pos.y()) > maxY)
        return true;
    return false;
}
bool Locations::isNearWall(const Position &pos, float distance, Walls::Wall *wall) {
    const float stuckX = fabs(ourFieldTopCorner().x()) - distance;
    const float stuckY = fabs(ourFieldTopCorner().y()) - distance;
    // Inside field
    if(fabs(pos.y())<=stuckY && fabs(pos.x())<=stuckX)
        return false;
    // Our upper corner
    if(WR::Utils::distance(ourFieldTopCorner(), ball()) < 0.15) {
        if(wall!=NULL)
            *wall = Walls::OUR_UPPER_CORNER;
        return true;
    }
    // Their upper corner
    if(WR::Utils::distance(theirFieldTopCorner(), ball()) < 0.15) {
        if(wall!=NULL)
            *wall = Walls::THEIR_UPPER_CORNER;
        return true;
    }
    // Our bottom corner
    if(WR::Utils::distance(ourFieldBottomCorner(), ball()) < 0.15) {
        if(wall!=NULL)
            *wall = Walls::OUR_BOTTOM_CORNER;
        return true;
    }
    // Their bottom corner
    if(WR::Utils::distance(theirFieldBottomCorner(), ball()) < 0.15) {
        if(wall!=NULL)
            *wall = Walls::THEIR_BOTTOM_CORNER;
        return true;
    }
    // Upper wall
    if(pos.y()>stuckY) {
        if(wall!=NULL)
            *wall = Walls::UPPER;
        return true;
    }
    // Bottom wall
    if(pos.y()<-stuckY) {
        if(wall!=NULL)
            *wall = Walls::BOTTOM;
        return true;
    }
    const float goalPostY = fabs(this->ourGoalRightPost().y());
    // Right wall
    if(pos.x()>stuckX) {
        if(fabs(pos.y())>goalPostY) {
            if(wall!=NULL)
                *wall = Walls::RIGHT;
            return true;
        }
    }
    // Left wall
    if(pos.x()<-stuckX) {
        if(fabs(pos.y())>goalPostY) {
            if(wall!=NULL)
                *wall = Walls::LEFT;
            return true;
        }
    }
    return false;
}
// Check is a given position is near to an adversary player
bool Locations::isNearOpponent(const Position &pos, float distance) {
    // Makes a list containing pointer to opponent players
    QList<Player*> advPlayers = _team->opTeam()->avPlayers().values();
    // Interates over all of them
    for(int i = 0; i < advPlayers.size(); i++) {
        Player *advPlayer = advPlayers.at(i);
        // Checks if one of them is closer or at the given distance
        if(WR::Utils::distance(pos, advPlayer->position()) <= distance){
            return true;
        }
    }
     return false;
}
Position Locations::ourAreaMargin(const Position &reference, float distance) {
    const Position midL = ourGoalLeftMidPost();
    const Position midR = ourGoalRightMidPost();
    Position projected = WR::Utils::projectPointAtSegment(midR, midL, reference);
    if(reference.x() > fieldMinX() && reference.x() < fieldMaxX())
        return WR::Utils::threePoints(projected, reference, fieldDefenseRadius()+distance, 0.0);
    else {
        float newX = (reference.x() < fieldMinX()? fieldMinX() : fieldMaxX());
        Position newReference(true, newX, reference.y(), 0.0);
        return WR::Utils::threePoints(projected, newReference, fieldDefenseRadius()+distance, 0.0);
    }
}
Position Locations::theirAreaMargin(const Position &reference, float distance) {
    const Position midL = theirGoalLeftMidPost();
    const Position midR = theirGoalRightMidPost();
    Position projected = WR::Utils::projectPointAtSegment(midR, midL, reference);
    return WR::Utils::threePoints(projected, reference, fieldDefenseRadius()+distance, 0.0);
}

QHash<quint8, Player*> Locations::getOpPlayers(){
    return _team->opTeam()->avPlayers();
}

QHash<quint8, Player*> Locations::getMRCPlayers(){
    return _team->avPlayers();
}

