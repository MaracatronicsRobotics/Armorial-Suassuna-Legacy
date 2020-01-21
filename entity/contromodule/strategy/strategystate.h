#ifndef STRATEGYSTATE_H
#define STRATEGYSTATE_H

#include <entity/contromodule/basecoach.h>
#include <entity/referee/SSLReferee/sslgameinfo.h>

class StrategyState {
public:
    StrategyState();
    virtual ~StrategyState();

    // Called one time before run is first called
    bool isInitialized() { return _initialized; }
    void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam, CoachUtils *utils, PlayersDistribution *dist, qint8 *kickerId, SSLGameInfo::RefProcessedState *lastState, SSLReferee *ref);

    // Called in Coach -> Strategy loop
    void runStrategyState();
    virtual QString name() = 0;
protected:
    // Playbook functions
    void usesPlaybook(Playbook *playbook);

    // canKickBall access (normal start)
    bool canKickBall() const;

    // Ball possession access
    bool hasBallPossession() const;
    quint8 ballPossession() const;

    // Utils, loc and dist access
    CoachUtils* utils() { return _utils; }
    Locations* loc() const;
    PlayersDistribution* dist() { return _dist; }
private:
    // Implemented by children
    virtual void configure(int numOurPlayers) = 0;
    virtual void run(int numOurPlayers) = 0;
    bool _configureEnabled;

    // Players access; used to give access to playbook
    int _lastNumOurPlayers;
    PlayersDistribution *_dist;
    qint8 *_kickerId;
    SSLGameInfo::RefProcessedState *_lastState;
    SSLReferee *_ref;

    // Game info
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;
    CoachUtils *_utils;

    // Playbook list
    QList<Playbook*> _playbookList;
    void clearOldPlaybook();
    void setCurrPlaybookToOld();
    QList<Playbook*> _oldPlaybook;

    // StrategyState initialized
    bool _initialized;
};

#endif // STRATEGYSTATE_H
