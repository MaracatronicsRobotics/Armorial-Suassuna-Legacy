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

#include "field_ssl2020.hh"

using namespace Fields;

QString SSL2020::name() const {
    return "SSL 2020";
}

float SSL2020::length() const {
    return 9.00;
}

float SSL2020::width() const {
    return 6.00;
}

float SSL2020::goalWidth() const {
    return 1.00;
}

float SSL2020::goalDepth() const {
    return 0.20;
}

float SSL2020::areaWidth() const {
    return 2.00;
}

float SSL2020::areaDepth() const {
    return 1.00;
}

float SSL2020::centerRadius() const {
    return 0.50;
}

float SSL2020::defenseRadius() const {
    return 1.50;
}

float SSL2020::defenseStretch() const {
    return 0.50;
}
