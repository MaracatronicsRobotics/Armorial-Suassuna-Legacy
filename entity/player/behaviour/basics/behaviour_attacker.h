#ifndef BEHAVIOUR_ATTACKER_H
#define BEHAVIOUR_ATTACKER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Attacker : public Behaviour {
private:
    void configure();
    void run();
    int _state;

    QList<quint8> _recvs;

public:
    Behaviour_Attacker();
    QString name();

    Skill_Kick *_teste;
    Skill_GoToLookTo *_sk_goto;

    enum{
        STATE_GOTO,
        STATE_KICK
    };

    void addReceiver(quint8 recvId) { _recvs.push_back(recvId); }
    void clearReceivers() { _recvs.clear(); }
    quint8 getBestReceiver();

    bool isBehindBall(Position posObjective);

signals:
    void goingToShoot(Position pos);
    void shooted();
};

#endif // BEHAVIOUR_ATTACKER_H
