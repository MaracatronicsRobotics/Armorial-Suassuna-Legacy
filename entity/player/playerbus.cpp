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
