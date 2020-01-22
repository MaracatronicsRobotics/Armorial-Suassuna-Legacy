#ifndef PLAYBOOK_TIMEOUT_H
#define PLAYBOOK_TIMEOUT_H

#include <entity/contromodule/playbook/playbook.h>
#include <entity/player/behaviour/mrcbehaviours.h>

class Playbook_TimeOut : public Playbook {
private:
    // Behaviors
    QList<Behaviour_TimeOut*> _bh_to;
    void configure(int numPlayers);
    void run(int numPlayers);
    int maxNumPlayer();
public:
    Playbook_TimeOut();
    QString name();
};

#endif // PLAYBOOK_TIMEOUT_H
