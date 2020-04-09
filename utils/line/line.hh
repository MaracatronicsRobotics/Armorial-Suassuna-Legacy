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

#ifndef LINE_H
#define LINE_H

#include <float.h>
#include <GEARSystem/Types/types.hh>

class Line {
private:
	float _a, _b;
public:
    Line();
	Line(float a, float b);

	float a() const { return _a; }
	float b() const { return _b; }

    Position interceptionWith(const Line &line);

    static Line getLine(const Position &pos, float angle) {
		float a = 0;
		if (angle > 2*GEARSystem::Angle::pi || angle < -2*GEARSystem::Angle::pi)
			angle = GEARSystem::Angle::toRadians(angle);
		if (angle == GEARSystem::Angle::pi/2 || angle == 3*GEARSystem::Angle::pi/2)
			a = FLT_MAX;
		else
			a = tan(angle);
		float b = pos.y() - (a * pos.x());
        return Line(a, b);
    }

	static Line getLine(const Position &pos0, const Position &pos1) {
		float a = 0;
        if (pos1.x() == pos0.x())
			a = DBL_MAX;
		else
            a = (pos1.y() - pos0.y())/(pos1.x() - pos0.x());
		float b = pos1.y() - (a * pos1.x());
		return Line(a, b);
	}
};

#endif // LINE_H
