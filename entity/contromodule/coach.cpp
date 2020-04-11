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

#include "coach.h"

#include <entity/contromodule/mrcteam.h>
#include <entity/player/playerbus.h>
#include <entity/contromodule/strategy/strategy.h>
#include <entity/contromodule/coachutils.h>

#include <entity/coachview/mainwindow.h>

QString Coach::name(){
    std::cout << "Coach" << std::endl;
}

Coach::Coach(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam)
{
    _ref = ref;
    _ourTeam = ourTeam;
    _theirTeam = theirTeam;

    // Initialize PlayerBus
    PlayerBus::initialize(ourTeam, theirTeam);

    // Load agressivity constraints
    loadClusters();
    _lastAgressivity = "";

    // Coach utils
    _utils = new CoachUtils(ourTeam);

    // null strat
    _strat = NULL;
}

Coach::~Coach(){
    if(_strat != NULL)
        delete _strat;

    delete _utils;
}

std::vector<double> Coach::getEnemyGaussiansInAreas(){
    std::vector<double> proportions;
    for(int x = 0; x < 5; x++) proportions.push_back(0.0);

    if(_ourTeam->fieldSide().isLeft()){
        QList<Player*> enemyPlayers = _theirTeam->avPlayers().values();
        QList<Player*>::iterator it;
        for(it = enemyPlayers.begin(); it != enemyPlayers.end(); it++){
            if((*it)->position().x() >= -4.5 && (*it)->position().x() <= -3.0){
                proportions[0] += 1.0;
            }
            else if((*it)->position().x() > -3.0 && (*it)->position().x() <= -1.5){
                proportions[1] += 1.0;
            }
            else if((*it)->position().x() > -1.5 && (*it)->position().x() <= 1.5){
                proportions[2] += 1.0;
            }
            else if((*it)->position().x() > 1.5 && (*it)->position().x() <= 3.0){
                proportions[3] += 1.0;
            }
            else if((*it)->position().x() > 3.0 && (*it)->position().x() <= 4.5){
                proportions[4] += 1.0;
            }
        }
    }
    else{
        QList<Player*> enemyPlayers = _theirTeam->avPlayers().values();
        QList<Player*>::iterator it;
        for(it = enemyPlayers.begin(); it != enemyPlayers.end(); it++){
            if((*it)->position().x() >= -4.5 && (*it)->position().x() <= -3.0){
                proportions[4] += 1.0;
            }
            else if((*it)->position().x() > -3.0 && (*it)->position().x() <= -1.5){
                proportions[3] += 1.0;
            }
            else if((*it)->position().x() > -1.5 && (*it)->position().x() <= 1.5){
                proportions[2] += 1.0;
            }
            else if((*it)->position().x() > 1.5 && (*it)->position().x() <= 3.0){
                proportions[1] += 1.0;
            }
            else if((*it)->position().x() > 3.0 && (*it)->position().x() <= 4.5){
                proportions[0] += 1.0;
            }
        }
    }

    int locBall = -1;
    if(_ourTeam->fieldSide().isLeft()){
        if(_ourTeam->loc()->ball().x() >= -4.5 &&_ourTeam->loc()->ball().x() <= -3.0){
            locBall = 1;
        }
        else if(_ourTeam->loc()->ball().x() > -3.0 && _ourTeam->loc()->ball().x() <= -1.5){
            locBall = 2;
        }
        else if(_ourTeam->loc()->ball().x() > -1.5 && _ourTeam->loc()->ball().x() <= 1.5){
            locBall = 3;
        }
        else if(_ourTeam->loc()->ball().x() > 1.5 && _ourTeam->loc()->ball().x() <= 3.0){
            locBall = 4;
        }
        else if(_ourTeam->loc()->ball().x() > 3.0 && _ourTeam->loc()->ball().x() <= 4.5){
            locBall = 5;
        }
    }
    else{
        if(_ourTeam->loc()->ball().x() >= -4.5 &&_ourTeam->loc()->ball().x() <= -3.0){
            locBall = 5;
        }
        else if(_ourTeam->loc()->ball().x() > -3.0 && _ourTeam->loc()->ball().x() <= -1.5){
            locBall = 4;
        }
        else if(_ourTeam->loc()->ball().x() > -1.5 && _ourTeam->loc()->ball().x() <= 1.5){
            locBall = 3;
        }
        else if(_ourTeam->loc()->ball().x() > 1.5 && _ourTeam->loc()->ball().x() <= 3.0){
            locBall = 2;
        }
        else if(_ourTeam->loc()->ball().x() > 3.0 && _ourTeam->loc()->ball().x() <= 4.5){
            locBall = 1;
        }
    }

    for(int x = 0; x < 5; x++){
        if(locBall == -1) break; // vision error?

        double sigma = 1.0;
        double mi = locBall;
        double delta_x = (x + 1);

        double t1 = (1.0 / (sigma * sqrt(2.0 * M_PI)));
        double t2_up = (-1.0/2.0) * pow(((delta_x - mi)/sigma), 2);
        double t2_f = exp(t2_up);
        double gauss = (t1 * t2_f);

        proportions[x] = proportions[x] * gauss;
    }

    return proportions;
}

