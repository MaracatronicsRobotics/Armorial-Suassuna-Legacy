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
    for(int i = 0; i < numPlayers; i++) {
        Role_Default *rl_def = new Role_Default();
        rl_def->initializeBehaviours();
        usesRole(rl_def);
        _rl_def.push_back(rl_def);
    }
}

void Playbook_DoNothing::run(int numPlayers) {
    for(int i = 0; i < numPlayers; i++){
        quint8 playerId = dist()->getPlayer();
        if(PlayerBus::ourPlayer(playerId)->position().x() < 0.0){
            _rl_def.at(i)->setBehaviour(_rl_def.at(i)->getBehaviours().at(0));
        }else{
            _rl_def.at(i)->setBehaviour(_rl_def.at(i)->getBehaviours().at(1));
        }
        setPlayerRole(playerId, _rl_def.at(i));
    }
}
