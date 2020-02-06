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
    QList<quint8> allPlayers = dist()->getAllPlayers();
    quint8 attackerId = allPlayers.at(0);
    if(attackerId != DIST_INVALID_ID){
        _pb_attack->setAttacker(attackerId);
        _pb_attack->addPlayer(attackerId); // set kicker
        _pb_attack->clearReceivers();
    }else{
        _pb_attack->setAttacker(DIST_INVALID_ID);
    }


    if(allPlayers.size() > 1){
        for(int x = 0; x < allPlayers.size() - 1; x++){
            _pb_attack->addReceiver(allPlayers.at(x+1));
            _pb_attack->addPlayer(allPlayers.at(x+1));
        }
    }

}
