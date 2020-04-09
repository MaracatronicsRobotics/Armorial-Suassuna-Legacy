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

#include "line.hh"

Line::Line() {
    _a = 0;
    _b = 0;
}

Line::Line(float a, float b) {
    _a = a;
    _b = b;
}

Position Line::interceptionWith(const Line &line) {
    float x = (line.b()-_b) / (_a-line.a());
    float y = _a*x + _b;
    return Position(true, x, y, 0.0);
}
