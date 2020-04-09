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

#include "field_ssl2014.hh"

using namespace Fields;

QString SSL2014::name() const {
    return "SSL 2014";
}

float SSL2014::length() const {
    return 6.05;
}

float SSL2014::width() const {
    return 4.05;
}

float SSL2014::goalWidth() const {
    return 0.70;
}

float SSL2014::goalDepth() const {
    return 0.20;
}

float SSL2014::centerRadius() const {
    return 0.50;
}

float SSL2014::defenseRadius() const {
    return 0.80;
}

float SSL2014::defenseStretch() const {
    return 0.35;
}
