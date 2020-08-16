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

#ifndef ROLE_STRIKER_H
#define ROLE_STRIKER_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Striker : public Role
{
    Q_OBJECT
private:
    // Behaviours
    Behaviour_Attacker      *_bh_atk;
    Behaviour_Penalty_CF    *_bh_atp;
    Behaviour_Receiver      *_bh_rcv;
    Behaviour_MarkBall      *_bh_mkb;
    Behaviour_MarkPlayer    *_bh_mkp;
    Behaviour_BallPlacement *_bh_plc;

    // Behaviours Enum
    enum{
        BEHAVIOUR_ATTACKER,
        BEHAVIOUR_PENALTYATK,
        BEHAVIOUR_RECEIVER,
        BEHAVIOUR_MARKBALL,
        BEHAVIOUR_MARKPLAYER,
        BEHAVIOUR_PLACEMENT
    };

    // Timer
    Timer updateReceiversTimer;

    // Parameters
    bool _config;
    bool _isMarkNeeded;
    quint8 _attackerId;
    quint8 _markId;
    int _quadrant;

    // Placement
    bool setPlacement;

    // Utils functions
    bool isBallComing(float minVelocity, float radius);

    // Inherited functions
    void configure();
    void run();

public:
    Role_Striker();
    void initializeBehaviours();
    QString name();

    // Mark
    void setMarkId(quint8 markId) { _markId = markId; }
    void setQuadrant(int quadrant) { _quadrant = quadrant; }

signals:
    void requestReceivers(quint8 playerId);
    void requestAttacker();
    void requestIsMarkNeeded();

public slots:
    void takeReceiver(quint8 receiverId);
    void takeAttacker(quint8 attackerId);
    void takeIsMarkNeeded(bool isMarkNeeded);
};

#endif // ROLE_CENTRE_FORWARD_H
