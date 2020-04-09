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

#ifndef SUASSUNA_H
#define SUASSUNA_H

#include <QHostAddress>
#include <GEARSystem/gearsystem.hh>

#include <utils/basics/color.hh>
#include <utils/basics/competition.hh>

#include <entity/referee/SSLReferee/sslreferee.h>

#include <utils/fieldside/fieldside.hh>
#include <utils/fields/wrfields.hh>

#include <entity/world/world.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/player/sslplayer/sslplayer.h>
#include <entity/contromodule/coach.h>
#include <entity/coachview/coachview.h>

#define MAX_ROBOT_ID 6

class Suassuna
{
public:
    Suassuna(quint8 teamId, Colors::Color teamColor, FieldSide fieldSide);
    ~Suassuna();

    // Start
    bool start();
    void stop();

    // Configuration
    void setServerAddress(QString ipAddress, int port);
private:
    // Internal methods
    bool connectToServer();
    void setupTeams(quint8 opTeamId, Colors::Color opTeamColor, FieldSide opFieldSide);
    void setupOurPlayers();
    void setupOppPlayers(quint8 opTeamId);

    // Server connection
    QString _serverAddress;
    int _serverPort;

    // Internal
    const quint8 _teamId;
    Controller *_ctr;
    const Colors::Color _teamColor;
    const FieldSide _fieldSide;
    Fields::Field* _defaultField;

    // Modules
    World *_world;
    SSLReferee *_ref;
    Coach *_coach;

    // Times
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;

    // GUI
    CoachView *_ourGUI;
};
#endif // SUASSUNA_H
