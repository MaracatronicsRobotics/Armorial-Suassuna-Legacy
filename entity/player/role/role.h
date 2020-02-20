#ifndef ROLE_H
#define ROLE_H

#include <entity/referee/SSLReferee/sslgameinfo.h>
#include <entity/referee/SSLReferee/sslreferee.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/contromodule/playersdistribution.h>
#include <entity/player/behaviour/behaviour.h>

class Role : public QObject {
        Q_OBJECT
public:
    Role();
    virtual ~Role();

    // Initialization
    bool isInitialized() { return _initialized; }
    void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, Locations *loc, CoachUtils *utils, SSLReferee *ref);
    void setPlayer(Player *player, PlayerAccess *playerAccess);

    // Called in Playbook loop
    void runRole();
    virtual QString name() = 0;

protected:
    // Behaviour list functions
    void usesBehaviour(Behaviour *behaviour);
    void setBehaviour(Behaviour *behaviour);

    // canKickBall (for parameters)
    bool canKickBall() const;

    // Utils, loc and player access
    CoachUtils* utils() { return _utils; }
    PlayerAccess* player();
    Locations* loc();

private:
    // Implemented by role children
    virtual void run() = 0;
    virtual void configure() = 0;
    bool _initialized;
    bool _configureEnabled;

    // Player access
    Player *_player;
    PlayerAccess *_playerAccess;

    // Game Info
    Locations *_loc;
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;
    CoachUtils *_utils;
    SSLReferee *_ref;

    // Behaviours list
    QList<Behaviour*> _behaviourList;
    Behaviour *_behaviour;

};

#endif // ROLE_H
