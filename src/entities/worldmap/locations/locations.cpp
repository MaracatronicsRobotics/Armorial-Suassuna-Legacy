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


#include "locations.h"

#include <math.h>
#include <src/utils/text/text.h>
#include <src/utils/utils.h>

Locations::Locations(Constants *constants) {
    _constants = constants;
}

void Locations::updateFieldData(Field field) {
    // Update local field var
    _field = field;

    // Load default dimensions based on defaultField
    const float fieldX = _field.fieldlength()/2;
    const float fieldY = _field.fieldwidth()/2;
    const float goalY = _field.goalwidth()/2;
    const float penaltyDist = _field.defensewidth();

    // Set constant positions
    _fieldCenter = /*Position(true, 0.0, 0.0)*/ Position();
    _fieldCenterRadius = _field.centerradius();
    _goalDepth = _field.goaldepth();
    _goalWidth = _field.goalwidth();
    _goalLength = fieldDefenseStretch() + 2*_goalWidth;

    // Update field corners
    _fieldBottomLeftCorner  = Utils::getPositionObject(-fieldX, -fieldY);
    _fieldBottomRightCorner = Utils::getPositionObject(fieldX, -fieldY);
    _fieldTopLeftCorner     = Utils::getPositionObject(-fieldX, fieldY);
    _fieldTopRightCorner    = Utils::getPositionObject(fieldX, fieldY);

    // Update goal/posts positions
    _rightGoalLeftPost = Utils::getPositionObject(fieldX, -goalY);
    _rightGoalRightPost = Utils::getPositionObject(fieldX, goalY);
    _leftGoalLeftPost = Utils::getPositionObject(-fieldX, goalY);
    _leftGoalRightPost = Utils::getPositionObject(-fieldX, -goalY);

    // Penalty marks
    _rightPenaltyMark = Utils::getPositionObject(penaltyDist, 0.0);
    _leftPenaltyMark = Utils::getPositionObject(-penaltyDist, 0.0);
}

FieldSide Locations::ourSide() {
    return getConstants()->getTeamSide();
}

FieldSide Locations::theirSide() {
    return getConstants()->getOppositeSide();
}

Position Locations::fieldCenter() {
    Position fieldCenter = _fieldCenter;

    return fieldCenter;
}

float Locations::fieldMinX()  {
    float fMinX = -fabs(ourFieldTopCorner().x());

    return fMinX;
}

float Locations::fieldMaxX()  {
    float fMaxX = fabs(ourFieldTopCorner().x());

    return fMaxX;
}

float Locations::fieldMinY()  {
    float fMinY = -fabs(ourFieldTopCorner().y());

    return fMinY;
}

float Locations::fieldMaxY()  {
    float fMaxY = fabs(ourFieldTopCorner().y());

    return fMaxY;
}

float Locations::fieldLength() {
    float fMaxX = fieldMaxX();

    _mutex.lockForRead();
    float fLength = 2*fMaxX;
    _mutex.unlock();

    return fLength;
}

float Locations::fieldWidth()  {
    float fMaxY = fieldMaxY();

    _mutex.lockForRead();
    float fWidth = 2*fMaxY;
    _mutex.unlock();

    return fWidth;
}

Position Locations::fieldRightTopCorner() {
    float fMaxX = fieldMaxX();
    float fMaxY = fieldMaxY();

    _mutex.lockForRead();
    Position fRightTopCorner = Utils::getPositionObject(fMaxX, fMaxY);
    _mutex.unlock();

    return fRightTopCorner;
}

Position Locations::fieldRightBottomCorner() {
    float fMaxX = fieldMaxX();
    float fMinY = fieldMinY();

    _mutex.lockForRead();
    Position fRightBottomCorner = Utils::getPositionObject(fMaxX, fMinY);
    _mutex.unlock();

    return fRightBottomCorner;
}

Position Locations::fieldLeftTopCorner() {
    float fMinX = fieldMinX();
    float fMaxY = fieldMaxY();

    _mutex.lockForRead();
    Position fLeftTopCorner = Utils::getPositionObject(fMinX, fMaxY);
    _mutex.unlock();

    return fLeftTopCorner;
}

Position Locations::fieldLeftBottomCorner() {
    float fMinX = fieldMinX();
    float fMinY = fieldMinY();

    _mutex.lockForRead();
    Position fLeftBottomCorner = Utils::getPositionObject(fMinX, fMinY);
    _mutex.unlock();

    return fLeftBottomCorner;
}

