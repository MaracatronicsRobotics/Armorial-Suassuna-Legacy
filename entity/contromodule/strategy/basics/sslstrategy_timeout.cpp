#include "sslstrategy_timeout.h"
#include <entity/contromodule/strategy/strategystate.h>
#pragma GCC diagnostic ignored "-Wunused-parameter"

QString SSLStrategy_TimeOut::name() {
    return "SSLStrategy_TimeOut";
}

SSLStrategy_TimeOut::SSLStrategy_TimeOut() {
    _pb_timeOut = NULL;
}

void SSLStrategy_TimeOut::configure(int numOurPlayers) {
    usesPlaybook(_pb_timeOut = new Playbook_TimeOut());
}

void SSLStrategy_TimeOut::run(int numOurPlayers) {
    _pb_timeOut->addPlayers(dist()->getAllPlayers());
}
