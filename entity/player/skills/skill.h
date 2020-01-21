#ifndef SKILL_H
#define SKILL_H

#include <entity/player/baseplayer.h>

class Skill
{
public:
    Skill();
    virtual ~Skill();

    // Called one time before run is first called
    bool isInitialized() const { return _initialized; }
    void initialize(Locations *loc);
    void setPlayer(Player *player);
    // Called every time before run is called; update general variables
    void runSkill();
    virtual QString name() = 0;

protected:
    Player *player() { return _player; }
    Locations *loc() { return _loc; }

private:
    virtual void run() = 0;

    // Player access
    Player *_player;

    // Game info
    Locations *_loc;

    // Skill initialized
    bool _initialized;
};

#endif // SKILL_H
