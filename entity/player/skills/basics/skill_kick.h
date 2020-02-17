#ifndef SKILL_KICK_H
#define SKILL_KICK_H

#include <entity/player/skills/skill.h>

class Skill_Kick : public Skill {
private:
    void run();
    bool _isPass;
    Position _aimPosition;

    enum{
        STATE_POS,
        STATE_KICK
    };

    int _state;
    float _zPower;
    bool isBehindBall(Position posObjective);
    bool isBallInFront();

public:
    Skill_Kick();
    QString name();

    void setIsPass(bool isPass) { _isPass = isPass; }
    void setAim (Position pos) { _aimPosition = pos; }
    void setZPower(float power) { _zPower = power; }
};

#endif // SKILL_KICK_H
