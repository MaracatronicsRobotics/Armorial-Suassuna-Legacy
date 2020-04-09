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

#include "coachview.h"
#include <entity/coachview/mainwindow.h>

#include <const/constants.h>

QString CoachView::name(){
    return "Coach View";
}

void CoachView::setTeams(MRCTeam *our, MRCTeam *their){
    _ourTeam = our;
    _theirTeam = their;

    _suassunaUI->setupTeams(_ourTeam, _theirTeam);
}

CoachView::CoachView() : Entity(ENT_GUI)
{
    _suassunaUI = new MainWindow();
    _suassunaUI->show();
}

MainWindow* CoachView::getUI(){
    return _suassunaUI;
}

CoachView::~CoachView(){
    _suassunaUI->close();
}

void CoachView::initialization(){
    this->setLoopFrequency(MRCConstants::threadFrequency());
}

void CoachView::loop(){

}

void CoachView::finalization(){

}
