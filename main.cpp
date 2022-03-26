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

#include <src/entities/worldmap/worldmap.h>
#include <src/entities/referee/referee.h>

#include <src/entities/player/player.h>

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
    WorldMap *worldMap = new WorldMap(constants);
    worldMap->start();

    Utils::setConstants(constants);
    Utils::setWorldMap(worldMap);

    SSLReferee *referee = new SSLReferee(constants, worldMap);
    referee->start();

    Color TeamColor = Color();
    TeamColor.set_isblue(false);

    //Player *player0 = new Player(0, constants);
    //Player *player1 = new Player(1, constants);
    //Player *player2 = new Player(2, constants);
    //Player *player3 = new Player(3, constants);
    //Player *player4 = new Player(4, constants);
    Player *player5 = new Player(5, worldMap, referee, constants);

    //player0->start();
    //player1->start();
    //player2->start();
    //player3->start();
    //player4->start();
    player5->start();


    // Wait for application end
    bool exec = a->exec();

    //player0->stopEntity();
    //player0->wait();

    //player1->stopEntity();
    //player1->wait();
    //
    //player2->stopEntity();
    //player2->wait();
    //
    //player3->stopEntity();
    //player3->wait();
    //
    //player4->stopEntity();
    //player4->wait();
    //
    player5->stopEntity();
    player5->wait();

    referee->stopEntity();
    referee->wait();

    worldMap->stopEntity();
    worldMap->wait();

    return exec;
}
