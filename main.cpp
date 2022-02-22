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

    // Wait for application end
    bool exec = a->exec();

    return exec;
}
