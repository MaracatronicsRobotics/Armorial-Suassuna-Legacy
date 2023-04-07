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

#include "behavior_intercept.h"

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

Behavior_Intercept::Behavior_Intercept() {
    _interceptSegment = Geometry::LineSegment(Geometry::Vector2D(0.0f, 0.0f), Geometry::Vector2D(0.0f, 0.0f));
    _intersectionAccuracy = 1.0f;
}

void Behavior_Intercept::configure() {
    // Starting skills
    _skill_goTo = new Skill_GoTo();

    // Adding to behavior skill list
    addSkill(SKILL_GOTO, _skill_goTo);

    _objectPosition = getWorldMap()->getBall().getPosition();
    _objectVelocity = getWorldMap()->getBall().getVelocity();
    _interceptPosition = player()->getPosition();
}

void Behavior_Intercept::run() {
    // Check ball speed (maybe a error)
    if (_interceptSegment.isPoint()) {
        _interceptPosition = player()->getPosition();
    } else if (_objectVelocity.length() <= 0.01f) {
        _interceptPosition = _interceptSegment.project(_objectPosition);
    } else {
        Geometry::Vector2D pastObjPos = _objectPosition - _objectVelocity;
        Geometry::Vector2D futureObjPos = _objectPosition + _objectVelocity;
        Geometry::Line objectLine(_objectPosition, futureObjPos);
        std::optional<Geometry::Vector2D> intersection = objectLine.intersect(Geometry::Line(_interceptSegment));
        if (intersection.has_value()) {
            if ((_interceptSegment.distanceToPoint(futureObjPos)) > (_interceptSegment.distanceToPoint(pastObjPos))) {
                // Object moving away from interception segment
                _interceptPosition = _interceptSegment.project(_objectPosition);
            } else {
                _interceptPosition = intersection.value() * _intersectionAccuracy +
                        _interceptSegment.project(_objectPosition) * (1.0f - _intersectionAccuracy);
                if (!_interceptSegment.isOnLine(_interceptPosition)) {
                    // Interception reaches segment limit
                    _interceptPosition = _interceptSegment.project(_interceptPosition);
                }
            }
        } else {
            // Intersection segment and object movement are parallel: There is no interception
            _interceptPosition = _interceptSegment.project(_objectPosition);
        }
    }

    _skill_goTo->setTargetPosition(_interceptPosition);
    _skill_goTo->setGUI(gui());
    runSkill(SKILL_GOTO);
}

void Behavior_Intercept::setIntersectionAccuracy(float intersectionAccuracy) {
    intersectionAccuracy = std::min(1.0f, intersectionAccuracy);
    intersectionAccuracy = std::max(0.0f, intersectionAccuracy);
    _intersectionAccuracy = intersectionAccuracy;
}
