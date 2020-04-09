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

#include "playerbus.h"

#include <entity/contromodule/mrcteam.h>
#include <entity/player/player.h>

MRCTeam* PlayerBus::_ourTeam = NULL;
MRCTeam* PlayerBus::_theirTeam = NULL;

void PlayerBus::initialize(MRCTeam *ourTeam, MRCTeam *opTeam) {
    PlayerBus::_ourTeam = ourTeam;
    PlayerBus::_theirTeam = opTeam;
}

bool PlayerBus::ourPlayerAvailable(quint8 id) {
    QHash<quint8,Player*> avPlayers = PlayerBus::_ourTeam->avPlayers();
    return avPlayers.contains(id);
}

bool PlayerBus::theirPlayerAvailable(quint8 id) {
    QHash<quint8,Player*> avOpPlayers = PlayerBus::_theirTeam->avPlayers();
    return avOpPlayers.contains(id);
}

PlayerAccess* PlayerBus::ourPlayer(quint8 id) {
    QHash<quint8,Player*> avPlayers = PlayerBus::_ourTeam->avPlayers();
    if(avPlayers.contains(id)==false) {
        std::cout << "[ERROR] PlayerBus::ourPlayer(" << (int)id << "), requesting unavailable player id!\n";
        return NULL; // generates seg. fault
    }
    return avPlayers.value(id)->access();
}

PlayerAccess* PlayerBus::theirPlayer(quint8 id) {
    QHash<quint8,Player*> avOpPlayers = PlayerBus::_theirTeam->avPlayers();
    if(avOpPlayers.contains(id)==false) {
        std::cout << "[ERROR] PlayerBus::theirPlayer(" << (int)id << "), requesting unavailable player id!\n";
        return NULL; // generates seg. fault
    }
    return avOpPlayers.value(id)->access();
}
