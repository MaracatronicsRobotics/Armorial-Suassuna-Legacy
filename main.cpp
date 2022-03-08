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

    ControlPacket cp;

    cp = actuator->setVelocity(0, false, 1.0f, 0.0f, 0.0f);

    ControlPacket cp2;
    cp2 = actuator->setDrible(1, false, true);

    ControlPacket cp3;
    cp3 = actuator->setAngularSpeed(3, false, 1.0f, false);

    ControlPacket cp4;
    cp4 = actuator->setVelocity(2, false, 1.5f, 0.0f, 0.0f);

    QList<ControlPacket> cpList;
    cpList.push_back(cp);
    cpList.push_back(cp2);
    cpList.push_back(cp3);
    cpList.push_back(cp4);

    QList<ControlPacket> receiving;

    //std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::SetControl() to be tested.") + '\n';
    //std::cout << Text::green("[LOG] ", true) + Text::bold("ControlPacket: ")
    //          << Text::bold("Robot ID: " + std::to_string(cp.robotidentifier().robotid())) + " "
    //          << Text::bold("Robot Color: Yellow") + " \n";
    //actuator->SetControl(cp);

    std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::SetControls() to be tested.") + '\n';
    actuator->SetControls(cpList);

    std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::GetControls() to be tested.") + '\n';
    receiving = actuator->GetControls();

    //std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::SetControl() to be tested. #2") + '\n';
    //actuator->SetControl(cp2);

    if (receiving.size() > 0) {
        std::cout << Text::yellow("[LOG] ", true) + Text::bold("Receiving list size: " + std::to_string(receiving.size())) + "\n";
        while (receiving.size() > 0) {
            actuator->SetControls(receiving);
            receiving = actuator->GetControls();
            if (receiving.size() > 0) {
                std::cout << Text::yellow("[LOG] ", true) + Text::bold("Receiving list size: " + std::to_string(receiving.size())) + "\n";
            } else {
                std::cout << Text::green("[LOG] ", true) + Text::bold("Receiving list size: " + std::to_string(receiving.size())) + "\n";
            }
        }
    } else {
        std::cout << Text::green("[LOG] ", true) + Text::bold("Receiving list size: " + std::to_string(receiving.size())) + "\n";
    }


    //TEST: COACH
    Color *color = new Color();
    color->set_isblue(true);

    RobotIdentifier robotID;
    robotID.set_robotid(2);
    robotID.set_allocated_robotcolor(color);

    Robot r2d2 = coach->getRobot(robotID);
    QList<Robot> robots = coach->getRobots(*color);



    // Wait for application end
    bool exec = a->exec();

    return exec;
}
