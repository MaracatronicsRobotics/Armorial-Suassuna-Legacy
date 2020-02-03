#ifndef BEHAVIOUR_ATTACKER_H
#define BEHAVIOUR_ATTACKER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>
#include <utils/mrctimer/mrctimer.h>

class Behaviour_Attacker : public Behaviour {
    Q_OBJECT
private:
    void configure();
    void run();
    int _state;

    QList<quint8> _recvs;
    MRCTimer *_timer;

    quint8 _bestReceiver;
    Position _kickPosition;

public:
    Behaviour_Attacker();
    QString name();

    Skill_Kick *_sk_kick;
    Skill_GoToLookTo *_sk_goto;

    enum{
        STATE_ATTACK,
        STATE_WAIT,
        STATE_KICK
    };

    void addReceiver(quint8 recvId) { _recvs.push_back(recvId); }
    void clearReceivers() { _recvs.clear(); }
    quint8 getBestReceiver();

    bool isBehindBall(Position posObjective);

signals:
    void goingToShoot(quint8 id);
    void shooted(quint8 id);
};

#endif // BEHAVIOUR_ATTACKER_H
