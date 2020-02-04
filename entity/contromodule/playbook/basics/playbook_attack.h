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

    quint8 _attackerId;
    QList <quint8> _receiversList;
    QMutex _mutex;

public:
    Playbook_Attack();
    QString name();

    void setAttacker(quint8 attackerId) { _attackerId = attackerId; }
    void addReceiver(quint8 receiverId) { _receiversList.push_back(receiverId); }
    void clearReceivers() { _receiversList.clear(); }
};

#endif // PLAYBOOK_ATTACK_H
