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





/* Colors */
std::string MRCConstants::red = "\033[1;31m";
std::string MRCConstants::green = "\033[1;32m";
std::string MRCConstants::yellow = "\033[1;33m";
std::string MRCConstants::blue = "\033[1;34m";
std::string MRCConstants::magenta = "\033[1;35m";
std::string MRCConstants::cyan = "\033[1;36m";
std::string MRCConstants::defaultBold = "\033[1m";
std::string MRCConstants::reset = "\033[0m";

/* GUI */
float MRCConstants::robotZ = 2.0;



MRCConstants::MRCConstants(QString configFileName)
{
    file.setFileName(configFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject docObject = document.object();
    QVariantMap doc_map = docObject.toVariantMap();

    _qtPlayers = doc_map["NumberOfPlayers"].toInt();
    std::cout<< MRCConstants::defaultBold << "NumberOfPlayers: "<< MRCConstants::green <<_qtPlayers<<std::endl<< MRCConstants::reset;

    _maxKickPower = doc_map["MaxKickPower"].toFloat();
    std::cout<< MRCConstants::defaultBold << "MaxKickPower: "<< MRCConstants::green <<doc_map["MaxKickPower"].toInt()<<std::endl<< MRCConstants::reset;

    _robotRadius = doc_map["RobotRadius"].toFloat();
    std::cout<< MRCConstants::defaultBold << "RobotRadius: "<< MRCConstants::green <<_robotRadius<<std::endl<< MRCConstants::reset;

    _ballRadius = doc_map["BallRadius"].toFloat();
    std::cout<< MRCConstants::defaultBold << "BallRadius: "<< MRCConstants::green <<_ballRadius<<std::endl<< MRCConstants::reset;

    _distToConsiderBallMovement=doc_map["DistToConsiderBallMovement"].toFloat();
    std::cout<< MRCConstants::defaultBold << "DistToConsiderBallMovement: "<< MRCConstants::green <<_distToConsiderBallMovement<<std::endl<< MRCConstants::reset;

    _threadFrequency=doc_map["ThreadFrequency"].toInt();
    std::cout<< MRCConstants::defaultBold << "ThreadFrequency: "<< MRCConstants::green <<_threadFrequency<<std::endl<< MRCConstants::reset;

    _guiUpdateFrequency=doc_map["GuiUpdateFrequency"].toInt();
    std::cout<< MRCConstants::defaultBold << "GuiUpdateFrequency: "<< MRCConstants::green <<_guiUpdateFrequency<<std::endl<< MRCConstants::reset;

    _goalieId=quint8(doc_map["GoalieID"].toInt());
    std::cout<< MRCConstants::defaultBold << "GoalieID: "<< MRCConstants::green <<int(_goalieId)<<std::endl<< MRCConstants::reset;

    // Path to files
    _clustersPath=doc_map["ClusterAbsolutePath"].toString();
    std::cout<< MRCConstants::defaultBold << "ClusterAbsolutePath: "<< MRCConstants::green << _clustersPath.toStdString() << std::endl<< MRCConstants::reset;

    _MLPPath=doc_map["MLPAbsolutePath"].toString();
    std::cout<< MRCConstants::defaultBold << "MLPAbsolutePath: "<< MRCConstants::green << _MLPPath.toStdString() << std::endl<< MRCConstants::reset;

    // Path Planning Constants
    QVariantMap fpp_map = doc_map["FPP"].toMap();
    _FPPBallThreshHold=fpp_map["FPPBallThreshHold"].toFloat();
    std::cout<< MRCConstants::defaultBold << "FPPBallThreshHold: "<< MRCConstants::green << _FPPBallThreshHold << std::endl<< MRCConstants::reset;

    _FPPRobotThreshHold=fpp_map["FPPRobotThreshHold"].toFloat();
    std::cout<< MRCConstants::defaultBold << "FPPRobotThreshHold: "<< MRCConstants::green << _FPPRobotThreshHold << std::endl<< MRCConstants::reset;

    _FPPBreakDistance=fpp_map["FPPBreakDistance"].toFloat();
    std::cout<< MRCConstants::defaultBold << "FPPBreakDistance: "<< MRCConstants::green << _FPPBreakDistance << std::endl<< MRCConstants::reset;

    _FPPSmoothPathResolution=fpp_map["FPPSmoothPathResolution"].toFloat();
    std::cout<< MRCConstants::defaultBold << "FPPSmoothPathResolution: "<< MRCConstants::green << _FPPSmoothPathResolution << std::endl<< MRCConstants::reset;

    // Pid constants
    // Linear
    QVariantMap linear_map = doc_map["LinearPID"].toMap();
    _linearKp = linear_map["kp"].toFloat();
    std::cout<< MRCConstants::defaultBold << "Linear Kp: "<< MRCConstants::green << _linearKp << std::endl<< MRCConstants::reset;

    _linearKi = linear_map["ki"].toFloat();
    std::cout<< MRCConstants::defaultBold << "Linear Ki: "<< MRCConstants::green << _linearKi << std::endl<< MRCConstants::reset;

    _linearKd = linear_map["kd"].toFloat();
    std::cout<< MRCConstants::defaultBold << "Linear Kd: "<< MRCConstants::green << _linearKd << std::endl<< MRCConstants::reset;

    // Angular
    QVariantMap angular_map = doc_map["AngularPID"].toMap();
    _angularKp = angular_map["kp"].toFloat();
    std::cout<< MRCConstants::defaultBold << "Angular Kp: "<< MRCConstants::green << _angularKp << std::endl<< MRCConstants::reset;

    _angularKi = angular_map["ki"].toFloat();
    std::cout<< MRCConstants::defaultBold << "Angular Ki: "<< MRCConstants::green << _angularKi << std::endl<< MRCConstants::reset;

    _angularKd = angular_map["kd"].toFloat();
    std::cout<< MRCConstants::defaultBold << "Angular Kd: "<< MRCConstants::green << _angularKd << std::endl<< MRCConstants::reset;

}

QString MRCConstants::getMLPPath() const
{
    return _MLPPath;
}

QString MRCConstants::getClustersPath() const
{
    return _clustersPath;
}

int MRCConstants::getThreadFrequency() const
{
    return _threadFrequency;
}

int MRCConstants::getGuiUpdateFrequency() const
{
    return _guiUpdateFrequency;
}

quint8 MRCConstants::getGoalieId() const
{
    return _goalieId;
}

float MRCConstants::getDistToConsiderBallMovement() const
{
    return _distToConsiderBallMovement;
}

float MRCConstants::getBallRadius() const
{
    return _ballRadius;
}

float MRCConstants::getRobotRadius() const
{
    return _robotRadius;
}

float MRCConstants::getMaxKickPower() const
{
    return _maxKickPower;
}

int MRCConstants::getQtPlayers() const
{
    return _qtPlayers;
}

float MRCConstants::getFPPSmoothPathResolution() const
{
    return _FPPSmoothPathResolution;
}

float MRCConstants::getFPPBreakDistance() const
{
    return _FPPBreakDistance;
}

float MRCConstants::getFPPRobotThreshHold() const
{
    return _FPPRobotThreshHold;
}

float MRCConstants::getFPPBallThreshHold() const
{
    return _FPPBallThreshHold;
}

float MRCConstants::getLinearKp() const
{
    return _linearKp;
}

float MRCConstants::getLinearKi() const
{
    return _linearKi;
}

float MRCConstants::getLinearKd() const
{
    return _linearKd;
}

float MRCConstants::getAngularKp() const
{
    return _angularKp;
}

float MRCConstants::getAngularKi() const
{
    return _angularKi;
}

float MRCConstants::getAngularKd() const
{
    return _angularKd;
}
