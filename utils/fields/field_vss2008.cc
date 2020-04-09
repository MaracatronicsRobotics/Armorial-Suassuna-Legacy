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

#include "field_vss2008.hh"

using namespace Fields;

QString VSS2008::name() const {
    return "VSS 2008";
}

float VSS2008::length() const {
    return 1.50;
}

float VSS2008::width() const {
    return 1.30;
}

float VSS2008::goalWidth() const {
    return 0.40;
}

float VSS2008::goalDepth() const {
    return 0.10;
}

float VSS2008::centerRadius() const {
    return 0.20;
}

float VSS2008::defenseRadius() const {
    return 0.15;
}

float VSS2008::defenseStretch() const {
    return 0.40;
}
