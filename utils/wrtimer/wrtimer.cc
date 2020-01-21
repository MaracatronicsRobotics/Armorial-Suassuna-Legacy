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

#include "wrtimer.hh"

WRTimer::WRTimer() {
    clock_gettime(CLOCK_REALTIME, &_time1);
    clock_gettime(CLOCK_REALTIME, &_time2);
}

void WRTimer::start() {
    clock_gettime(CLOCK_REALTIME, &_time1);
}

void WRTimer::stop() {
    clock_gettime(CLOCK_REALTIME, &_time2);
}

double WRTimer::timesec() {
    return timensec()/1E9;
}

double WRTimer::timemsec()   {
    return timensec()/1E6;
}

double WRTimer::timeusec()   {
    return timensec()/1E3;
}

double WRTimer::timensec()   {
    return (_time2.tv_sec*1E9 + _time2.tv_nsec) - (_time1.tv_sec*1E9 + _time1.tv_nsec);
}
