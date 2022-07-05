#include "playerboard.h"

PlayerBoard::PlayerBoard()
{

}

void PlayerBoard::setPlayersList(QList<Player *> playerList){
    _playersList = playerList;
}

QList<quint8> PlayerBoard::getPlayersIds(const roles role){
    QList<quint8> ids;
    QString roleName = QString::fromStdString(std::string(magic_enum::enum_name(role)));
    for (Player* player : _playersList){
        if (player->roleName() == roleName){
            ids.push_back(player->getPlayerID());
        }
    }
    return ids;
}

QHash<quint8, Position> PlayerBoard::getPlayersPositions(const roles role){
    QHash<quint8, Position> idPositionHashmap;
    QString roleName = QString::fromStdString(std::string(magic_enum::enum_name(role)));
    for (Player* player : _playersList){
        if (player->roleName() == roleName){
            idPositionHashmap.insert(player->getPlayerID(), player->getPlayerPos());
        }
    }
    return idPositionHashmap;
}
