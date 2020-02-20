#include "mrcstrategy.h"
#include <entity/contromodule/strategy/mrcstrategies.h>

QString MRCStrategy::name() {
    return "MRCStrategy_2020";
}

MRCStrategy::MRCStrategy() {

}

void MRCStrategy::configure() {
    setStrategyState(HALT, new SSLStrategy_Halt());
    setStrategyState(GAMEOFF, new SSLStrategy_Halt());
    setStrategyState(GAMEON, new SSLStrategy_Halt());
    setStrategyState(OURPENALTY, new SSLStrategy_Halt());
    setStrategyState(THEIRPENALTY, new SSLStrategy_Halt());
    setStrategyState(OURINDIRECTKICK, new SSLStrategy_Halt());
    setStrategyState(OURDIRECTKICK, new SSLStrategy_Halt());
    setStrategyState(OURKICKOFF, new SSLStrategy_Halt());
    setStrategyState(THEIRKICKOFF, new SSLStrategy_Halt());
    setStrategyState(THEIRDIRECTKICK, new SSLStrategy_Halt());
    setStrategyState(THEIRINDIRECTKICK, new SSLStrategy_Halt());
    setStrategyState(TIMEOUT, new SSLStrategy_Halt());
    setStrategyState(UNDEFINED, new SSLStrategy_Halt());
}
