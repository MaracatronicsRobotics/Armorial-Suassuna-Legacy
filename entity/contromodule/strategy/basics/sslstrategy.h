#ifndef SSLSTRATEGY_H
#define SSLSTRATEGY_H

#include <entity/contromodule/strategy/strategy.h>

class SSLStrategy : public Strategy {
public:
    SSLStrategy();
    virtual ~SSLStrategy();
    virtual QString name() = 0;
protected:
    typedef enum {
        HALT,
        GAMEON,
        GAMEOFF,
        OURINDIRECTKICK,
        OURDIRECTKICK,
        OURKICKOFF,
        OURPENALTY,
        THEIRKICKOFF,
        THEIRPENALTY,
        THEIRDIRECTKICK,
        THEIRINDIRECTKICK,
        TIMEOUT,
        UNDEFINED
    } SSLGameState;
    void setStrategyState(SSLGameState gameState, StrategyState *strategyState);
private:
    virtual void configure() = 0;
    void runStrategy();
    QString state2str(int gameState);
    SSLGameState refState2SSLGameState(SSLGameInfo::RefProcessedState refState) const;
};


#endif // SSLSTRATEGY_H
