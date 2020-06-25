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

#ifndef BEHAVIOUR_ATTACKER_H
#define BEHAVIOUR_ATTACKER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>
#include <utils/mrctimer/mrctimer.h>
#include <QObject>

class Behaviour_Attacker : public Behaviour {
    Q_OBJECT
private:
    void configure();
    void run();
    int _state;

    QList<quint8> _recvs;

    quint8 _bestReceiver;
    Position _kickPosition;
    QMutex _mutex;

    Position getBestAimPosition();
    Position calcImpactPositionInGoal();
    bool isBallInFront();
    bool isBallAlignedToGoal();
    bool hasBallAnyPathTo(Position posObjective);
    quint8 getBestReceiver();

    // Quadrant
    int getBestQuadrant();
    Position getQuadrantBarycenter(int quadrant);
    std::pair<Position, Position> getQuadrantInitialPosition(int quadrant);
    Position getBestPosition(int quadrant);

public:
    Behaviour_Attacker();
    QString name();

    Skill_Kick *_sk_kick;
    Skill_GoToLookTo *_sk_goto;
    Skill_PushBall2 *_sk_push;

    enum{
        SKT_KICK,
        SKT_PUSH,
        SKT_GOTO
    };

    enum{
        STATE_GOTO,
        STATE_KICK,
        STATE_PUSH,
        STATE_PASS,
        STATE_CANTKICK
    };

    enum{
        NO_QUADRANT,
        QUADRANT_UP,
        QUADRANT_MID,
        QUADRANT_BOT
    };

    void addReceiver(quint8 recvId) { _recvs.push_back(recvId); }
    void clearReceivers() { _recvs.clear(); }


signals:
    void goingToShoot(quint8 id);
    void shooted(quint8 id);
};

#endif // BEHAVIOUR_ATTACKER_H
