#ifndef PLAYBOOK_H
#define PLAYBOOK_H

#include <climits>
#include <entity/referee/SSLReferee/sslgameinfo.h>
#include <entity/referee/SSLReferee/sslreferee.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/contromodule/playersdistribution.h>
#include <entity/player/behaviour/behaviour.h>

class Playbook : public QObject {
    Q_OBJECT
public:
    Playbook();
    virtual ~Playbook();

    // Called one time before run is first called
    bool isInitialized() { return _initialized; }
    void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, CoachUtils *utils, qint8 *kickerId, SSLGameInfo::RefProcessedState *lastState, SSLReferee *ref);

    // Called in StrategyState loop
    void runPlaybook(QString strategyState);
    void clearOldBehaviours();

    // Players manipulation
    void addPlayer(quint8 id);
    void addPlayers(const QList<quint8> &ids);
    void clearPlayers();
    int numPlayers();
    virtual QString name() = 0;
protected:
    // Players on this playbook access
    bool hasPlayer(quint8 id);
    void setPlayerBehaviour(quint8 id, Behaviour *behaviour);

    // Behaviour functions
    void usesBehaviour(Behaviour *behaviour);

    // canKickBall access (normal start)
    bool canKickBall() const;

    // Utils, loc and dist access
    CoachUtils* utils() { return _utils; }
    Locations* loc() const;
    PlayersDistribution* dist() { return _dist; }
private:
    // Implemented by children
    virtual void run(int numPlayers) = 0;
    virtual void configure(int numPlayers) = 0;
    virtual int maxNumPlayer() = 0;

    // Configure info
    bool _configureEnabled;
    int _lastNumPlayers;
    void updatePlayersBehaviours();

    // Behaviours garbage collector
    QList<Behaviour*> _oldBehaviours;
    void setCurrBehavioursToOld();

    // Game info
    MRCTeam *_ourTeam;
    MRCTeam *_opTeam;
    CoachUtils *_utils;
    SSLReferee *_ref;

    // Players in this playbook list
    PlayersDistribution *_dist;
    QList<quint8> _players;
    QHash<quint8,Behaviour*> _assignmentTable;
    QList<Behaviour*> _behavioursList;

    // Playbook initialized
    bool _initialized;
};

#endif // PLAYBOOK_H
