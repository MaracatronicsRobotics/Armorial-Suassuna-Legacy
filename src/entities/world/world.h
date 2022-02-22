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

#ifndef WORLD_H
#define WORLD_H

#include <QMap>

#include <src/entities/entity.h>
#include <src/constants/constants.h>

class World : public Entity
{
public:
    World(Constants *constants);
    ~World();
    QString name();

    // Entities management
    void addEntity(Entity *e, int priority);

private:
    // Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    // Constants
    Constants *_constants;
    Constants* getConstants();

    // Internal
    void startEntities();
    void stopAndDeleteEntities();

    // Hashtable for entities
    QMap<int, QHash<int, Entity*>*> _moduleEntities;
};

#endif // WORLD_H
