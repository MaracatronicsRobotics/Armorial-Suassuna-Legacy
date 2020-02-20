#ifndef BEHAVIOUR_RECEIVER_H
#define BEHAVIOUR_RECEIVER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>
#include <utils/freeangles/freeangles.h>
#include <QObject>

class Behaviour_Receiver : public Behaviour {
    Q_OBJECT
private:
    void configure();
    void run();

    // Skills
    Skill_GoToLookTo *_skill_GoToLookTo;
    Skill_InterceptBall *_skill_Receiver;

    // Parameters
    bool _followAttacker;
    int _quadrant;
    int _attackerId;
    int _state;
    float _minRadius;
    float _maxRadius;

    // Transitions and states enums
    enum {
        STATE_POSITION,
        STATE_RECEIVE,
        STATE_WAIT
    };

    enum{
        SK_GOTO,
        SK_RECV
    };

    enum{
        QUADRANT_UP,
        QUADRANT_UPMID,
        QUADRANT_BOT,
        QUADRANT_BOTMID,
        NO_QUADRANT
    };

    // Auxiliary functions
    Position getReceiverBestPosition(int quadrant, quint8 attackerId, float minRadius, float maxRadius);
    std::pair<Position, Position> getQuadrantInitialPosition(int quadrant);

public:
    Behaviour_Receiver();
    QString name();

    // setters
    void setFollowAttacker(bool followAttacker) { _followAttacker = followAttacker; }
    void setAttackerId (quint8 attackerId) { _attackerId = attackerId; }
    void setQuadrant(int quadrant) { _quadrant = quadrant; }
    void setActionRadius(float minRadius, float maxRadius) { _minRadius = minRadius; _maxRadius = maxRadius;}

    // getters
    quint8 getAttackerId(){ return _attackerId; }
    int getQuadrant() { return _quadrant; }
    float getMinRadius() { return _minRadius; }
    float getMaxRadius() { return _maxRadius; }

public slots:
    void goingToReceive(quint8 id);
    void attackerShooted(quint8 id);
};

#endif // BEHAVIOUR_RECEIVER_H
