#include "playerboard.h"

PlayerBoard::PlayerBoard()
{

}

void PlayerBoard::setPlayersList(QList<Player *> playerList){
    _playersList = playerList;
}

QList<quint8> PlayerBoard::getPlayersIds(const roles role){
    QList<quint8> ids;
    QString roleName = magic_enum::enum_name(role).data();
    for (Player* player : _playersList){
        if (player->roleName() == roleName){
            ids.push_back(player->getPlayerID());
        }
    }
    return ids;
}

QHash<quint8, Position> PlayerBoard::getTeamPlayersPositions(){
    QHash<quint8, Position> idPositionHashmap;
    for (Player* player : _playersList){
        idPositionHashmap.insert(player->getPlayerID(), player->getPlayerPos());
    }
    return idPositionHashmap;
}

QHash<quint8, Position> PlayerBoard::getRolePlayersPositions(const roles role){
    QHash<quint8, Position> idPositionHashmap;
    QString roleName = magic_enum::enum_name(role).data();
    for (Player* player : _playersList){
        if (player->roleName() == roleName){
            idPositionHashmap.insert(player->getPlayerID(), player->getPlayerPos());
        }
    }
    return idPositionHashmap;
}

quint8 PlayerBoard::getClosestTeamPlayerTo(const Position &target){
    quint8 closestPlayer = -1;
    float smallestDistance = 99999.9f;
    for (Player* player : _playersList){
        float playerDistance = Utils::distance(player->getPlayerPos(), target);
        if (playerDistance <= smallestDistance){
            closestPlayer = player->getPlayerID();
            smallestDistance = playerDistance;
        }
    }
    return closestPlayer;
}

quint8 PlayerBoard::getClosestRolePlayerTo(const Position &target, const roles role){
    quint8 closestPlayer = -1;
    float smallestDistance = 99999.9f;
    QString roleName = magic_enum::enum_name(role).data();
    for (Player* player : _playersList){
        if (player->roleName() == roleName){
            float playerDistance = Utils::distance(player->getPlayerPos(), target);
            if (playerDistance <= smallestDistance){
                closestPlayer = player->getPlayerID();
                smallestDistance = playerDistance;
            }
        }
    }
    return closestPlayer;
}
