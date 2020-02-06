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

    _bh_pgk = new Behaviour_Penalty_GK();
    _bh_atk = new Behaviour_Attacker();
    usesBehaviour(_bh_pgk);
    usesBehaviour(_bh_atk);

    for(int i=0; i<numPlayers; i++) {
        Behaviour_DoNothing *bh_doNothing = new Behaviour_DoNothing();
        usesBehaviour(bh_doNothing);
        _bh_doNothing.push_back(bh_doNothing);
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    QList<quint8> allPlayers = dist()->getAllPlayers();
    setPlayerBehaviour(allPlayers.at(0), _bh_pgk);
    setPlayerBehaviour(allPlayers.at(1), _bh_atk);

    //setPlayerBehaviour(allPlayers.at(2), _bh_mkp);
    /*
    for(int i=0; i<numPlayers; i++)
        setPlayerBehaviour(dist()->getPlayer(), _bh_doNothing.at(i));
        */
}
