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

#ifndef UTILS_HH
#define UTILS_HH

//#include <WRCoach/entity/controlmodule/coach/basecoach.hh>
#include <GEARSystem/gearsystem.hh>

namespace WR {

    class Utils {
	public:
		//static void initialize(WRTeam *ourTeam, WRTeam *opTeam);

        static Position threePoints(const Position &near, const Position &far, float distance, float beta);

        static float distance(const Position &a, const Position &b);

        static bool isPointAtLine(const Position &s1, const Position &s2, const Position &point);
        static bool isPointAtSegment(const Position &s1, const Position &s2, const Position &point);
        static Position projectPointAtLine(const Position &s1, const Position &s2, const Position &point);
        static Position projectPointAtSegment(const Position &s1, const Position &s2, const Position &point);
        static float distanceToLine(const Position &s1, const Position &s2, const Position &point);
        static float distanceToSegment(const Position &s1, const Position &s2, const Position &point);

        static float scalarProduct(const Position &A, const Position &B);
        static float crossProduct(const Position &A, const Position &B);
        static float getPerpendicularCoefficient(const Position &s1, const Position &s2);
        static Position hasInterceptionSegments(const Position &s1, const Position &s2, const Position &s3, const Position &s4);

        static float getAngle(const Position &a, const Position &b);
        static float angleDiff(const float A, const Angle &B);
        static float angleDiff(const Angle &A, const float B);
        static float angleDiff(const Angle &A, const Angle &B);
        static float angleDiff(const float A, const float B);
        static void angleLimitZeroTwoPi(float *angle);

        static bool checkInterval(double value, double minValue, double maxValue);
        static void limitValue(float *value, float minValue, float maxValue);
        static void limitMinValue(float *value, float minValue);
        static bool approximateToZero(float *value, float error = 1e-3);

        //static Position getPlayerKickDevice(quint8 team, quint8 id);
    };

}

#endif // UTILS_HH
