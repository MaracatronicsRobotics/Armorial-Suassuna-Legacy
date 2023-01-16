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

#include "worldmap.h"

#include <spdlog/spdlog.h>

#include <src/common/constants/constants.h>
#include <src/common/utils/utils.h>
#include <src/entities/coach/team/team.h>

WorldMap::WorldMap() {
    // Setup default field
    _field = Common::Types::Field(Suassuna::Constants::teamPlaySide(), 0.25, 1.5, 1.3, 0.1, 0.4, 0.15, 0.7, 0.1125);
}

void WorldMap::setupTeams(QMap<Common::Enums::Color, Team*>& teams) {
    _teams = teams;
}

Common::Types::Field WorldMap::getField() {
    _mutex.lockForRead();
    Common::Types::Field field = _field;
    _mutex.unlock();

    return field;
}

Common::Types::Object WorldMap::getBall() {
    _mutex.lockForRead();
    Common::Types::Object ball = _ball;
    _mutex.unlock();

    return ball;
}

void WorldMap::updatePlayers(const QList<Armorial::Robot>& robots) {
    _mutex.lockForWrite();
    for(const auto& r : robots) {
        Common::Enums::Color teamColor = Suassuna::Utils::isColorBlue(r.robotidentifier().robotcolor()) ? Common::Enums::Color::BLUE : Common::Enums::Color::YELLOW;
        if(!r.robotposition().isinvalid()) {
            _teams[teamColor]->updatePlayer(r);
        }
    }
    _mutex.unlock();
}

void WorldMap::updateBall(const QList<Armorial::Ball>& balls) {
    _mutex.lockForWrite();
    if(balls.size()) {
        Armorial::Ball mostConfidentBall = balls.first();
        _ball = Common::Types::Object({mostConfidentBall.ballposition().x(), mostConfidentBall.ballposition().y()},
                                      {mostConfidentBall.ballvelocity().vx(), mostConfidentBall.ballvelocity().vy()},
                                      {mostConfidentBall.ballacceleration().ax(), mostConfidentBall.ballacceleration().ay()});
    }
    _mutex.unlock();
}

void WorldMap::updateField(const Common::Types::Field& field) {
    _mutex.lockForWrite();
    _field = Common::Types::Field(Suassuna::Constants::teamPlaySide(), field.centerRadius()/1000.0,
                                  field.length()/1000.0, field.width()/1000.0,
                                  field.goalDepth()/1000.0, field.goalWidth()/1000.0,
                                  field.penaltyDepth()/1000.0, field.penaltyWidth()/1000.0,
                                  field.penaltyMarkDistanceFromGoal()/1000.0);
    _mutex.unlock();
}
