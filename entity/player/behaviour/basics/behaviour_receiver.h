#ifndef BEHAVIOUR_RECEIVER_H
#define BEHAVIOUR_RECEIVER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Receiver : public Behaviour {
private:
    void configure();
    void run();
    Skill_GoToLookTo *_skill_GoToLookTo;
    bool _followAttacker;
    int _attackerId;
    int _state;

    enum {
        STATE_FOLLOW,
        STATE_RECEIVE,
        STATE_STOP
    };

public:
    Behaviour_Receiver();
    QString name();

    void setFollowAttacker(bool followAttacker) { _followAttacker = followAttacker; }
    void setAttackerId (quint8 attackerId) { _attackerId = attackerId; }
};

#endif // BEHAVIOUR_RECEIVER_H
