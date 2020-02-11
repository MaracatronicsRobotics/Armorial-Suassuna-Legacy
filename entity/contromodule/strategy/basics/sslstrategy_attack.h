#ifndef SSLSTRATEGY_ATTACK_H
#define SSLSTRATEGY_ATTACK_H

#include <entity/contromodule/strategy/strategystate.h>
#include <entity/contromodule/playbook/mrcplaybook.h>

class SSLStrategy_Attack : public StrategyState {
private:
    // Playbooks
    Playbook_DoNothing *_pb_attack;
    void configure(int numOurPlayers);
    void run(int numOurPlayers);
public:
    SSLStrategy_Attack();
    QString name();
};

#endif // SSLSTRATEGY_ATTACK_H
