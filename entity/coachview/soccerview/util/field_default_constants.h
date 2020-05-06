//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    field_default_constants.h
  \brief   Definition of field dimensions
  \author  Stefan Zickler / Tim Laue, (C) 2009
*/
//========================================================================

#ifndef FIELD_DEFAULT_CONSTANTS_H
#define FIELD_DEFAULT_CONSTANTS_H

#include <vector>
#include "field.h"
#include "gvector.h"

namespace FieldConstantsLarc2020 {
    const double kFieldLength = 9000.0;
    const double kFieldWidth = 6000.0;
    const double kGoalWidth = 1000.0;
    const double kGoalDepth = 200.0;
    const double kBoundaryWidth = 500.0;

    // WR
    const double kCenterRadius = 500.0;
    const double kDefenseRadius = 1000.0;
    const double kDefenseStretch = 500.0;

    const double kLineThickness = 10.0;
    const double kXMax = (kFieldLength-2*kLineThickness)/2;
    const double kXMin = -kXMax;
    const double kYMax = (kFieldWidth-kLineThickness)/2;
    const double kYMin = -kYMax;

    const std::size_t kNumFieldLines = 8;
    const FieldLine kFieldLines[kNumFieldLines] = {
      FieldLine("TopTouchLine", kXMin-kLineThickness/2, kYMax, kXMax+kLineThickness/2, kYMax, kLineThickness),
      FieldLine("BottomTouchLine", kXMin-kLineThickness/2, kYMin, kXMax+kLineThickness/2, kYMin, kLineThickness),
      FieldLine("LeftGoalLine", kXMin, kYMin, kXMin, kYMax, kLineThickness),
      FieldLine("RightGoalLine", kXMax, kYMin, kXMax, kYMax, kLineThickness),
      FieldLine("HalfwayLine", 0, kYMin, 0, kYMax, kLineThickness),
      FieldLine("CenterLine", kXMin, 0, kXMax, 0, kLineThickness),
      FieldLine("LeftPenaltyStretch", kXMin+kDefenseRadius, -kDefenseStretch/2, kXMin+kDefenseRadius, kDefenseStretch/2, kLineThickness),
      FieldLine("RightPenaltyStretch", kXMax-kDefenseRadius, -kDefenseStretch/2, kXMax-kDefenseRadius, kDefenseStretch/2, kLineThickness),
    };

    const std::size_t kNumFieldArcs = 5;
    const FieldCircularArc kFieldArcs[kNumFieldArcs] = {
      FieldCircularArc("LeftFieldLeftPenaltyArc",   kXMin, kDefenseStretch/2,  kDefenseRadius-kLineThickness/2, 0,        M_PI/2,   kLineThickness),
      FieldCircularArc("LeftFieldRightPenaltyArc",  kXMin, -kDefenseStretch/2, kDefenseRadius-kLineThickness/2, 1.5*M_PI, 2*M_PI,   kLineThickness),
      FieldCircularArc("RightFieldLeftPenaltyArc",  kXMax, -kDefenseStretch/2, kDefenseRadius-kLineThickness/2, M_PI,     1.5*M_PI, kLineThickness),
      FieldCircularArc("RightFieldRightPenaltyArc", kXMax, kDefenseStretch/2,  kDefenseRadius-kLineThickness/2, M_PI/2,   M_PI,     kLineThickness),
      FieldCircularArc("CenterCircle",              0,     0,                  kCenterRadius-kLineThickness/2,  0,        2*M_PI,   kLineThickness)
    };
}

#endif // FIELD_DEFAULT_CONSTANTS_H
