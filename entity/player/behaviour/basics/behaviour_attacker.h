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

class Behaviour_Attacker : public Behaviour {
private:
    void configure();
    void run();

    // Machine state
    int _state;
    enum {
        STATE_CANTKICK,
        STATE_PUSH
    };

    // Skill Transitions
    enum {
        SKT_POS,
        SKT_PUSH,
        SKT_KICK
    };

    // Receivers
    QList<quint8> _receiversList;
    quint8 getBestReceiver();
    bool _alreadyShooted;
    quint8 _bestRcv;

    // Utils functions
    bool canTakeBall();
    quint8 getTheirClosestPlayerToGoal();
    std::pair<float, Position> getBestAimPosition();

public:
    Behaviour_Attacker();
    QString name();

    // Skills
    Skill_GoToLookTo *_sk_goToLookTo;
    Skill_Test *_sk_push;
    Skill_Kick *_sk_kick;

    // Receivers add
    void addReceiver(quint8 id) { _receiversList.push_back(id); }
    void clearReceivers()       { _receiversList.clear(); }
};

#endif // BEHAVIOUR_ATTACKER_H
