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

#include <Armorial/Common/Enums/Enums.h>

#include <src/constants/constants.h>
#include <include/proto/ssl_gc_referee_message.pb.h>

/*!
 * \brief The SSLGameInfo class
 */
class SSLGameInfo
{
public:
    /*!
     * \brief SSLGameInfo
     */
    SSLGameInfo();

    /*!
     * \brief updateGameInfo
     * \param referee
     */
    void updateGameInfo(const Referee& refereeMessage);

    /*!
     * \return The goalie id from game controller.
     */
    quint8 getGoalieID();

    /*!
     * \return The current game stage.
     */
    Referee::Stage gameStage();

    /*!
     * \return The current referee command.
     */
    Referee_Command refereeCommand();

    /*!
     * \return Returns the referee team info.
     */
    Referee_TeamInfo ourTeamInfo();
    Referee_TeamInfo theirTeamInfo();

    /*!
     * \return The current ball placement position.
     */
    Referee_Point ballPlacementPosition();

    /*!
     * \return Returns the current state.
     */
    int getState();

    /*!
     * \brief Mark the ball as kicked (set game on in direct kicks)
     */
    void setBallKicked();

    // Methods used to general requests from Referee
    bool gameOn() { return getState() & GAME_ON; }
    bool restart() { return getState() & RESTART; }
    bool ourRestart() { return restart() && (getState() & _ourStateColor); }
    bool theirRestart() { return restart() && !(getState() & _ourStateColor); }

    bool kickoff() { return getState() & KICKOFF; }
    bool ourKickoff() { return kickoff() && (getState() & _ourStateColor); }
    bool theirKickoff() { return kickoff() && !(getState() & _ourStateColor); }

    bool penaltyKick() { return getState() & PENALTY; }
    bool ourPenaltyKick() { return penaltyKick() && (getState() & _ourStateColor); }
    bool theirPenaltyKick() { return penaltyKick() && !(getState() & _ourStateColor); }

    bool directKick() { return getState() & DIRECT; }
    bool ourDirectKick() { return directKick() && (getState() & _ourStateColor); }
    bool theirDirectKick() { return directKick() && !(getState() & _ourStateColor); }

    bool indirectKick() { return getState() & INDIRECT; }
    bool ourIndirectKick() { return indirectKick() && (getState() & _ourStateColor); }
    bool theirIndirectKick() { return indirectKick() && !(getState() & _ourStateColor); }

    bool freeKick() { return directKick() || indirectKick(); }
    bool ourFreeKick() { return ourDirectKick() || ourIndirectKick(); }
    bool theirFreeKick() { return theirDirectKick() || theirIndirectKick(); }

    bool timeOut() { return getState() == TIMEOUT; }

    bool canMove() { return (getState() != HALTED); }

    bool ballPlacement() { return getState() & BALL_PLACEMENT; }
    bool ourBallPlacement() { return ballPlacement() && (getState() & _ourStateColor); }
    bool theirBallPlacement() { return ballPlacement() && !(getState() & _ourStateColor); }

    bool allowedNearBall() { return gameOn() || (getState() & _ourStateColor); }

    bool canKickBall() { return gameOn() || (ourRestart() && (getState() & READY)); }

    int getYellowCards(Common::Enums::Color& teamColor);
    int getRedCards(Common::Enums::Color& teamColor);

public:
    static const int GAME_ON  = (1 << 0);
    static const int GAME_OFF = (1 << 1);
    static const int HALTED   = (1 << 2);

    static const int KICKOFF  = (1 << 3);
    static const int PENALTY  = (1 << 4);
    static const int DIRECT   = (1 << 5);
    static const int INDIRECT = (1 << 6);
    static const int RESTART  = (KICKOFF | PENALTY | DIRECT | INDIRECT);

    static const int BLUE     = (1 << 8);
    static const int YELLOW   = (1 << 9);

    static const int READY    = (1 << 10);
    static const int NOTREADY = (1 << 11);
    static const int TIMEOUT  = (1 << 12);

    static const int BALL_PLACEMENT = (1 << 13);

private:
    /*!
     * \brief Set the current state to a given state.
     * \param state The given state.
     */
    void setState(int state);

    /*!
     * \brief processCommand
     */
    void processCommand();

    int _refereeState;
    int _ourStateColor;
    Referee _lastRefereeCommand;
    quint8 _goalieIdFromReferee;
    QMutex _stateMutex;
};

#endif // SSLGAMEINFO_H
