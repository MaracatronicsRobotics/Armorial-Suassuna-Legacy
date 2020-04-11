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

#include "world.h"
#include <entity/entity.h>
#include <entity/contromodule/mrcteam.h>
#include <entity/contromodule/controlmodule.h>

QString World::name() {
    return (_ctrModule==NULL? "World" : _ctrModule->name());
}

World::World(Controller *ctr, Fields::Field *defaultField) : Entity(ENT_WORLD) {

    _ctr = ctr;
    _ourTeam = _theirTeam = NULL;

    // Create WorldMap
    _wm = new WorldMap();

    setupWorldMap();
    // Create WorldMapUpdater
    _wmUpdater = new WorldMapUpdater(ctr, defaultField);
    _wmUpdater->setDefaultFieldGeometry(_wm);

    // Set self loop time
    this->setLoopFrequency(MRCConstants::threadFrequency());

    // Initialize
    _ctrModule = NULL;
}

World::~World() {
    // Delete WorldMap
    delete _wm;
    // Delete WorldMapUpdater
    delete _wmUpdater;
}

void World::setControlModule(ControlModule *ctrModule) {
    _ctrModuleMutex.lock();
    _ctrModule = ctrModule;
    _ctrModuleMutex.unlock();
}

void World::setTeams(MRCTeam *ourTeam, MRCTeam *theirTeam) {
    _ourTeam = ourTeam;
    _theirTeam = theirTeam;
}

void World::initialization() {
    std::cout << "[WORLD] thread started.\n";
    // Get priorities
    QList<int> priorities = _priorityLevels.keys();
    // In each priority
    for(QList<int>::const_iterator i=priorities.constBegin(); i!=priorities.constEnd(); i++) {
        // Get entities
        if(_priorityLevels.contains(*i) == false)
            continue;
        QHash<int,Entity*> *entities = _priorityLevels.value(*i);
        QList<Entity*> ents = entities->values();
        // Start entity thread
        for(QList<Entity*>::const_iterator ie=ents.constBegin(); ie!=ents.constEnd(); ie++) {
            (*ie)->setLoopFrequency(MRCConstants::threadFrequency());
            (*ie)->start();
        }
    }
}

void World::loop() {
    _ctrModuleMutex.lock();
    // Lock for updating
    this->wmLockWrite();

    // Update world map
    _wmUpdater->update(_wm);

    // Update available players
    _ourTeam->updateAvailablePlayers();
    _theirTeam->updateAvailablePlayers();

    // Unlock
    this->wmUnlock();

    // Run control module
    if(_ctrModule!=NULL)
        _ctrModule->runControlModule();

    _ctrModuleMutex.unlock();
}

void World::finalization() {
    // Stop all entities and delete them
    stopAndDeleteEntities();
    std::cout << "[WORLD] thread ended.\n";
}

void World::stopAndDeleteEntities() {
    // Get priorities
    QList<int> priorities = _priorityLevels.keys();
    // In each priority (decreasing)
    for(int i=priorities.size()-1; i>=0; i--) {
        const int priority = priorities.at(i);
        // Get entities
        QHash<int,Entity*> *entities = _priorityLevels.value(priority);
        QList<Entity*> ents = entities->values();
        // Stop entities
        for(QList<Entity*>::const_iterator it=ents.constBegin(); it!=ents.constEnd(); it++) {
            Entity *entity = *it;
            // Stop and wait
            entity->stopEntity();
            entity->wait();
        }
        // Delete entities
        for(QList<Entity*>::const_iterator it=ents.constBegin(); it!=ents.constEnd(); it++) {
            Entity *entity = *it;
            // Remove entity from hash
            _priorityLevels.value(priority)->remove(entity->entityId());
            // Delete
            delete entity;
        }
        // Delete hash from priority
        delete _priorityLevels.value(priority);
        // Remove priority from list
        _priorityLevels.remove(priority);
    }
}

void World::addEntity(Entity *e, int priority) {
    // Check if Entity is already on world
    QMap<int,QHash<int,Entity*>*>::const_iterator it;
    for(it=_priorityLevels.constBegin(); it!=_priorityLevels.constEnd(); it++) {
        const QHash<int,Entity*> *hash = (*it);
        if(hash->contains(e->entityId()))
            return;
    }
    // Get priority level hash; add if necessary
    QHash<int,Entity*> *priorityLevel;
    if(_priorityLevels.keys().contains(priority)==false) {
        priorityLevel = new QHash<int,Entity*>();
        _priorityLevels.insert(priority, priorityLevel);
    } else
        priorityLevel = _priorityLevels.value(priority);
    // Insert Entity on priority level
    priorityLevel->insert(e->entityId(), e);
    // Set priority
    e->setEntityPriority(priority);
}

void World::removeEntity(int id) {
    // Check if Entity is on world
    QMap<int,QHash<int,Entity*>*>::iterator it;
    for(it=_priorityLevels.begin(); it!=_priorityLevels.end(); it++) {
        QHash<int,Entity*> *hash = (*it);
        if(hash->contains(id)) {
            Entity *entity = hash->value(id);
            // Stop and wait
            entity->stopEntity();
            entity->wait();
            // Remove from hash
            hash->remove(id);
            // Delete
            delete entity;
            return;
        }
    }
}

void World::setupWorldMap() {
    // Fill balls
    QList<quint8> balls =_ctr->balls();
    for(QList<quint8>::const_iterator iballs=balls.constBegin(); iballs!=balls.constEnd(); iballs++)
        _wm->addBall(*iballs);
    // Fill teams
    QList<quint8> teams = _ctr->teams();
    for(QList<quint8>::const_iterator iteams=teams.constBegin(); iteams!=teams.constEnd(); iteams++) {
        _wm->addTeam(*iteams, _ctr->teamName(*iteams));
        // Fill players in team
        QList<quint8> players = _ctr->players(*iteams);
        for(QList<quint8>::const_iterator i=players.constBegin(); i!=players.constEnd(); i++)
            _wm->addPlayer(*iteams, *i);
    }
}

void World::wmLockWrite() {
    _wmLock.lockForWrite();
}

void World::wmLockRead() {
    _wmLock.lockForRead();
}

void World::wmUnlock() {
    _wmLock.unlock();
}
