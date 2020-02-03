#include "sslstrategy_attack.h"
#include <entity/contromodule/strategy/strategystate.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

QString SSLStrategy_Attack::name() {
    return "SSLStrategy_Attack";
}

SSLStrategy_Attack::SSLStrategy_Attack() {
    _pb_attack = NULL;
}

void SSLStrategy_Attack::configure(int numOurPlayers) {
    usesPlaybook(_pb_attack = new Playbook_Attack());
}

void SSLStrategy_Attack::run(int numOurPlayers) {
    quint8 attackerId = dist()->getKNN(1, loc()->ball()).at(0);
    if(attackerId != DIST_INVALID_ID){
        _pb_attack->setAttacker(attackerId);
        _pb_attack->addPlayer(attackerId); // set kicker
        _pb_attack->clearReceivers();
    }else{
        _pb_attack->setAttacker(DIST_INVALID_ID);
    }

    QList<quint8> receiversList = dist()->getKNN(2, loc()->ball());
    for(int x = 0; x < receiversList.size(); x++){
        if(receiversList.at(x) == DIST_INVALID_ID) continue;
        else{
            _pb_attack->addReceiver(receiversList[x]);
            _pb_attack->addPlayer(receiversList[x]);
        }
    }

}
