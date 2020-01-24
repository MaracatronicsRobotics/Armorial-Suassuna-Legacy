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

#include "mrctimer.h"
#include <chrono>

MRCTimer::MRCTimer(double time){
    this->time = time;
    this->start = std::chrono::steady_clock::now();
}

void MRCTimer::update(){
    this->start = std::chrono::steady_clock::now();
}

void MRCTimer::setTime(double time){
    this->time = time;
}

bool MRCTimer::checkTimerEnd(){
    return (getTimeInMilliSeconds() > time);
}

double MRCTimer::getTimeInSeconds(){
    this->end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}

double MRCTimer::getTimeInMilliSeconds(){
    this->end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

double MRCTimer::getTimeInMicroSeconds(){
    this->end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

double MRCTimer::getTimeInNanoSeconds(){
    this->end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}
