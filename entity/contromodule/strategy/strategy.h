#ifndef STRATEGY_H
#define STRATEGY_H

#include <entity/contromodule/basecoach.h>
#include <entity/referee/SSLReferee/sslgameinfo.h>

class Strategy
{
public:
    Strategy();
    virtual ~Strategy();

    bool isInitialized() { return _initialized; }
    void initialize(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam, CoachUtils *utils);

    // Coach loop
    virtual void runStrategy() = 0;
    virtual QString name() = 0;

protected:
    SSLGameInfo::RefProcessedState getGameState();
    virtual void setStrategyState(int gameState, StrategyState *strategyState);
    void runStrategy(int gameState, SSLGameInfo::RefProcessedState refState);

private:
    virtual QString state2str(int gameState) = 0;
    StrategyState* getStrategyState (int gameState);

    // Children strategies
    virtual void configure() = 0;

    // Referee
    SSLReferee *_ref;

    // Teams
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;

    // Player distribution
    // here goes things like kickerId, ref last state and playerdistribution itself
    qint8 _kickerId;
    SSLGameInfo::RefProcessedState _lastState;
    PlayersDistribution *_dist;
    CoachUtils *_utils;

    // Game states table
    QHash<int, StrategyState*> _strategyStatesTable;

    // Strategy initialized
    bool _initialized;
};

#endif // STRATEGY_H
