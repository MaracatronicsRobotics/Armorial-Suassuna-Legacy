#ifndef PLAYBOOK_ATTACK_H
#define PLAYBOOK_ATTACK_H

#include <entity/contromodule/playbook/playbook.h>
#include <entity/player/behaviour/mrcbehaviours.h>

class Playbook_Attack : public Playbook {
private:
    // Behaviors
    Behaviour_Attacker *_bh_atk;
    QList<Behaviour_Receiver*> _bh_rcv;
    void configure(int numPlayers);
    void run(int numPlayers);
    int maxNumPlayer();

    int _attackerId;
public:
    Playbook_Attack();
    QString name();

    void setAttacker(int attackerId) { _attackerId = attackerId; }
};

#endif // PLAYBOOK_ATTACK_H
