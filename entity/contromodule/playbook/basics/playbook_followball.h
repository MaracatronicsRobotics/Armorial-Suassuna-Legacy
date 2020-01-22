#ifndef PLAYBOOK_FOLLOWBALL_H
#define PLAYBOOK_FOLLOWBALL_H

#include <entity/contromodule/playbook/playbook.h>
#include <entity/player/behaviour/mrcbehaviours.h>

class Playbook_FollowBall : public Playbook {
private:
    // Behaviors
    QList<Behaviour_FollowBall*> _bh_followBall;
    void configure(int numPlayers);
    void run(int numPlayers);

    // distance to followball skill
    double _distBall;
public:
    Playbook_FollowBall();
    QString name();
    int maxNumPlayer();

    void setBallDistance(double ballDistance) { _distBall = ballDistance; }
};

#endif // PLAYBOOK_FOLLOWBALL_H