Position Locations::ourFieldTopCorner() {
    _mutex.lockForRead();
    Position ourFTopCorner = (ourSide().isRight() ? _fieldTopRightCorner : _fieldTopLeftCorner);
    _mutex.unlock();

    return ourFTopCorner;
}

Position Locations::ourFieldBottomCorner() {
    _mutex.lockForRead();
    Position ourFBottomCorner = (ourSide().isRight() ? _fieldBottomRightCorner : _fieldBottomLeftCorner);
    _mutex.unlock();

    return ourFBottomCorner;
}

Position Locations::ourGoal() {
    _mutex.lockForRead();
    Position ourG = Utils::getPositionObject((ourSide().isRight() ? _rightGoalLeftPost : _leftGoalLeftPost).x(), 0.0);
    _mutex.unlock();

    return ourG;
}

Position Locations::ourGoalRightPost() {
    _mutex.lockForRead();
    Position ourGRightPost = (ourSide().isRight() ? _rightGoalRightPost : _leftGoalRightPost);
    _mutex.unlock();

    return ourGRightPost;
}

Position Locations::ourGoalLeftPost() {
    _mutex.lockForRead();
    Position ourGLeftPost = (ourSide().isRight() ? _rightGoalLeftPost : _leftGoalLeftPost);
    _mutex.unlock();

    return ourGLeftPost;
}

Position Locations::ourPenaltyMark() {
    _mutex.lockForRead();
    Position ourPMark = (ourSide().isRight() ? _rightPenaltyMark : _leftPenaltyMark);
    _mutex.unlock();

    return ourPMark;
}

Position Locations::theirFieldTopCorner() {
    _mutex.lockForRead();
    Position theirFTopCorner = theirSide().isRight() ? _fieldTopRightCorner : _fieldTopLeftCorner;
    _mutex.unlock();

    return theirFTopCorner;
}

Position Locations::theirFieldBottomCorner() {
    _mutex.lockForRead();
    Position theirFBottomCorner = (theirSide().isRight() ? _fieldBottomRightCorner : _fieldBottomLeftCorner);
    _mutex.unlock();

    return theirFBottomCorner;
}

Position Locations::theirGoal() {
    _mutex.lockForRead();
    Position theirG = Utils::getPositionObject((theirSide().isRight() ? _rightGoalLeftPost : _leftGoalLeftPost).x(), 0.0);
    _mutex.unlock();

    return theirG;
}

Position Locations::theirGoalRightPost() {
    _mutex.lockForRead();
    Position theirGRightPost = (theirSide().isRight() ? _rightGoalRightPost : _leftGoalRightPost);
    _mutex.unlock();

    return theirGRightPost;
}

Position Locations::theirGoalLeftPost() {
    _mutex.lockForRead();
    Position theirGLeftPost = (theirSide().isRight() ? _rightGoalLeftPost : _leftGoalLeftPost);
    _mutex.unlock();

    return theirGLeftPost;
}

Position Locations::theirPenaltyMark() {
    _mutex.lockForRead();
    Position theirPMark = (theirSide().isRight() ? _rightPenaltyMark : _leftPenaltyMark);
    _mutex.unlock();

    return theirPMark;
}

float Locations::fieldCenterRadius() {
    _mutex.lockForRead();
    float fCenterRadius = _fieldCenterRadius;
    _mutex.unlock();

    return fCenterRadius;
}

float Locations::fieldDefenseLength() {
    _mutex.lockForRead();
    float fDefenseLength = _goalLength;
    _mutex.unlock();

    return fDefenseLength;
}

float Locations::fieldDefenseWidth() {
    _mutex.lockForRead();
    float fDefenseWidth = _goalWidth;
    _mutex.unlock();

    return fDefenseWidth;
}

float Locations::fieldDefenseRadius() {
    _mutex.lockForRead();
    float fDefenseRadius = _field.defensewidth();
    _mutex.unlock();

    return fDefenseRadius;
}

float Locations::fieldDefenseStretch() {
    _mutex.lockForRead();
    float fDefenseStretch = _field.defenselength();
    _mutex.unlock();

    return fDefenseStretch;
}

float Locations::fieldGoalDepth() {
    _mutex.lockForRead();
    float fGoalDepth = _goalDepth;
    _mutex.unlock();

    return fGoalDepth;
}

Constants* Locations::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at Locations") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
