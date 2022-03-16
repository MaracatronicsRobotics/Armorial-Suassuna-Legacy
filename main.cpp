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

#include <QApplication>
#include <QCommandLineParser>
#include <spdlog/spdlog.h>

#include <src/exithandler/exithandler.h>
#include <src/constants/constants.h>
#include <src/entities/entity.h>
#include <src/services/actuator/actuatorservice.h>
#include <src/services/coach/coachservice.h>
#include <src/utils/text/text.h>
#include <src/utils/timer/timer.h>

QCoreApplication *createApplication(int &argc, char *argv[]) {
    // Try to found in args an '--gui'
    bool foundArg = false;
    for (int i = 0; i < argc; ++i) {
        if (!qstrcmp(argv[i], "--gui")) {
            foundArg = true;
            break;
        }
    }

    // if not found, call core application
    if(!foundArg) {
        return new QCoreApplication(argc, argv);
    }
    // otherwise, call gui application
    else {
        return new QApplication(argc, argv);
    }
}

int main(int argc, char *argv[]){
    spdlog::enable_backtrace(32);

    QScopedPointer<QCoreApplication> a(createApplication(argc, argv));
    a->setApplicationName("Armorial Suassuna");
    a->setApplicationVersion("2.0.0");

    // Setup command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    // Setup application options
    // Use gui
    QCommandLineOption useGuiOption("gui", "Enable GUI mode");
    parser.addOption(useGuiOption);

    // Process parser in application
    parser.process(*a);

    // Setup ExitHandler
    ExitHandler::setApplication(a.data());
    ExitHandler::setup();

    Constants *constants = new Constants(QString(PROJECT_PATH) + "/src/constants/constants.json");

    ActuatorService *actuator = new ActuatorService(constants);
    CoachService *coach = new CoachService(constants);

    Ball ball = coach->getBall();
    Field field = coach->getField();

    ControlPacket *cp;

    cp = actuator->setVelocity(0, false, 1.0f, 0.0f, 0.0f);

    ControlPacket *cp2;
    cp2 = actuator->setDrible(1, false, true);

    ControlPacket *cp3;
    cp3 = actuator->setAngularSpeed(3, false, 1.0f, false);

    ControlPacket *cp4;
    cp4 = actuator->setVelocity(2, false, 1.5f, 0.0f, 0.0f);

    ControlPacket *cp5;
    cp5 = actuator->setVelocity(4, false, 0.5f, 0.0f, 0.0f);

    QList<ControlPacket> cpList;
    //cpList.push_back(*cp);
    cpList.push_back(*cp2);
    cpList.push_back(*cp3);
    cpList.push_back(*cp4);
    cpList.push_back(*cp5);

    QList<ControlPacket> receiving;


    spdlog::info(Text::bold("ActuatorService::SetControl() to be tested."));

    spdlog::info(Text::bold("ControlPacket: ") +
                 Text::bold("Robot ID: " + std::to_string(cp->robotidentifier().robotid())) + " " +
                 Text::bold("Robot Color: Yellow"));
    actuator->SetControl(*cp);

    spdlog::info(Text::bold("ActuatorService::SetControls() to be tested."));
    for (ControlPacket packet : cpList) {
        spdlog::info(Text::bold("ControlPacket: ") +
                     Text::bold("Robot ID: " + std::to_string(packet.robotidentifier().robotid())) + " " +
                     Text::bold("Robot Color: Yellow"));
    }
    actuator->SetControls(cpList);

    spdlog::info(Text::bold("ActuatorService::GetControls() to be tested."));
    receiving = actuator->GetControls();

    //std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::SetControl() to be tested. #2") + '\n';
    //actuator->SetControl(cp2);

    if (receiving.size() > 0) {
        spdlog::warn(Text::bold("Receiving list size: " + std::to_string(receiving.size())));
        while (receiving.size() > 0) {
            actuator->SetControls(receiving);
            receiving = actuator->GetControls();
            if (receiving.size() > 0) {
                spdlog::warn(Text::bold("Receiving list size: " + std::to_string(receiving.size())));
            } else {
                spdlog::info(Text::bold("Receiving list size: " + std::to_string(receiving.size())));
            }
        }
    } else {
        std::cout << Text::green("[LOG] ", true) + Text::bold("Receiving list size: " + std::to_string(receiving.size())) + "\n";
    }

    //TEST: COACH
    Color *color = new Color();
    color->set_isblue(false);

    QList<Robot> robots = coach->getRobots(*color);

    std::string teamColor;
    if (color->isblue()) {
        teamColor = Text::blue("blue");
    } else {
        teamColor = Text::yellow("yellow");
    }

    for (Robot r : robots) {
        std::cout << Text::green("[LOG] ", true) + Text::bold("Robot #" + std::to_string(r.robotidentifier().robotid()) + " of team ")
                  << teamColor
                  << Text::bold(" is positioned in coordinates: ")
                  << Text::blue("( " + std::to_string(r.robotposition().x()) + ", " + std::to_string(r.robotposition().y()) + ")", true) + "\n";
    }

    // Wait for application end
    bool exec = a->exec();

    return exec;
}
