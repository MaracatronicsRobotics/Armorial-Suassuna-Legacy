#ifndef SSLSTRATEGY_TIMEOUT_H
#define SSLSTRATEGY_TIMEOUT_H

#include <entity/contromodule/strategy/strategystate.h>
#include <entity/contromodule/playbook/mrcplaybook.h>

class SSLStrategy_TimeOut : public StrategyState {
private:
    // Playbooks
    Playbook_TimeOut *_pb_timeOut;
    void configure(int numOurPlayers);
    void run(int numOurPlayers);
public:
    SSLStrategy_TimeOut();
    QString name();
};

#endif // SSLSTRATEGY_TIMEOUT_H
