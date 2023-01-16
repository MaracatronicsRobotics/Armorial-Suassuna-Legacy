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

#include <Armorial/Utils/ExitHandler/ExitHandler.h>

#include <src/common/constants/constants.h>
#include <src/suassuna.h>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);

    // Setup command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    // Setup application options
    // Use gui
    QCommandLineOption useGuiOption("gui", "Enable GUI mode");
    parser.addOption(useGuiOption);

    // Process parser in application
    parser.process(app);

    // Setup ExitHandler
    Utils::ExitHandler::setApplication(&app);
    Utils::ExitHandler::setup();

    // Load constants
    Suassuna::Constants::loadFile(QString(PROJECT_PATH) + "/src/common/constants/constants.json");

    // Register metatypes
    qRegisterMetaType<QList<Armorial::Robot>>("QList<Armorial::Robot>");
    qRegisterMetaType<QList<Armorial::Ball>>("QList<Armorial::Ball>");

    // Start Suassuna core
    SuassunaCore *suassuna = new SuassunaCore();
    bool started = suassuna->start(parser.isSet(useGuiOption));

    // Check if started well
    if(started) {
        spdlog::info("Suassuna started succesfully.");
    }
    else {
        spdlog::error("Suassuna could not start properly, check previous log messages.");
        exit(-1);
    }

    // Hold exec
    bool exec = app.exec();

    // Check if stopped well
    bool stopped = suassuna->stop();
    if(stopped) {
        spdlog::info("Suassuna stopped succesfully.");
    }
    else {
        spdlog::error("Suassuna could not stop properly, check previous log messages.");
        exit(-1);
    }

    // Finish
    return exec;
}
