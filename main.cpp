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
    cp.set_dribbling(true);

    ControlPacket cp2;
    cp2.set_w1(0.5);
    cp2.set_w2(0.5);
    cp2.set_w3(0.5);
    cp2.set_w4(0.5);

    QList<ControlPacket> cpList;
    cpList.push_back(cp);
    cpList.push_back(cp2);

    QList<ControlPacket> receiving;

    std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::SetControl() to be tested.") + '\n';
    actuator->SetControl(cp);

    std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::SetControls() to be tested.") + '\n';
    actuator->SetControls(cpList);

    std::cout << Text::green("[LOG] ", true) + Text::bold("ActuatorService::GetControls() to be tested.") + '\n';
    receiving = actuator->GetControls();

    // Wait for application end
    bool exec = a->exec();

    return exec;
}
