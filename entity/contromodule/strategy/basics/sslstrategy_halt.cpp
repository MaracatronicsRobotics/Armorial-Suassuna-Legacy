#include "sslstrategy_halt.h"
#include <entity/contromodule/strategy/strategystate.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

QString SSLStrategy_Halt::name() {
    return "SSLStrategy_WR2015_Halt";
}

SSLStrategy_Halt::SSLStrategy_Halt() {
    _pb_doNothing = NULL;
}

void SSLStrategy_Halt::configure(int numOurPlayers) {
    usesPlaybook(_pb_doNothing = new Playbook_DoNothing());
}

void SSLStrategy_Halt::run(int numOurPlayers) {
    _pb_doNothing->addPlayer(0);
    //_pb_doNothing->addPlayers(dist()->getAllPlayers());
}
