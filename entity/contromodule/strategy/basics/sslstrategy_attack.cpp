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
    usesPlaybook(_pb_attack = new Playbook_DoNothing());
}

void SSLStrategy_Attack::run(int numOurPlayers) {
    QList<quint8> allPlayers = dist()->getAllPlayers();
    if(!allPlayers.isEmpty())
        _pb_attack->addPlayers(allPlayers);
}
