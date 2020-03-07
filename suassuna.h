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

    // GRSIM Simulator for test
    grsSimulator *_grSimulator;

    // Times
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;

    // GUI
    CoachView *_ourGUI;
};
#endif // SUASSUNA_H
