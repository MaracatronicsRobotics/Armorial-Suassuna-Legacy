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

    /*
    _bh_gk = new Behaviour_GoalKeeper();
    _bh_bar = new Behaviour_Barrier();
    _bh_bar2 = new Behaviour_Barrier();
    usesBehaviour(_bh_gk);
    usesBehaviour(_bh_bar);
    usesBehaviour(_bh_bar2);
    */

    for(int i=0; i<numPlayers; i++) {
        Behaviour_DoNothing *bh_doNothing = new Behaviour_DoNothing();
        usesBehaviour(bh_doNothing);
        _bh_doNothing.push_back(bh_doNothing);
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    /*
    QList<quint8> allPlayers = dist()->getAllPlayers();
    setPlayerBehaviour(allPlayers.at(0), _bh_gk);
    if(allPlayers.size() == 2){
        setPlayerBehaviour(allPlayers.at(1), _bh_bar);
    }else if(allPlayers.size() == 3){
        setPlayerBehaviour(allPlayers.at(1), _bh_bar);
        setPlayerBehaviour(allPlayers.at(2), _bh_bar2);
    }
    //setPlayerBehaviour(dist()->getKNN(1, loc()->ourGoal()).at(0), _bh_gk);
    */

    for(int i=0; i<numPlayers; i++)
        setPlayerBehaviour(dist()->getPlayer(), _bh_doNothing.at(i));

}