void Coach::loadClusters(){
    QString settings;
    QFile file;
    file.setFileName("../const/agressivity_clusters.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(settings.toUtf8());
    QJsonObject sett2 = sd.object();

    QStringList keys = sett2.keys();
    for(int x = 0; x < keys.size(); x++){
        QJsonObject structure = (sett2.value(keys.at(x))).toObject();
        QJsonArray data = structure.take("clusters").toArray();
        std::vector<std::vector<double>> all_vec;
        for(int y = 0; y < data.size(); y++){
            QJsonValue parse_data = data.at(y);
            QJsonArray parse_data_arr = parse_data.toObject().value(QString("values")).toArray();
            std::vector<double> vec_aux;
            for(int z = 0; z < parse_data_arr.size(); z++){
                vec_aux.push_back(parse_data_arr.at(z).toDouble());
            }
            all_vec.push_back(vec_aux);
        }
        _agressivityClusters.insert(keys.at(x).toStdString(), all_vec);
    }

    std::cout << "[COACH] Agressivity clusters loaded." << std::endl;
}

namespace std
{
  int inline qHash(const std::string& key, uint seed = 0)
  {
    return qHash(QByteArray::fromRawData(key.data(), key.length()), seed);
  }
}

std::string Coach::calculateAgressivity(std::vector<double> &gaussians){
    QHash<std::string, std::vector<std::vector<double>>>::iterator it;
    std::string best_ans = "";
    double dist = 1e9;
    for(it = _agressivityClusters.begin(); it != _agressivityClusters.end(); it++){
        double dist_now = 0.0;
        for(int x = 0; x < it.value().size(); x++){
            dist_now = 0;
            for(int y = 0; y < gaussians.size(); y++){
                dist_now += pow((gaussians.at(y) - it.value().at(x).at(y)), 2);
            }
            dist_now = sqrt(dist_now);
            if(dist_now < dist){
                dist = dist_now;
                best_ans = it.key();
            }
        }
    }

    return best_ans;
}

void Coach::run(){
    if(_ourTeam->avPlayersSize() == 0){
        std::cout << "[COACH] No players available!" << std::endl;
        return ;
    }

    std::vector<double> gaussians = getEnemyGaussiansInAreas();
    std::string agressivity = calculateAgressivity(gaussians);

    if(agressivity != _lastAgressivity){
        _lastAgressivity = agressivity;
    }


    // get strategy
    Strategy *strat = strategy();

    // run strategy
    if(strat != NULL){
        if(strat->isInitialized() == false){
            strat->initialize(_ref, _ourTeam, _theirTeam, _utils);
        }
        strat->runStrategy();
    }

}

QString Coach::getAgressivity(){
    return _lastAgressivity.c_str();
}

void Coach::setStrategy(Strategy *strat){
    _mutexStrategy.lock();

    // deleting old strategy
    if(_strat != NULL){
        delete _strat;
    }

    //setting new
    _strat = strat;

    _mutexStrategy.unlock();
}

Strategy* Coach::strategy(){
    _mutexStrategy.lock();
    Strategy *s = _strat;
    _mutexStrategy.unlock();

    return s;
}
