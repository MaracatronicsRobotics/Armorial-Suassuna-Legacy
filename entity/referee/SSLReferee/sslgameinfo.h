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

#ifndef SSLGAMEINFO_HH
#define SSLGAMEINFO_HH

#include <QMutex>
#include <utils/basics/color.hh>
#include <include/3rd_party/referee.pb.h>
#include <entity/baseentity.h>

class SSLGameInfo {
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

    int _state;
    QMutex mState;

    Colors::Color _color;
    int _stateColor;

    SSL_Referee lastRefPack;
    QMutex mLastRefPack;

    QMutex mProcessCmd;
    void processCommand();

    void setState(int _state);

    quint8 _goalie;

public:
    SSLGameInfo(Colors::Color _color);

    std::string refCommandToString(SSL_Referee_Command cmd);
    std::string refStageToString(SSL_Referee::Stage stage);
    std::string refTimeLeftToString();

    void updateGameInfo(SSL_Referee &ref);

    Colors::Color getColor() const;
    bool isYellow() const;
    bool isBlue() const;

    SSL_Referee::Stage stage();
    SSL_Referee_Command command();

    SSL_Referee_TeamInfo ourTeamInfo();
    SSL_Referee_TeamInfo theirTeamInfo();

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

    bool allowedNearBall();

    bool canKickBall();

    // news
    int getYellowCards(Colors::Color colorAt);
    int getRedCards(Colors::Color colorAt);

    quint8 getGoalie() { return _goalie; }

    // Mutex
    QMutex *_UIMutex;


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
        STATE_UNDEFINED
    } RefProcessedState;


    RefProcessedState processedState();
};

#endif // SSLGAMEINFO_HH
