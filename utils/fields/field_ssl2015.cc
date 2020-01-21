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

#include "field_ssl2015.hh"

using namespace Fields;

QString SSL2015::name() const {
    return "SSL 2015";
}

float SSL2015::length() const {
    return 9.00;
}

float SSL2015::width() const {
    return 6.00;
}

float SSL2015::goalWidth() const {
    return 1.00;
}

float SSL2015::goalDepth() const {
    return 0.20;
}

float SSL2015::centerRadius() const {
    return 0.50;
}

float SSL2015::defenseRadius() const {
    return 1.00;
}

float SSL2015::defenseStretch() const {
    return 0.50;
}
