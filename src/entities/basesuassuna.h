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

#ifndef BASESUASSUNA_H
#define BASESUASSUNA_H

#include <QObject>
#include <QMap>
#include <QReadWriteLock>

#include <Armorial/Base/Client/Client.h>
#include <Armorial/Common/Types/Object/Object.h>
#include <Armorial/Common/Enums/Color/Color.h>
#include <Armorial/Threaded/Entity/Entity.h>
#include <Armorial/Utils/Timer/Timer.h>
#include <Armorial/Libs/nameof/include/nameof.hpp>

#include <include/proto/messages.pb.h>

class WorldMap;
class BaseStation;
class Player;
class Team;

class Coach;
class Playbook;
class Role;
class Behavior;
class Skill;

#endif // BASESUASSUNA_H
