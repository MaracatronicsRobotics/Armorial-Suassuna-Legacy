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

#include <QHash>

#include <src/utils/text/text.h>

World::World(Constants *constants) {
    _constants = constants;
}

World::~World() {

}

QString World::name() {
    return "World";
}

void World::addEntity(Entity *e, int priority) {
    // Check if priority is already created
    QHash<int, Entity*> *prioEntities;
    if(!_moduleEntities.keys().contains(priority)) {
         prioEntities = new QHash<int, Entity*>();
        _moduleEntities.insert(priority, prioEntities);
    }

    prioEntities = _moduleEntities.value(priority);
    prioEntities->insert(e->entityId(), e);
}

void World::initialization() {
    std::cout << Text::cyan("[WORLD] ", true) + Text::bold("Initializing modules.") + '\n';
    startEntities();
}

void World::loop() {
    // World is currently disabled, so loop() doesn't run (change if needed!)
}

void World::finalization() {
    std::cout << Text::cyan("[WORLD] ", true) + Text::bold("Finishing modules.") + '\n';
    stopAndDeleteEntities();
}

void World::startEntities() {
    // Get priorities in hash
    QList<int> priorities = _moduleEntities.keys();

    // In each priority (decreasing, most priority first)
    const int prioritiesSize = priorities.size();
    for(int i = prioritiesSize - 1; i >= 0; i--) {
        const int priority = priorities.at(i);

        // Get associated entities
        const QList<Entity*> entities = _moduleEntities.value(priority)->values();

        // Start those entities
        QList<Entity*>::const_iterator it;
        for(it = entities.constBegin(); it != entities.constEnd(); it++) {
            // Take entity
            Entity *entity = *it;

            // Set frequency
            entity->setLoopFrequency(/*getConstants()->threadFrequency()*/60);

            // Start entity
            entity->start();
        }
    }
}

void World::stopAndDeleteEntities() {
    // Get priorities in hash
    QList<int> priorities = _moduleEntities.keys();

    // In each priority (increasing)
    const int prioritiesSize = priorities.size();
    for(int i = 0; i < prioritiesSize; i++) {
        const int priority = priorities.at(i);

        // Get associated entities
        const QList<Entity*> entities = _moduleEntities.value(priority)->values();

        // Stop those entities
        QList<Entity*>::const_iterator it;
        for(it = entities.constBegin(); it != entities.constEnd(); it++) {
            // Take entity
            Entity *entity = *it;

            // Stop entity
            entity->stopEntity();

            // Wait entity
            entity->wait();
        }

        // Delete those entities
        for(it = entities.constBegin(); it != entities.constEnd(); it++) {
            // Take entity
            Entity *entity = *it;

            // Remove entity from hash
            _moduleEntities.value(priority)->remove(entity->entityId());

            // Delete entity
            delete entity;
        }
    }
}

Constants* World::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::red("[ERROR] ", true) << Text::bold("Constants with nullptr value at World") + '\n';
    }
    else {
        return _constants;
    }

    return nullptr;
}
