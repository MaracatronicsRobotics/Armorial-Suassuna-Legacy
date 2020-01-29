#include "playbook_attack.h"

QString Playbook_Attack::name() {
    return "Playbook_Attack";
}

Playbook_Attack::Playbook_Attack() {
    _attackerId = -1;
}

int Playbook_Attack::maxNumPlayer() {
    return 3;
}

void Playbook_Attack::configure(int numPlayers) {
    _bh_rcv.clear();

    for(int i=0; i<numPlayers - 1; i++) {
        Behaviour_Receiver *bh_rcv = new Behaviour_Receiver();
        usesBehaviour(bh_rcv);
        _bh_rcv.push_back(bh_rcv);
    }

    _bh_atk = new Behaviour_Attacker();
    usesBehaviour(_bh_atk);


}

void Playbook_Attack::run(int numPlayers) {
    if(_attackerId != -1){
        setPlayerBehaviour(_attackerId, _bh_atk);
        _bh_atk->clearReceivers();
    }

    for(int i=0; i<numPlayers-1; i++){
        _bh_rcv.at(i)->setFollowAttacker(true);
        _bh_rcv.at(i)->setAttackerId(_attackerId);

        double player = dist()->getPlayer();
        _bh_atk->addReceiver(player);
        setPlayerBehaviour(player, _bh_rcv.at(i));
    }
}
