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

    _bh_atk = new Behaviour_Attacker();
    _bh_gk = new Behaviour_Goalkeeper();
    _bh_rcv = new Behaviour_Receiver();
    usesBehaviour(_bh_atk);
    usesBehaviour(_bh_gk);
    usesBehaviour(_bh_rcv);

    for(int i=0; i<numPlayers; i++) {
        Behaviour_DoNothing *bh_doNothing = new Behaviour_DoNothing();
        usesBehaviour(bh_doNothing);
        _bh_doNothing.push_back(bh_doNothing);
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    QList<quint8> allPlayers = dist()->getAllPlayers();
    setPlayerBehaviour(allPlayers.at(0), _bh_atk);
    setPlayerBehaviour(allPlayers.at(1), _bh_gk);

    setPlayerBehaviour(allPlayers.at(2), _bh_rcv);
    // setting attacker
    _bh_rcv->setAttackerId(allPlayers.at(0));
    // setting receivers
    _bh_atk->clearReceivers();
    _bh_atk->addReceiver(allPlayers.at(2));
    /*
    for(int i=0; i<numPlayers; i++)
        setPlayerBehaviour(dist()->getPlayer(), _bh_doNothing.at(i));
        */
}
