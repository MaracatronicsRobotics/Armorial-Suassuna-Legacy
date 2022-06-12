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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QtCore>
#include <QList>

#include <src/utils/types/fieldside/fieldside.h>
#include <proto/messages.pb.h>
#include <src/utils/utils.h>

#define ROBOT_INVALID_ID 200
#define YELLOW_ID 0
#define BLUE_ID 1

using namespace Armorial;

class Constants
{
public:
    Constants(QString fileName);

    bool isTeamBlue();
    bool isTeamYellow();
    QString getTeamColor() const;
    void setTeamColor(const QString &teamColor);
    Color teamColor();
    float getRobotRadius();
    float getBallRadius();

    FieldSide getTeamSide();
    FieldSide getOppositeSide();
    void swapTeamSide();

    int getQtdPlayers() const;
    void setQtdPlayers(int qtdPlayers);

    QString getGRPCAddress() const;
    void setGRPCAddress(const QString &GRPCAddress);

    quint16 getGRPCActuatorPort() const;
    void setGRPCActuatorPort(const quint16 &GRPCActuatorPort);

    quint16 getGRPCCoachPort() const;
    void setGRPCCoachPort(const quint16 &GRPCCoachPort);

    QString getSimAddress() const;
    void setSimAddress(const QString &SimAddress);

    quint16 getSimActuatorPort() const;
    void setSimActuatorPort(const quint16 &SimActuatorPort);

    float getTimeToSendPacketZero() const;
    void setTimeToSendPacketZero(float timeToSendPacketZero);
    
    QString getRefereeAddress() const;
    void setRefereeAddress(const QString &RefereeAddress);

    quint16 getRefereePort() const;
    void setRefereePort(const quint16 &RefereePort);

    QString getTeamSide() const;
    void setTeamSide(const QString &teamSide);

    float getMinDistToConsiderBallMovement();
    void setMinDistToConsiderBallMovement(float minDistToConsiderBallMovement);

    float getRobotRadius() const;
    void setRobotRadius(float value);

    quint16 getKeeperID() const;
    void setKeeperID(const quint16 &keeperID);

    float getBallRadius() const;
    void setBallRadius(float ballRadius);

    float maxKickPower();
    float maxChipKickPower();
    float maxRobotLinearSpeed();
    float maxRobotAngularSpeed();
    float playerChipKickAngle();
    QList<float> playerLinearPID();
    QList<float> playerAngularPID();
    QList<float> keeperLinearPID();
    QList<float> keeperAngularPID();

private:
    QString _fileName;
    QFile file;

    // Network Constants
    QString _GRPCAddress;
    quint16 _GRPCActuatorPort;
    quint16 _GRPCCoachPort;

    QString _SimAddress;
    quint16 _SimActuatorPort;
    
    QString _RefereeAddress;
    quint16 _RefereePort;

    // Team Constants
    QString _teamColor;
    Color _color;
    FieldSide _teamSide;
    int _qtdPlayers;
    float _robotRadius;
    float _ballRadius;
    float _timeToSendPacketZero;
    quint16 _keeperID;
    float _maxKickPower;
    float _maxChipKickPower;
    float _maxRobotLinearSpeed;
    float _maxRobotAngularSpeed;
    float _playerChipKickAngle;
    QList<float> _playerLinearPID;
    QList<float> _playerAngularPID;
    QList<float> _keeperLinearPID;
    QList<float> _keeperAngularPID;

    // Foul/Referee-related constants
    float _minDistToConsiderBallMovement;

    // Robot dimensions
    float _robotRadius;

    float _ballRadius;
};

#endif // CONSTANTS_H
