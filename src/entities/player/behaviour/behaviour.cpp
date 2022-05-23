/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "behaviour.h"

Behaviour::Behaviour() {
    _skill = NULL;
    _configureEnabled = false;
    _player = NULL;
    _playerAccess = NULL;
    _loc = NULL;
    _ref = NULL;
    _initialized = false;
}

Behaviour::~Behaviour() {
    // Delete skills transitions
    QList<SkillTransition*> transitions = _transitionTable.values();
    QList<SkillTransition*>::iterator it;
    for(it=transitions.begin(); it!=transitions.end(); it++){
            delete *it;
    }
    _transitionTable.clear();
    // Delete skills
    QList<Skill*>::iterator it2;
    for(it2=_skillList.begin(); it2!=_skillList.end(); it2++){
            delete *it2;
    }
    _skillList.clear();
}

void Behaviour::initialize(Locations *loc, SSLReferee *ref, Constants *constants) {
    _loc = loc;
    _ref = ref;
    _constants=constants;
    // Configurate skills to be used
    _configureEnabled = true;
    configure();
    _configureEnabled = false;
    _initialized = true;
}

void Behaviour::setPlayer(Player *player, PlayerAccess *playerAccess) {
    _player = player;
    _playerAccess = playerAccess;
}

void Behaviour::runBehaviour() {
    // Check if behavior has at least one skill set
    if(_skillList.size()==0) {
        std::cout << Text::red("[ERROR] ") << name().toStdString() << " has no skills set!\n";
        return;
    }
    // Run behavior (implemented by child)
    run();
    // Run skill (implemented by skill)
    if(_skill->isInitialized()==false)
        _skill->initialize(_loc, getConstants());
    _skill->setPlayer(_player);
    _skill->runSkill();
}

void Behaviour::usesSkill(Skill *skill) {
    if(_configureEnabled==false) {
        std::cout << Text::yellow("[WARNING] ") << "Blocked '" << name().toStdString() << "' setting Skill to use outside configure().\n";
        return;
    }
    if(_skillList.contains(skill)==false) {
        // Add to list
        _skillList.push_back(skill);
        // If setting first skill, use as initial state (default)
        if(_skill==NULL)
            _skill = skill;
   }
}

void Behaviour::setInitialSkill(Skill *skill) {
    // Set initial skill
    if(_skillList.contains(skill))
        _skill = skill;
    else
        std::cout << Text::red("[ERROR] ") << name().toStdString() << " setting initial skill '" << skill->name().toStdString() << "' that isn't at skill list!\n";
}

void Behaviour::addTransition(int id, Skill *source, Skill *target) {
    // Check if isn't a invalid transition (same source for same id)
    if(_transitionTable.contains(id)) {
        QList<SkillTransition*> transitions = _transitionTable.values(id);
        QList<SkillTransition*>::const_iterator it;
        for(it=transitions.constBegin(); it!=transitions.constEnd(); it++) {
            const SkillTransition *transition = *it;
            if(transition->source()==source) {
                std::cout << Text::red("[ERROR] ") << name().toStdString() << " adding invalid transition from " << source->name().toStdString() << " to " << target->name().toStdString() << "; source already has a transition with id " << id << "!\n";
                return;
            }
        }
    }
    // Add transition to transitionTable
    SkillTransition *tmpTransition = new SkillTransition(source, target);
    _transitionTable.insertMulti(id, tmpTransition);
}

void Behaviour::enableTransition(int id) {
    // Check if transition exists
    if(_transitionTable.contains(id)==false) {
        std::cout << Text::red("[ERROR] ") << name().toStdString() << ", enabling transition id '" << id << "' not found!\n";
        return;
    }
    // Get transitions in transition table
    QList<SkillTransition*> transitions = _transitionTable.values(id);
    // For each transition, add transition and emit signal
    QList<SkillTransition*>::const_iterator it;
    for(it=transitions.constBegin(); it!=transitions.constEnd(); it++) {
        const SkillTransition *transition = *it;
        // Check if curr state is source state
        if(_skill!=transition->source())
            continue;
        // Do transition
        _skill = transition->target();
        break;
    }
}

PlayerAccess* Behaviour::player() {
    if(_playerAccess==NULL)
        std::cout << Text::red("[ERROR] ") << name().toStdString() << ", requesting player(), playerAccess not set!\n";
    return _playerAccess;
}

Locations* Behaviour::loc() {
    if(_loc==NULL)
        std::cout << Text::red("[ERROR] ") << name().toStdString() << ", requesting loc(), loc not initialized!\n";
    return _loc;
}
Constants *Behaviour::getConstants() {
    if(_constants==NULL)
        std::cout << Text::red("[ERROR] ") << name().toStdString() << ", requesting getConstants(), _mrcconstants not initialized!\n";
    return _constants;
}

SSLReferee* Behaviour::ref() {
    if(_ref==NULL)
        std::cout << Text::red("[ERROR] ") << name().toStdString() << ", requesting ref(), ref not initialized!\n";
    return _ref;
}
