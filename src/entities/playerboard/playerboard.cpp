#include "playerboard.h"

PlayerBoard::PlayerBoard()
{

}

Robot PlayerBoard::getKeeper()
{
    _keeperLock.lockForRead();
    Robot keeper = _keeper;
    _keeperLock.unlock();
    return keeper;
}

void PlayerBoard::setKeeper(const Robot &keeper)
{
    _keeperLock.lockForWrite();
    _keeper = keeper;
    _keeperLock.unlock();
}

QList<Robot> PlayerBoard::getBarriersList()
{
    _barriersLock.lockForRead();
    QList<Robot> barriers = _barriersList;
    _barriersLock.unlock();
    return barriers;
}

void PlayerBoard::setBarriersList(const QList<Robot> &barriersList)
{
    _barriersLock.lockForWrite();
    _barriersList = barriersList;
    _barriersLock.unlock();
}

void PlayerBoard::addBarrier(const Robot newBarrier){
    _barriersLock.lockForWrite();
    _barriersList.append(newBarrier);
    _barriersLock.unlock();
}

void PlayerBoard::clearBarriersList(){
    _barriersLock.lockForWrite();
    _barriersList.clear();
    _barriersLock.unlock();
}

QList<Robot> PlayerBoard::getMidfieldersList()
{
    _midfieldersLock.lockForRead();
    QList<Robot> midfielders = _midfieldersList;
    _midfieldersLock.unlock();
    return midfielders;
}

void PlayerBoard::setMidfieldersList(const QList<Robot> &midfieldersList)
{
    _midfieldersLock.lockForWrite();
    _midfieldersList = midfieldersList;
    _midfieldersLock.unlock();
}

void PlayerBoard::addMidfielder(const Robot newMidfielder){
    _midfieldersLock.lockForWrite();
    _midfieldersList.append(newMidfielder);
    _midfieldersLock.unlock();
}

void PlayerBoard::clearMidfieldersList(){
    _midfieldersLock.lockForWrite();
    _midfieldersList.clear();
    _midfieldersLock.unlock();
}

QList<Robot> PlayerBoard::getForwardersList()
{
    _forwardersLock.lockForRead();
    QList<Robot> forwarders = _forwardersList;
    _forwardersLock.lockForWrite();
    return forwarders;
}

void PlayerBoard::setForwardersList(const QList<Robot> &forwardersList)
{
    _forwardersLock.lockForWrite();
    _forwardersList = forwardersList;
    _forwardersLock.lockForWrite();
}

void PlayerBoard::addForwarder(const Robot newForwarder){
    _forwardersLock.lockForWrite();
    _forwardersList.append(newForwarder);
    _forwardersLock.unlock();
}

void PlayerBoard::clearForwardersList(){
    _forwardersLock.lockForWrite();
    _forwardersList.clear();
    _forwardersLock.lockForWrite();
}

QList<Robot> PlayerBoard::getTeamList()
{
    _teamLock.lockForRead();
    QList<Robot> team = _teamList;
    _teamLock.unlock();
    return team;
}

void PlayerBoard::setTeamList(const QList<Robot> &teamList)
{
    _teamLock.lockForWrite();
    _teamList = teamList;
    _teamLock.unlock();
}

QHash<quint8, Position> PlayerBoard::getBarriersPositions(){
    QHash<quint8, Position> hashmap;
    _barriersLock.lockForRead();
    for (Robot r : _barriersList){
        quint8 robotId = r.robotidentifier().robotid();
        Position robotPosition = r.robotposition();
        hashmap.insert(robotId, robotPosition);
    }
    _barriersLock.unlock();
    return hashmap;
}

QHash<quint8, Position> PlayerBoard::getMidfieldersPositions(){
    QHash<quint8, Position> hashmap;
    _midfieldersLock.lockForRead();
    for (Robot r : _midfieldersList){
        quint8 robotId = r.robotidentifier().robotid();
        Position robotPosition = r.robotposition();
        hashmap.insert(robotId, robotPosition);
    }
    _midfieldersLock.unlock();
    return hashmap;
}

QHash<quint8, Position> PlayerBoard::getForwardersPositions(){
    QHash<quint8, Position> hashmap;
    _forwardersLock.lockForRead();
    for (Robot r : _forwardersList){
        quint8 robotId = r.robotidentifier().robotid();
        Position robotPosition = r.robotposition();
        hashmap.insert(robotId, robotPosition);
    }
    _forwardersLock.unlock();
    return hashmap;
}

QHash<quint8, Position> PlayerBoard::getTeamPositions(){
    QHash<quint8, Position> hashmap;
    _teamLock.lockForRead();
    for (Robot r : _teamList){
        quint8 robotId = r.robotidentifier().robotid();
        Position robotPosition = r.robotposition();
        hashmap.insert(robotId, robotPosition);
    }
    _teamLock.unlock();
    return hashmap;
}


void PlayerBoard::clearTeamList(){
    _teamLock.lockForWrite();
    _teamList.clear();
    _teamLock.unlock();
}

void PlayerBoard::clearAllLists(){
    clearBarriersList();
    clearMidfieldersList();
    clearForwardersList();
    clearTeamList();
}

