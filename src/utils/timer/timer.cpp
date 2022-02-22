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

#include "timer.h"

Timer::Timer() {
    // Updating time1 and time2 with actual time
    clock_gettime(CLOCK_REALTIME, &_time1);
    clock_gettime(CLOCK_REALTIME, &_time2);
}

void Timer::start() {
    // Updating time1 with last time
    clock_gettime(CLOCK_REALTIME, &_time1);
}

void Timer::stop() {
    // Updating time2 with last time
    clock_gettime(CLOCK_REALTIME, &_time2);
}

double Timer::getSeconds() {
    return (getNanoSeconds() / 1E9);
}

double Timer::getMiliSeconds() {
    return (getNanoSeconds() / 1E6);
}

double Timer::getMicroSeconds() {
    return (getNanoSeconds() / 1E3);
}

double Timer::getNanoSeconds() {
    // Get time2 - time1
    return ((_time2.tv_sec * 1E9) + _time2.tv_nsec) - ((_time1.tv_sec * 1E9) + _time1.tv_nsec);
}
