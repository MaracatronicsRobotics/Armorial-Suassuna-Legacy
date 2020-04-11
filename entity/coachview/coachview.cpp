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
#include <entity/player/playerbus.h>
#include <const/constants.h>

QString CoachView::name(){
    return "Coach View";
}

void CoachView::setTeams(MRCTeam *our, MRCTeam *their){
    _ourTeam = our;
    _theirTeam = their;

    _suassunaUI->setupTeams(_ourTeam, _theirTeam);
}

void CoachView::setCoach(Coach *coach){
    _coach = coach;
}

void CoachView::setReferee(SSLReferee *ref){
    _ref = ref;
}

CoachView::CoachView() : Entity(ENT_GUI)
{
    _UIMutex = new QMutex();

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
    _UIMutex->lock();

    // process every ssl game info
    SSLGameInfo* _gameInfo = _ref->getGameInfo(_ourTeam->teamColor());

    if(_gameInfo->getColor() == _ourTeam->teamColor()){
        _suassunaUI->updateRefereeCommand(_gameInfo->refCommandToString(_gameInfo->command()).c_str());
        _suassunaUI->updateGameStage(_gameInfo->refStageToString(_gameInfo->stage()).c_str());
        SSL_Referee_TeamInfo theirTeamInfo = _gameInfo->theirTeamInfo();
        SSL_Referee_TeamInfo ourTeamInfo = _gameInfo->ourTeamInfo();
        if(_ourTeam->teamColor() == Colors::BLUE){
            _suassunaUI->updateScores(theirTeamInfo.score(), theirTeamInfo.yellow_cards(), theirTeamInfo.red_cards(), theirTeamInfo.timeouts(), ourTeamInfo.score(), ourTeamInfo.yellow_cards(), ourTeamInfo.red_cards(), ourTeamInfo.timeouts());
        }else{
            _suassunaUI->updateScores(ourTeamInfo.score(), ourTeamInfo.yellow_cards(), ourTeamInfo.red_cards(), ourTeamInfo.timeouts(), theirTeamInfo.score(), theirTeamInfo.yellow_cards(), theirTeamInfo.red_cards(), theirTeamInfo.timeouts());
        }
    }
    _suassunaUI->updateTimeLeft(_gameInfo->refTimeLeftToString().c_str());

    // process coach agressivity
    _suassunaUI->setAgressivity(_coach->getAgressivity());

    // process players avaliability
    QHash<quint8, Player*> ourPlayers = _ourTeam->avPlayers();
    for(quint8 x = 0; x < MRCConstants::_qtPlayers; x++){
        if(PlayerBus::ourPlayerAvailable(x)){
            _suassunaUI->enableRobot(x);
            _suassunaUI->setPlayerRole(x, ourPlayers[x]->getRoleName());
        }else{
            _suassunaUI->disableRobot(x);
        }
    }

    _UIMutex->unlock();
}

void CoachView::finalization(){
}
