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

#include "behaviour_markball.h"

QString Behaviour_MarkBall::name() {
    return "Behaviour_Marker";
}

Behaviour_MarkBall::Behaviour_MarkBall() {
    _mrcconstants=getConstants();
    _sk_GoToLookTo = NULL;
}

void Behaviour_MarkBall::configure() {
    usesSkill(_sk_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_MarkBall::run() {
    if(player()->distBall() <= 0.2f)
        player()->kick(_mrcconstants->getMaxKickPower());

    Position desired = WR::Utils::threePoints(loc()->ball(), player()->position(), 0.15f, GEARSystem::Angle::pi);
    _sk_GoToLookTo->setDesiredPosition(desired);
    _sk_GoToLookTo->setAvoidBall(false);
    _sk_GoToLookTo->setAvoidOpponents(false);
    for (int id = 0; id < _mrcconstants->getQtPlayers() ; id++) {
        if(PlayerBus::theirPlayerAvailable(id)){
            if (PlayerBus::theirPlayer(id)->hasBallPossession()) {
                if (PlayerBus::theirPlayer(id)->orientation().value() < 0 && PlayerBus::theirPlayer(id)->orientation().value() > -3.141592654)
                    _sk_GoToLookTo->setAimPosition(PlayerBus::theirPlayer(id)->position());

                break;
            }
        }
        else _sk_GoToLookTo->setAimPosition(loc()->ball());
    }
}

