/***
 * Warthog Robotics
 * University of Sao Paulo (USP) at Sao Carlos
 * http://www.warthog.sc.usp.br/
 *
 * This file is part of WRCoach project.
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

#include "instancechecker.h"
#include <QThread>
#include <iostream>
#include <stdio.h>
#include <QProcess>

bool InstanceChecker::isDuplicatedInstance(const QString &processName) {
    return (InstanceChecker::getInstances(processName)>1);
}

void InstanceChecker::waitIfDuplicated(const QString &processName) {
    if(InstanceChecker::isDuplicatedInstance(processName)) {
        std::cout << "[INSTANCE CHECKING] Attempting to open duplicated " << processName.toStdString() << " instance.\n";
        std::cout << "[INSTANCE CHECKING] Please close the other instance to continue!\n";
        std::cout << "[INSTANCE CHECKING] Waiting...";
        while(InstanceChecker::isDuplicatedInstance(processName)) {
            std::cout << ".";
            std::cout.flush();
            QThread::msleep(750);
        }
        std::cout << std::endl;
    }
}

unsigned InstanceChecker::getInstances(const QString &processName) {
    // Generate command
    QString cmd = "pidof "+processName;
    // Execute command
    QProcess *process = new QProcess();
    process->start(cmd);
    process->waitForFinished();
    // Return number of instances
    return process->readAll().split(' ').size();
}
