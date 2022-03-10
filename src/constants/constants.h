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

#define ROBOT_INVALID_ID 200

class Constants
{
public:
    Constants(QString fileName);

    bool isTeamBlue();
    bool isTeamYellow();
    QString getTeamColor() const;
    void setTeamColor(const QString &teamColor);

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

private:
    QString _fileName;
    QFile file;

    // Network Constants
    QString _GRPCAddress;
    quint16 _GRPCActuatorPort;
    quint16 _GRPCCoachPort;

    QString _SimAddress;
    quint16 _SimActuatorPort;

    // Team Constants
    QString _teamColor;
    int _qtdPlayers;
    float _timeToSendPacketZero;
};

#endif // CONSTANTS_H