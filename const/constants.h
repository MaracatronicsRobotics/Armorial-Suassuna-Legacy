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

#ifndef CONSTANTS_H
#define CONSTANTS_H

class MRCConstants
{
private:
    static int _threadFrequency;
public:
    MRCConstants();

    static int threadFrequency() { return _threadFrequency; }

    /* Fast Path Planning */
    static float _FPPBallThreshHold;
    static float _FPPRobotThreshHold;
    static float _FPPBreakDistance;
    static float _FPPSmoothPathResolution;

    /* Soccer constants */
    static int _qtPlayers;
};

#endif // CONSTANTS_H
