#ifndef PLAYBOOK_DONOTHING_H
#define PLAYBOOK_DONOTHING_H

#include <entity/contromodule/playbook/playbook.h>
#include <entity/player/behaviour/mrcbehaviours.h>

class Playbook_DoNothing : public Playbook {
private:
    // Behaviors
    Behaviour_Attacker *_bh_atk;
    Behaviour_Goalkeeper *_bh_gk;
    Behaviour_Receiver *_bh_rcv;
    QList<Behaviour_DoNothing*> _bh_doNothing;

    void configure(int numPlayers);
    void run(int numPlayers);
    int maxNumPlayer();
public:
    Playbook_DoNothing();
    QString name();
};

#endif // PLAYBOOK_DONOTHING_H
