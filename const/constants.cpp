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

int MRCConstants::_threadFrequency; // frequencia das threads criadas
int MRCConstants::_guiUpdateFrequency;  // frequencia de update da GUI

/* Fast Path Planning */
float MRCConstants::_FPPBallThreshHold;
float MRCConstants::_FPPRobotThreshHold;
float MRCConstants::_FPPBreakDistance;
float MRCConstants::_FPPSmoothPathResolution;

/* Soccer constants */
int MRCConstants::_qtPlayers;
float MRCConstants::_maxKickPower;
float MRCConstants::_robotRadius;
float MRCConstants::_ballRadius;

/* Ball Sensor constants */
float MRCConstants::_distToConsiderBallMovement;

/* Colors */
std::string MRCConstants::red = "\033[1;31m";
std::string MRCConstants::green = "\033[1;32m";
std::string MRCConstants::yellow = "\033[1;33m";
std::string MRCConstants::blue = "\033[1;34m";
std::string MRCConstants::magenta = "\033[1;35m";
std::string MRCConstants::cyan = "\033[1;36m";
std::string MRCConstants::defaultBold = "\033[1m";
std::string MRCConstants::reset = "\033[0m";

MRCConstants::MRCConstants()
MRCConstants::MRCConstants(QString configFileName)
{
    file.setFileName(configFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject docObject = document.object();
    QVariantMap doc_map = docObject.toVariantMap();

    MRCConstants::_qtPlayers               = doc_map["NumberOfPlayers"].toInt();
    MRCConstants::_maxKickPower            = doc_map["MaxKickPower"].toFloat();
    MRCConstants::_robotRadius             = doc_map["RobotRadius"].toFloat();
    MRCConstants::_ballRadius              = doc_map["BallRadius"].toFloat();

    MRCConstants::_distToConsiderBallMovement=doc_map["DistToConsiderBallMovement"].toFloat();

    MRCConstants::_threadFrequency=doc_map["ThreadFrequency"].toInt();
    MRCConstants::_guiUpdateFrequency=doc_map["GuiUpdateFrequency"].toInt();


    QVariantMap fpp_map = doc_map["LinearPID"].toMap();
    MRCConstants::_FPPBallThreshHold=fpp_map["FPPBallThreshHold"].toFloat();
    MRCConstants::_FPPRobotThreshHold=fpp_map["FPPRobotThreshHold"].toFloat();
    MRCConstants::_FPPBreakDistance=fpp_map["FPPBreakDistance"].toFloat();
    MRCConstants::_FPPSmoothPathResolution=fpp_map["FPPSmoothPathResolution"].toFloat();
}
