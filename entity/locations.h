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

#ifndef LOCATIONS_HH
#define LOCATIONS_HH
#include <utils/fieldside/fieldside.hh>
#include <utils/basics/wall.hh>
#include <entity/contromodule/mrcteam.h>
// Constant Locations values
#define VSS_GOAL_ARCRADIUS 0.12
class Locations {
public:
    Locations(MRCTeam *team);
    // Side
    FieldSide ourSide() const;
    FieldSide theirSide() const;
    // Field positions
    Position fieldCenter() const;
    Position fieldRightTopCorner() const;
    Position fieldRightBottomCorner() const;
    Position fieldLeftTopCorner() const;
    Position fieldLeftBottomCorner() const;
    Position ourFieldTopCorner() const;
    Position ourFieldBottomCorner() const;
    Position ourGoal() const;
    Position ourGoalRightPost() const;
    Position ourGoalRightMidPost() const;
    Position ourGoalLeftPost() const;
    Position ourGoalLeftMidPost() const;
    Position ourPenaltyMark() const;
    Position ourAreaPostLeft() const;
    Position ourAreaPostRight() const;
    Position ourAreaPostLeftInside() const;
    Position ourAreaPostRightInside() const;

    Position theirFieldTopCorner() const;
    Position theirFieldBottomCorner() const;
    Position theirGoal() const;
    Position theirGoalRightPost() const;
    Position theirGoalRightMidPost() const;
    Position theirGoalLeftPost() const;
    Position theirGoalLeftMidPost() const;
    Position theirPenaltyMark() const;
    Position theirAreaPostLeft() const;
    Position theirAreaPostRight() const;
    Position theirAreaPostLeftInside() const;
    Position theirAreaPostRightInside() const;

    // Field
    float fieldMinX() const;
    float fieldMaxX() const;
    float fieldMinY() const;
    float fieldMaxY() const;
    float fieldLength() const;
    float fieldWidth() const;
    float fieldCenterRadius() const;
    float fieldDefenseLength() const;
    float fieldDefenseWidth() const;
    float fieldDefenseRoundedRadius() const;
    float fieldDefenseRadius() const;
    float fieldDefenseStretch() const;
    float fieldGoalDepth() const;
    // Distances
    float distBallOurGoal() const;
    float distBallOurRightPost() const;
    float distBallOurRightMidPost() const;
    float distBallOurLeftPost() const;
    float distBallOurLeftMidPost() const;
    float distBallTheirGoal() const;
    float distBallTheirRightPost() const;
    float distBallTheirRightMidPost() const;
    float distBallTheirLeftPost() const;
    float distBallTheirLeftMidPost() const;
    // Ball
    Position ball() const;
    Velocity ballVelocity() const;
    // Auxiliar functions
    bool isInsideOurField(const Position &pos);
    bool isInsideTheirField(const Position &pos);
    bool isInsideOurArea(const Position &pos, float factor = 1.0);
    bool isInsideTheirArea(const Position &pos, float factor = 1.0);
    bool isOutsideField(const Position &pos, float factor = 1.0);
    bool isOutsideField(const Position &pos, float dx, float dy);
    bool isInsideField(const Position &pos, float factor = 1.0);
    bool isInsideField(const Position &pos, float dx, float dy);
    bool isNearWall(const Position &pos, float distance, Walls::Wall *wall = NULL);
    bool isNearOpponent(const Position &pos, float distance);
    Position ourAreaMargin(const Position &reference, float distance);
    Position theirAreaMargin(const Position &reference, float distance);
    //
    QHash<quint8, Player*> getOpPlayers();
    QHash<quint8, Player*> getMRCPlayers();

private:
    // Team access
    MRCTeam *_team;
    // Internal acess
    WorldMap* wm() const;
    // Internal
    bool _isInsideArea(const Position &pos, float factor, const Position &goalLeftPost, const Position &goalRightDeslocatedPost);
    bool _isOutsideField(const Position &pos, const float maxX, const float maxY);
};
#endif // LOCATIONS_HH
