#ifndef PLAYBOOK_DONOTHING_H
#define PLAYBOOK_DONOTHING_H

#include <entity/contromodule/playbook/playbook.h>
#include <entity/player/behaviour/mrcbehaviours.h>

class Playbook_DoNothing : public Playbook {
private:
    // Behaviors
    QList<Behaviour_DoNothing*> _bh_doNothing;
    Behaviour_GoalKeeper *_bh_gk;
    Behaviour_Barrier *_bh_bar;
    Behaviour_Barrier *_bh_bar2;
    void configure(int numPlayers);
    void run(int numPlayers);
    int maxNumPlayer();
public:
    Playbook_DoNothing();
    QString name();
};

#endif // PLAYBOOK_DONOTHING_H
