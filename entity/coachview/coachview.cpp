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

#include <entity/coachview/soccerview/soccerview.h>

#include <entity/contromodule/strategy/strategystate.h>
#include <entity/contromodule/playbook/playbook.h>

MRCTeam* CoachView::_ourTeam = NULL;
MRCTeam* CoachView::_theirTeam = NULL;
MainWindow* CoachView::_suassunaUI = NULL;
SSLReferee* CoachView::_ref = NULL;
Coach* CoachView::_coach = NULL;
bool CoachView::_isEnabled = false;

QString CoachView::name(){
    return "COACHVIEW";
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

CoachView::CoachView( MRCConstants *mrcconstants) : Entity(ENT_GUI)
{
    _mrcconstants = mrcconstants;
    _suassunaUI = new MainWindow(nullptr,mrcconstants);
    _suassunaUI->show();

    _timer.start();

    // half of the openGL application update
    

    // set as enabled
    _isEnabled = true;
    timeToUpdate = (1000.0 / getConstants()->getGuiUpdateFrequency()) / 2.0;
}

CoachView::~CoachView(){
    _suassunaUI->close();

    delete _suassunaUI;
}

void CoachView::drawTriangle(Position a, Position b, Position c, RGBA color){
    if(_isEnabled)
        _suassunaUI->drawTriangle(a, b, c, color);
}

void CoachView::drawLine(Position a, Position b, RGBA color){
    if(_isEnabled)
        _suassunaUI->drawLine(a, b, color);
}

void CoachView::initialization(){
   std::cout << MRCConstants::defaultBold << "[COACHVIEW] " << MRCConstants::green << "Thread started.\n" << MRCConstants::reset;
}

void CoachView::loop(){
    if(getConstants()==NULL) return;
    // Update GUI
    _suassunaUI->updateGUI(_ourTeam, _theirTeam, _ourTeam->loc());

    _timer.stop();
    if(_timer.timemsec() >= timeToUpdate){
        // process every ssl game info
        SSLGameInfo* _gameInfo = _ref->getGameInfo(_ourTeam->teamColor());

        if(_gameInfo->getColor() == _ourTeam->teamColor()){
            _suassunaUI->updateRefereeCommand(_gameInfo->refCommandToString(_gameInfo->command()).c_str());
            _suassunaUI->updateGameStage(_gameInfo->refStageToString(_gameInfo->stage()).c_str());
            Referee_TeamInfo theirTeamInfo = _gameInfo->theirTeamInfo();
            Referee_TeamInfo ourTeamInfo = _gameInfo->ourTeamInfo();
            if(_ourTeam->teamColor() == Colors::BLUE){
                _suassunaUI->updateScores(theirTeamInfo.score(), theirTeamInfo.yellow_cards(), theirTeamInfo.red_cards(), theirTeamInfo.timeouts(), ourTeamInfo.score(), ourTeamInfo.yellow_cards(), ourTeamInfo.red_cards(), ourTeamInfo.timeouts());
            }else{
                _suassunaUI->updateScores(ourTeamInfo.score(), ourTeamInfo.yellow_cards(), ourTeamInfo.red_cards(), ourTeamInfo.timeouts(), theirTeamInfo.score(), theirTeamInfo.yellow_cards(), theirTeamInfo.red_cards(), theirTeamInfo.timeouts());
            }
        }

        // process time left
        _suassunaUI->updateTimeLeft(_gameInfo->refTimeLeftToString().c_str());

        // process coach agressivity
        _suassunaUI->setAgressivity(_coach->getAgressivity());

        // process players avaliability
        QHash<quint8, Player*> ourPlayers = _ourTeam->avPlayers();
        for(quint8 x = 0; x <getConstants()->getQtPlayers(); x++){
            bool status = PlayerBus::ourPlayerAvailable(x);
            _suassunaUI->setRobotVisionStatus(x, status);
            if(status) _suassunaUI->setPlayerRole(x, PlayerBus::ourPlayer(x)->roleName());
        }

        // start timer for the next it
        _timer.start();
    }
}

void CoachView::updateTree(StrategyState *strat){
    // Process coach strategy, playbooks, roles and players
    QList<QString> playbookList;
    QMap<QString, QList<QString>> rolesList;
    QMap<std::pair<QString, QString>, QList<std::pair<QString, quint8>>> playersList;
    QMap<QString, QString> behavioursList;

    // Parsing strategy, playbook, roles, players and its actual behaviours
    if(strat != NULL){
        QString stratName = strat->name();
        QList<Playbook*> pbList = strat->getPlaybooks();
        QList<Playbook*>::iterator it;
        QList<Role*>::iterator it2;
        for(it = pbList.begin(); it != pbList.end(); it++){
            if((*it) == NULL) continue;
            if(!(*it)->isInitialized()) continue;
            QString playbookName = (*it)->name();
            playbookList.push_back(playbookName);
            QList<Role*> rList = (*it)->getRoles();
            for(it2 = rList.begin(); it2 != rList.end(); it2++){
                if((*it2) == NULL) continue;
                if(!(*it2)->isInitialized()) continue;
                QString roleName = (*it2)->name();
                rolesList[playbookName].push_back(roleName);
                if((*it2)->player() == NULL) continue;
                quint8 playerId = (*it2)->player()->playerId();
                std::string playerName = "Robot " + std::to_string(playerId);
                behavioursList[playerName.c_str()] = ((*it2)->getBehaviours()[(*it2)->getActualBehaviour()])->name();
                playersList[std::make_pair(playbookName, roleName)].push_back(std::make_pair(playerName.c_str(), playerId));
            }
        }
        _suassunaUI->resetTree(stratName, playbookList, rolesList, playersList, behavioursList);
    }
}

void CoachView::finalization(){
    std::cout << MRCConstants::defaultBold << "[COACHVIEW] " << MRCConstants::green << "Thread ended.\n" << MRCConstants::reset;
}


MRCConstants *CoachView::getConstants() {
    if(_mrcconstants==NULL)
        std::cout << MRCConstants::red << "[ERROR] " << MRCConstants::reset << name().toStdString() << ", requesting getConstants(), _mrcconstants not initialized!\n";
    return _mrcconstants;
}
