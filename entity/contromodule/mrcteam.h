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

#ifndef MRCTEAM_H
#define MRCTEAM_H

#include <utils/fieldside/fieldside.hh>
#include <entity/player/player.h>

#define BALLPOSS_NONE 100

class MRCTeam
{
public:
    MRCTeam(quint8 teamId, WorldMap *wm);
    virtual ~MRCTeam();

    // Constant infos access
    quint8 teamId() const { return _teamId; }

    // Infos getters
    Colors::Color teamColor() const;
    FieldSide fieldSide() const;

    // Infos setters
    void setTeamColor(Colors::Color color);
    void setFieldSide(FieldSide fieldSide);

    // Opponent team info
    void setOpponentTeam(MRCTeam *team) { _opTeam = team; }
    MRCTeam *opTeam() const { return _opTeam; }

    // Ball possession
    bool hasBallPossession();
    quint8 ballPossession();

    // Add players
    void addPlayer(Player *player);

    // Available players access
    int avPlayersSize();
    QHash<quint8, Player*> avPlayers();
    void updateAvailablePlayers();

    // WorldMap access
    WorldMap *wm() { return _wm; }
    Locations* loc() { return _loc; }
    Position ballPosition();

private:
    // Team id
    quint8 _teamId;

    // Team color
    Colors::Color _teamColor;
    mutable QMutex _mutexTeamColor;

    // Team field side
    FieldSide _fieldSide;
    mutable QMutex _mutexFieldSide;

    // Opp team access
    MRCTeam *_opTeam;

    // Players
    QMutex _muxPlayers;
    QHash<quint8, Player*> _players;
    QHash<quint8, Player*> _avPlayers;

    // Locations
    Locations *_loc;

    // Worldmap
    WorldMap *_wm;
};

#endif // MRCTEAM_H
