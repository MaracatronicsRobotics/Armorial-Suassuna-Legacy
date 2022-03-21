
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

#ifndef SSLGAMEINFO_H
#define SSLGAMEINFO_H

#include <QMutex>
#include <src/utils/text/text.h>
#include <src/entities/entity.h>
#include <src/entities/referee/referee.h>

#include <proto/messages.pb.h>

class GameInfo : public Entity {
private:
    static const int GAME_ON =  (1 << 0);
    static const int GAME_OFF = (1 << 1);
    static const int HALTED =   (1 << 2);

    static const int KICKOFF =  (1 << 3);
    static const int PENALTY =  (1 << 4);
    static const int DIRECT =   (1 << 5);
    static const int INDIRECT = (1 << 6);
    static const int RESTART =  (KICKOFF | PENALTY | DIRECT | INDIRECT);

    static const int BLUE =     (1 << 8);
    static const int YELLOW =   (1 << 9);

    static const int READY =    (1 << 10);
    static const int NOTREADY = (1 << 11);

    static const int TIMEOUT =  (1 << 12);        //not used yet

    static const int BALL_PLACEMENT = (1 << 13);

    int _state;
    QMutex mState;

    Color _color;
    int _stateColor;

    SSLReferee lastRefPack;
    QMutex mLastRefPack;

    QMutex mProcessCmd;
    void processCommand();

    void setState(int _state);

    quint8 _goalie;

public:
    GameInfo(Color _color);

    std::string refCommandToString(Referee_Command cmd);
    std::string refStageToString(Referee_Stage stage);
    std::string refTimeLeftToString();

    void updateGameInfo(SSLReferee &ref);

    Color getColor() const;
    bool isYellow() const;
    bool isBlue() const;

    Referee_Stage stage();
    Referee_Command command();

    Referee_TeamInfo ourTeamInfo();
    Referee_TeamInfo theirTeamInfo();
    Referee_Point desiredPosition();

    bool ballKicked();
    void setBallKicked();

    int getState();

    bool gameOn();
    bool restart();
    bool ourRestart();
    bool theirRestart();

    bool kickoff();
    bool ourKickoff();
    bool theirKickoff();

    bool penaltyKick();
    bool ourPenaltyKick();
    bool theirPenaltyKick();

    bool directKick();
    bool ourDirectKick();
    bool theirDirectKick();

    bool indirectKick();
    bool ourIndirectKick();
    bool theirIndirectKick();

    bool freeKick();
    bool ourFreeKick();
    bool theirFreeKick();

    bool timeOut();

    bool canMove();

    bool ballPlacement();
    bool ourBallPlacement();
    bool theirBallPlacement();

    bool allowedNearBall();

    bool canKickBall();

    // news
    int getYellowCards(Color colorAt);
    int getRedCards(Color colorAt);

    quint8 getGoalie() { return _goalie; }


    typedef enum {
        STATE_CANTMOVE,
        STATE_GAMEON,
        STATE_GAMEOFF,
        STATE_OURDIRECTKICK,
        STATE_OURINDIRECTKICK,
        STATE_OURKICKOFF,
        STATE_OURPENALTY,
        STATE_THEIRDIRECTKICK,
        STATE_THEIRINDIRECTKICK,
        STATE_THEIRPENALTY,
        STATE_THEIRKICKOFF,
        STATE_TIMEOUT,
        STATE_THEIRBALLPLACEMENT,
        STATE_OURBALLPLACEMENT,
        STATE_UNDEFINED
    } RefProcessedState;


    RefProcessedState processedState();
};

#endif // SSLGAMEINFO_H
