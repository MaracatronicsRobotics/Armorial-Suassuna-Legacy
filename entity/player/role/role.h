#ifndef ROLE_H
#define ROLE_H

#include <entity/player/baseplayer.h>
#include <entity/player/playeraccess.h>
#include <entity/player/playerbus.h>
#include <entity/referee/SSLReferee/sslgameinfo.h>
#include <entity/referee/SSLReferee/sslreferee.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/contromodule/playersdistribution.h>
#include <entity/player/behaviour/behaviour.h>
#include <QObject>

class Role : public QObject {
        Q_OBJECT
public:
    Role();
    virtual ~Role();

    // Initialization
    bool isInitialized() { return _initialized; }
    void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, Locations *loc, SSLReferee *ref);
    void setPlayer(Player *player, PlayerAccess *playerAccess);

    // Called in Playbook loop
    void runRole();

    // Initialize
    virtual void initializeBehaviours() = 0;

    // Public functions
    void setBehaviour(int behaviour_id);
    QHash<int, Behaviour*> getBehaviours();
    virtual QString name() = 0;
    int getActualBehaviour();

protected:
    // Behaviour list functions
    void usesBehaviour(int id, Behaviour *behaviour);

    // canKickBall (for parameters)
    bool canKickBall() const;

    // Utils, loc and player access
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
    QHash<int, Behaviour*> _behaviourList;
    Behaviour *_behaviour;
    int _actualBehaviour;

};

#endif // ROLE_H
