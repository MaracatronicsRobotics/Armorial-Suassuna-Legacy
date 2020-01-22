#ifndef SSLSTRATEGY_HALT_H
#define SSLSTRATEGY_HALT_H

#include <entity/contromodule/strategy/strategystate.h>
#include <entity/contromodule/playbook/mrcplaybook.h>

class SSLStrategy_Halt : public StrategyState {
private:
    // Playbooks
    Playbook_DoNothing *_pb_doNothing;
    void configure(int numOurPlayers);
    void run(int numOurPlayers);
public:
    SSLStrategy_Halt();
    QString name();
};


#endif // SSLSTRATEGY_HALT_H
