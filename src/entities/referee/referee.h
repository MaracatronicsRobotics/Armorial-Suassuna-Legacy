#ifndef REFEREE_H
#define REFEREE_H

#include <QUdpSocket>
#include <QMutex>
#include <QNetworkDatagram>

#include <src/utils/text/text.h>
#include <proto/messages.pb.h>
#include <src/entities/entity.h>
#include <src/entities/referee/gameinfo/gameinfo.h>
#include <src/entities/referee/ballplay/ballplay.h>

class SSLReferee : public Entity
{
    Q_OBJECT
public:
    SSLReferee(Constants *constants, WorldMap *worldMap);
    ~SSLReferee();

    QString name();

    GameInfo* getGameInfo();
    int getRemainingTime();
    Referee_Stage getLastStage();
    Referee_Command getLastCommand();
    Referee_TeamInfo getLastTeamInfo(Color teamColor);
    Position getLastPlacementPosition();

    // Return if ball is inside field, outside goal areas and player can take it
    bool isBallInPlay();

private:
    // Entity inherited methods
    void initialization();
    void loop();
    void finalization();

    // Socket management
    QUdpSocket *_refereeSocket;
    void createAndInitializeSocket();

    // Packet management
    Referee_Command _lastCommand;
    Referee_Stage _lastStage;
    QHash<Colors::Color, Referee_TeamInfo> _lastTeamsInfo;
    Position _lastPlacementPosition;
    int _remainingTime;
    QReadWriteLock _packetMutex;

    // BallPlay checker
    BallPlay *_ballPlay;

    // GameInfo
    GameInfo *_gameInfo;

    // Constants
    Constants *_constants;
    Constants* getConstants();

    // WorldMap
    WorldMap *_worldMap;
    WorldMap* getWorldMap();

signals:
    void updateReferee();
};

#endif // REFEREE_H
