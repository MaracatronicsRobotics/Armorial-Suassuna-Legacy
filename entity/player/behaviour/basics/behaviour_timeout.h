#ifndef BEHAVIOUR_TIMEOUT_H
#define BEHAVIOUR_TIMEOUT_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_TimeOut : public Behaviour {
private:
    void configure();
    void run();
    Position _desiredPosition;
    Skill_GoToLookTo *_skill_GoToLookTo;
<<<<<<< HEAD
    double _offsetBall;
=======
    Skill_AroundTheBall *_skill_AroundTheBall;
    double _distBall;
>>>>>>> createPID
public:
    Behaviour_TimeOut();
    QString name();

    void setSkillPosition(Position pos);
};

#endif // BEHAVIOUR_TIMEOUT_H
