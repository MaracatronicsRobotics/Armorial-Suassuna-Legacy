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

#include "include/3rd_party/grSim_Packet.pb.h"
#include "include/3rd_party/grSim_Commands.pb.h"
#include "include/3rd_party/grSim_Replacement.pb.h"
#include <QByteArray>
#include <QUdpSocket>

#include <entity/entity.h>

#define MAX_TEAMS 2
#define MAX_ROBOTS 6

typedef struct{
    bool isYellow;
    int id;
    double v1, v2, v3, v4;
    double vx ,vy, angle;
    double kickspeedx, kickspeedz;
    bool spinner;
} grs_robot;

class grsSimulator : public Entity {

public:
    grsSimulator();
    QString name();
    void sendPacket(grs_robot robot);
    void setSpeed(quint8 teamId, quint8 robotId, float vx, float vy, float theta);
    void setKickSpeed(quint8 teamId, quint8 robotId, float kickx, float kickz);
    void setDribble(quint8 teamId, quint8 robotId, bool active);
private:
    QMutex _writeMutex;
    QUdpSocket _socket;
    grs_robot packets[MAX_TEAMS][MAX_ROBOTS];

    // Entity methods
    void initialization();
    void loop();
    void finalization();
};
