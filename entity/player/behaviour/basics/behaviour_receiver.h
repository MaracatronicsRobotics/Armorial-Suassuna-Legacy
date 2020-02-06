#ifndef BEHAVIOUR_RECEIVER_H
#define BEHAVIOUR_RECEIVER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Receiver : public Behaviour {
    Q_OBJECT
private:
    void configure();
    void run();
    Skill_GoToLookTo *_skill_GoToLookTo;
    Skill_GoalKeeper *_skill_Receiver;
    bool _followAttacker;
    int _attackerId;
    int _state;

    enum {
        STATE_FOLLOW,
        STATE_RECEIVE,
        STATE_STOP
    };

    enum{
        SK_GOTO,
        SK_RECV
    };

public:
    Behaviour_Receiver();
    QString name();

    void setFollowAttacker(bool followAttacker) { _followAttacker = followAttacker; }
    void setAttackerId (quint8 attackerId) { _attackerId = attackerId; }

public slots:
    void goingToReceive(quint8 id);
    void attackerShooted(quint8 id);
};

#endif // BEHAVIOUR_RECEIVER_H
