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


#include "constants.h"
#include <iostream>
#include <src/utils/text/text.h>

Constants::Constants(QString fileName) {
    _fileName = fileName;


    file.setFileName(_fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject docObject = document.object();
    QVariantMap docMap = docObject.toVariantMap();

    _GRPCAddress = docMap["GRPCAddress"].toString();
    std::cout << Text::bold("GRPC Address: ") + Text::green(_GRPCAddress.toStdString(), true) << std::endl;

    _GRPCActuatorPort = docMap["GRPCActuatorPort"].toInt();
    std::cout << Text::bold("GRPC Actuator Port: ") + Text::green(std::to_string(_GRPCActuatorPort), true) << std::endl;

    _GRPCCoachPort = docMap["GRPCCoachPort"].toInt();
    std::cout << Text::bold("GRPC Coach Port: ") + Text::green(std::to_string(_GRPCCoachPort), true) << std::endl;

    _teamColor = docMap["teamColor"].toString();
    std::cout << Text::bold("Team Color: ") + Text::green(_teamColor.toStdString(), true) << std::endl;

    _teamSide = FieldSide(docMap["teamSide"].toString().toLower() == "left" ? Sides::LEFT : Sides::RIGHT);
    std::cout << Text::bold("Team Side: ") + Text::green(docMap["teamSide"].toString().toLower().toStdString(), true) << std::endl;

    _qtdPlayers = docMap["qtdPlayers"].toInt();
    std::cout << Text::bold("Number of players: ") + Text::green(std::to_string(_qtdPlayers), true) << std::endl;

    _timeToSendPacketZero = docMap["timeToSendPacketZero"].toFloat();
    std::cout << Text::bold("Time to send packet zero: ") + Text::green(std::to_string(_timeToSendPacketZero), true) << std::endl;

    _RefereeAddress = docMap["RefereeAddress"].toString();
    std::cout << Text::bold("Referee Address: ") + Text::green(_RefereeAddress.toStdString(), true) << std::endl;

    _RefereePort = docMap["RefereePort"].toInt();
    std::cout << Text::bold("Referee Port: ") + Text::green(std::to_string(_RefereePort), true) << std::endl;

    _minDistToConsiderBallMovement = docMap["minDistToConsiderBallMovement"].toFloat();
    std::cout << Text::bold("Min distance to consider ball movement Side: ") + Text::green(std::to_string(_minDistToConsiderBallMovement), true) << std::endl;

    _robotRadius = docMap["robotRadius"].toFloat();
    std::cout << Text::bold("Robot radius: ") + Text::green(std::to_string(_robotRadius), true) << std::endl;

    _keeperID = quint16(docMap["keeperID"].toInt());
    std::cout << Text::bold("Keeper ID: ") + Text::green(std::to_string(_keeperID), true) << std::endl;
}

bool Constants::isTeamBlue() {
    if (getTeamColor() == "blue") {
        return true;
    }
    return false;
}

bool Constants::isTeamYellow(){
    if (getTeamColor() == "yellow") {
        return true;
    }
    return false;
}

QString Constants::getTeamColor() const {
    return _teamColor;
}

void Constants::setTeamColor(const QString &teamColor) {
    _teamColor = teamColor;
}

FieldSide Constants::getTeamSide() {
    return _teamSide;
}

FieldSide Constants::getOppositeSide() {
    return _teamSide.oppositeSide();
}

void Constants::swapTeamSide() {
    _teamSide = FieldSide(_teamSide.isLeft() ? Sides::RIGHT : Sides::LEFT);
}

int Constants::getQtdPlayers() const {
    return _qtdPlayers;
}

void Constants::setQtdPlayers(int qtdPlayers) {
    _qtdPlayers = qtdPlayers;
}

QString Constants::getGRPCAddress() const {
    return _GRPCAddress;
}

void Constants::setGRPCAddress(const QString &GRPCAddress) {
    _GRPCAddress = GRPCAddress;
}

quint16 Constants::getGRPCActuatorPort() const {
    return _GRPCActuatorPort;
}

void Constants::setGRPCActuatorPort(const quint16 &GRPCActuatorPort) {
    _GRPCActuatorPort = GRPCActuatorPort;
}

quint16 Constants::getGRPCCoachPort() const {
    return _GRPCCoachPort;
}

void Constants::setGRPCCoachPort(const quint16 &GRPCCoachPort) {
    _GRPCCoachPort = GRPCCoachPort;
}

QString Constants::getSimAddress() const {
    return _SimAddress;
}

void Constants::setSimAddress(const QString &SimAddress) {
    _SimAddress = SimAddress;
}

quint16 Constants::getSimActuatorPort() const {
    return _SimActuatorPort;
}

void Constants::setSimActuatorPort(const quint16 &SimActuatorPort) {
    _SimActuatorPort = SimActuatorPort;
}

float Constants::getTimeToSendPacketZero() const {
    return _timeToSendPacketZero;
}

void Constants::setTimeToSendPacketZero(float timeToSendPacketZero) {
    _timeToSendPacketZero = timeToSendPacketZero;
}

QString Constants::getRefereeAddress() const
{
    return _RefereeAddress;
}

void Constants::setRefereeAddress(const QString &RefereeAddress)
{
    _RefereeAddress = RefereeAddress;
}

quint16 Constants::getRefereePort() const
{
    return _RefereePort;
}

void Constants::setRefereePort(const quint16 &RefereePort)
{
    _RefereePort = RefereePort;
}

float Constants::getMinDistToConsiderBallMovement(){
    return _minDistToConsiderBallMovement;
}

void Constants::setMinDistToConsiderBallMovement(float minDistToConsiderBallMovement){
    _minDistToConsiderBallMovement = minDistToConsiderBallMovement;
}

float Constants::getRobotRadius() const
{
    return _robotRadius;
}

void Constants::setRobotRadius(float value)
{
    _robotRadius = value;
}

quint16 Constants::getKeeperID() const
{
    return _keeperID;
}

void Constants::setKeeperID(const quint16 &keeperID)
{
    _keeperID = keeperID;
}
