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

#include "constants.h"

int MRCConstants::_threadFrequency           = 60; // frequencia das threads criadas

/* Fast Path Planning */
float MRCConstants::_FPPBallThreshHold       = 0.2f;
float MRCConstants::_FPPRobotThreshHold      = 0.09f;
float MRCConstants::_FPPBreakDistance        = 0.42f;
float MRCConstants::_FPPSmoothPathResolution = 0.3f;

/* Soccer constants */
int MRCConstants::_qtPlayers                 = 6;

MRCConstants::MRCConstants()
{

}
