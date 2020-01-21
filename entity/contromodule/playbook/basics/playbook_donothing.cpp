#include "playbook_donothing.h"

QString Playbook_DoNothing::name() {
    return "Playbook_DoNothing";
}

Playbook_DoNothing::Playbook_DoNothing() {
}

int Playbook_DoNothing::maxNumPlayer() {
    return INT_MAX;
}

void Playbook_DoNothing::configure(int numPlayers) {
    _bh_doNothing.clear();
    for(int i=0; i<numPlayers; i++) {
        Behaviour_DoNothing *bh_doNothing = new Behaviour_DoNothing();
        usesBehaviour(bh_doNothing);
        _bh_doNothing.push_back(bh_doNothing);
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    for(int i=0; i<numPlayers; i++)
        setPlayerBehaviour(dist()->getPlayer(), _bh_doNothing.at(i));
}
