#include "skill.h"
#include <entity/locations.h>

Skill::Skill() {
    _player = NULL;
    _loc = NULL;
    _initialized = false;
}

Skill::~Skill(){

}

void Skill::initialize(Locations *loc){
    _loc = loc;
    _initialized = true;
}

void Skill::setPlayer(Player *player){
    _player = player;
}

void Skill::runSkill(){
    // Skill implemented by child of this class
    run();
}
