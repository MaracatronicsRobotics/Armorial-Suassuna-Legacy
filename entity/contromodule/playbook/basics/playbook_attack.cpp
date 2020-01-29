#include "playbook_attack.h"
#define INVALID_ID 200

QString Playbook_Attack::name() {
    return "Playbook_Attack";
}

Playbook_Attack::Playbook_Attack() {
    _attackerId = INVALID_ID;
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
    if(_attackerId != INVALID_ID){
        _mutex.lock();
        setPlayerBehaviour(_attackerId, _bh_atk);
        _bh_atk->clearReceivers();
        _mutex.unlock();
    }

    for(int i=0; i<_receiversList.size(); i++){
        _mutex.lock();
        _bh_rcv.at(i)->setFollowAttacker(true);
        _bh_rcv.at(i)->setAttackerId(_attackerId);


        quint8 player = _receiversList.at(i);
        _bh_atk->addReceiver(player);
        setPlayerBehaviour(player, _bh_rcv.at(i));
        _mutex.unlock();

        // conecta as funções
        connect(_bh_atk, SIGNAL(goingToShoot(quint8)), _bh_rcv.at(i), SLOT(goingToReceive(quint8)), Qt::DirectConnection);
        connect(_bh_atk, SIGNAL(shooted(quint8)), _bh_rcv.at(i), SLOT(attackerShooted(quint8)), Qt::DirectConnection);
    }
}
