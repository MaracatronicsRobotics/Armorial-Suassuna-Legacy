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

#include <QNetworkInterface>
#include <QRegularExpression>

using namespace Suassuna;

Utils::ParameterHandler Constants::_parameterHandler = Utils::ParameterHandler();

void Constants::loadFile(QString filePath) {
    // Load constants file
    bool couldLoad = _parameterHandler.loadFile(filePath);

    // Check if could load
    if(couldLoad) {
        spdlog::info("[Constants] Readed constants file in path '{}'.", filePath.toStdString());
    }
    else {
        spdlog::error("[Constants] Failed to read constants file in path '{}'.", filePath.toStdString());
        exit(-1);
    }
}

QString Constants::refereeNetworkAddress() {
    QString address = _parameterHandler["Network"].getAsMap()["Referee"].getAsMap()["refereeNetworkAddress"].toString();
    bool matchNetworkAddress = QRegularExpression("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}").match(address).hasMatch();

    if(!matchNetworkAddress) {
        spdlog::error("[Constants] Failed to read a valid address in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return address;
}

quint16 Constants::refereeNetworkPort() {
    bool converted;
    quint16 port = _parameterHandler["Network"].getAsMap()["Referee"].getAsMap()["refereeNetworkPort"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid integer in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return port;
}

QString Constants::refereeNetworkInterface() {
    QString interface = _parameterHandler["Network"].getAsMap()["Referee"].getAsMap()["refereeNetworkInterface"].toString();
    bool networkInterfaceExists = QNetworkInterface::interfaceFromName(interface).isValid();

    if(!networkInterfaceExists) {
        spdlog::error("[Constants] Failed to read a valid network interface in '{}'.", __FUNCTION__);
    }

    return interface;
}

QString Constants::visionAddress() {
    QString address = _parameterHandler["Network"].getAsMap()["Vision"].getAsMap()["visionAddress"].toString();
    bool matchNetworkAddress = QRegularExpression("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}").match(address).hasMatch();

    if(!matchNetworkAddress) {
        spdlog::error("[Constants] Failed to read a valid address in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return address;
}

quint16 Constants::visionPort() {
    bool converted;
    quint16 port = _parameterHandler["Network"].getAsMap()["Vision"].getAsMap()["visionPort"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid integer in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return port;
}

QString Constants::visionNetworkInterface() {
    QString interface = _parameterHandler["Network"].getAsMap()["Vision"].getAsMap()["visionNetworkInterface"].toString();
    bool networkInterfaceExists = QNetworkInterface::interfaceFromName(interface).isValid();

    if(!networkInterfaceExists) {
        spdlog::error("[Constants] Failed to read a valid network interface in '{}'.", __FUNCTION__);
    }

    return interface;
}

QString Constants::actuatorAddress() {
    QString address = _parameterHandler["Network"].getAsMap()["Actuator"].getAsMap()["actuatorAddress"].toString();
    bool matchNetworkAddress = QRegularExpression("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}").match(address).hasMatch();

    if(!matchNetworkAddress) {
        spdlog::error("[Constants] Failed to read a valid address in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return address;
}

quint16 Constants::actuatorPort() {
    bool converted;
    quint16 port = _parameterHandler["Network"].getAsMap()["Actuator"].getAsMap()[(teamColor() == Common::Enums::Color::BLUE) ? "actuatorBluePort" : "actuatorYellowPort"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid integer in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return port;
}

QString Constants::actuatorNetworkInterface() {
    QString interface = _parameterHandler["Network"].getAsMap()["Actuator"].getAsMap()["actuatorNetworkInterface"].toString();
    bool networkInterfaceExists = QNetworkInterface::interfaceFromName(interface).isValid();

    if(!networkInterfaceExists) {
        spdlog::error("[Constants] Failed to read a valid network interface in '{}'.", __FUNCTION__);
    }

    return interface;
}

Common::Enums::Color Constants::teamColor() {
    QString teamColor = _parameterHandler["Team"].getAsMap()["teamColor"].toString();
    auto color = magic_enum::enum_cast<Common::Enums::Color>(teamColor.toUpper().toStdString());

    if(!color.has_value()) {
        spdlog::error("[Constants] Failed to read a valid color in '{}'.", __FUNCTION__);
        return Common::Enums::Color::UNDEFINED;
    }

    return color.value();
}

Common::Enums::Side Constants::teamPlaySide() {
    QString teamPlaySide = _parameterHandler["Team"].getAsMap()["teamPlaySide"].toString();
    auto playSide = magic_enum::enum_cast<Common::Enums::Side>("SIDE_" + teamPlaySide.toUpper().toStdString());

    if(!playSide.has_value()) {
        spdlog::error("[Constants] Failed to read a valid play side in '{}', defaulting to LEFT.", __FUNCTION__);
        return Common::Enums::Side::SIDE_LEFT;
    }

    return playSide.value();
}

quint16 Constants::maxNumPlayers() {
    bool converted;
    quint16 maxPlayers = _parameterHandler["Team"].getAsMap()["maxNumPlayers"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid integer in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return maxPlayers;
}

float Constants::univectorRadius() {
    bool converted;
    float univectorRadius = _parameterHandler["Univector"].getAsMap()["radius"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid float in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return univectorRadius;
}

float Constants::univectorKr() {
    bool converted;
    float univectorKr = _parameterHandler["Univector"].getAsMap()["kr"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid float in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return univectorKr;
}

float Constants::univectorDelta() {
    bool converted;
    float univectorDelta = _parameterHandler["Univector"].getAsMap()["delta"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid float in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return univectorDelta;
}

float Constants::univectorDmin() {
    bool converted;
    float univectorDMin = _parameterHandler["Univector"].getAsMap()["dMin"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid float in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return univectorDMin;
}

float Constants::univectorKo() {
    bool converted;
    float univectorKo = _parameterHandler["Univector"].getAsMap()["ko"].toInt(&converted);

    if(!converted) {
        spdlog::error("[Constants] Failed to read a valid float in '{}'.", __FUNCTION__);
        exit(-1);
    }

    return univectorKo;
}
