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


#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <QReadWriteLock>

#include <proto/messages.pb.h>
#include <src/constants/constants.h>
#include <src/utils/types/fieldside/fieldside.h>

class Locations
{
public:
    Locations(Constants *constants);

    // Side
    FieldSide ourSide();
    FieldSide theirSide();

    // Field
    Position fieldCenter();
    Position fieldRightTopCorner();
    Position fieldRightBottomCorner();
    Position fieldLeftTopCorner();
    Position fieldLeftBottomCorner();

    float fieldMinX();
    float fieldMaxX();
    float fieldMinY();
    float fieldMaxY();
    float fieldLength();
    float fieldWidth();
    float fieldCenterRadius();
    float fieldDefenseLength();
    float fieldDefenseWidth();
    float fieldDefenseRadius();
    float fieldDefenseStretch();
    float fieldGoalDepth();

    // Our
    Position ourFieldTopCorner();
    Position ourFieldBottomCorner();
    Position ourGoal();
    Position ourGoalRightPost();
    Position ourGoalLeftPost();
    Position ourPenaltyMark();

    // Their
    Position theirFieldTopCorner();
    Position theirFieldBottomCorner();
    Position theirGoal();
    Position theirGoalRightPost();
    Position theirGoalLeftPost();
    Position theirPenaltyMark();

    // Update geometry
    void updateFieldData(Field field);

private:
    // Constants
    Constants *_constants;
    Constants* getConstants();

    // Field data
    Field _field;

    // Field positions
    Position _fieldCenter;
    Position _fieldTopRightCorner;
    Position _fieldTopLeftCorner;
    Position _fieldBottomLeftCorner;
    Position _fieldBottomRightCorner;
    Position _rightPenaltyMark;
    Position _leftPenaltyMark;
    Position _rightGoalLeftPost;
    Position _rightGoalRightPost;
    Position _leftGoalLeftPost;
    Position _leftGoalRightPost;
    float _fieldCenterRadius;
    float _goalLength;
    float _goalWidth;
    float _goalDepth;

    // Mutex for data management
    QReadWriteLock _mutex;
};

#endif // LOCATIONS_H
