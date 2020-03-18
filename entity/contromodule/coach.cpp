#include "coach.h"

#include <entity/contromodule/mrcteam.h>
#include <entity/player/playerbus.h>
#include <entity/contromodule/strategy/strategy.h>
#include <entity/contromodule/coachutils.h>

#include <entity/coachview/mainwindow.h>

QString Coach::name(){
    std::cout << "Coach" << std::endl;
}

Coach::Coach(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam, CoachView *ourGUI)
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

    // suassuna ui
    _ourGUI = ourGUI;

    // debug
    _updateRoles = true;
}

Coach::~Coach(){
    if(_strat != NULL)
        delete _strat;

    delete _utils;
}

std::vector<double> Coach::getEnemyProportionsInAreas(){
    std::vector<double> proportions;
    for(int x = 0; x < 3; x++) proportions.push_back(0.0);

    if(_ourTeam->fieldSide().isLeft()){
        QList<Player*> enemyPlayers = _theirTeam->avPlayers().values();
        QList<Player*>::iterator it;
        for(it = enemyPlayers.begin(); it != enemyPlayers.end(); it++){
            if((*it)->position().x() <= 0.0 && (*it)->position().x() >= -1.5){
                proportions[0] += 1.0;
            }
            else if((*it)->position().x() < 1.5 && (*it)->position().x() >= -3.0){
                proportions[1] += 1.0;
            }
            else if((*it)->position().x() < 3.0 && (*it)->position().x() >= -4.5){
                proportions[2] += 1.0;
            }
        }
    }
    else{
        QList<Player*> enemyPlayers = _theirTeam->avPlayers().values();
        QList<Player*>::iterator it;
        for(it = enemyPlayers.begin(); it != enemyPlayers.end(); it++){
            if((*it)->position().x() >= 0.0 && (*it)->position().x() <= 1.5){
                proportions[0] += 1.0;
            }
            else if((*it)->position().x() > 1.5 && (*it)->position().x() <= 3.0){
                proportions[1] += 1.0;
            }
            else if((*it)->position().x() > 3.0 && (*it)->position().x() <= 4.5){
                proportions[2] += 1.0;
            }
        }
    }

    for(int x = 0; x < 3; x++){
        proportions[x] = proportions[x] / _theirTeam->avPlayers().values().size();
    }

    return proportions;
}

void Coach::loadClusters(){
    QString settings;
    QFile file;
    file.setFileName("../constraints/agressivity_clusters.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(settings.toUtf8());
    QJsonObject sett2 = sd.object();

    QStringList keys = sett2.keys();
    for(int x = 0; x < keys.size(); x++){
        QJsonObject structure = (sett2.value(keys.at(x))).toObject();
        QJsonArray data = structure.take("clusters").toArray();
        for(int y = 0; y < data.size(); y++){
            QJsonValue parse_data = data.at(y);
            QJsonArray parse_data_arr = parse_data.toObject().value(QString("values")).toArray();
            std::vector<double> vec_aux;
            for(int z = 0; z < parse_data_arr.size(); z++){
                vec_aux.push_back(parse_data_arr.at(z).toDouble());
            }
            _agressivityClusters.insert(keys.at(x).toStdString(), vec_aux);
        }
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

std::string Coach::calculateAgressivity(std::vector<double> &distributions){
    QHash<std::string, std::vector<double>>::iterator it;
    std::string best_ans = "";
    double dist = 1e9;
    for(it = _agressivityClusters.begin(); it != _agressivityClusters.end(); it++){
        double dist_now = 0.0;
        for(int x = 0; x < distributions.size(); x++){
            dist_now += pow((distributions.at(x) - it.value().at(x)), 2);
        }
        dist_now = sqrt(dist_now);
        if(dist_now > 0.2){
            std::cout << "[OUTLIER] ";
            for(int x = 0; x < distributions.size(); x++) std::cout << distributions[x] << " ";
            std::cout << std::endl;
        }
        if(dist_now < dist){
            dist = dist_now;
            best_ans = it.key();
        }
    }

    return best_ans;
}

void Coach::run(){
    if(_ourTeam->avPlayersSize() == 0){
        std::cout << "[COACH] No players available!" << std::endl;
        return ;
    }

    std::vector<double> proportions = getEnemyProportionsInAreas();
    std::string agressivity = calculateAgressivity(proportions);
    if(agressivity != _lastAgressivity){
        _lastAgressivity = agressivity;
        std::cout << "[AGRESSIVITY] " << _lastAgressivity << "\n";
        for(int x = 0; x < 3; x++) std::cout << proportions[x] << " ";
        std::cout << "\n";
    }


    // get strategy
    Strategy *strat = strategy();

    // run strategy
    if(strat != NULL){
        if(strat->isInitialized() == false){
            strat->initialize(_ref, _ourTeam, _theirTeam, _utils);
        }
        strat->runStrategy();
        if(_updateRoles){
            for(int x = 0; x < _ourTeam->avPlayersSize(); x++){
                _ourGUI->getUI()->setPlayerRole((int) _ourTeam->avPlayers().values().at(x)->playerId(), _ourTeam->avPlayers().values().at(x)->getRoleName());
            }
            _updateRoles = false;
        }
    }

}

void Coach::setStrategy(Strategy *strat){
    _mutexStrategy.lock();

    // deleting old strategy
    if(_strat != NULL){
        delete _strat;
    }

    //setting new
    _strat = strat;
    _updateRoles = true;

    _mutexStrategy.unlock();
}

Strategy* Coach::strategy(){
    _mutexStrategy.lock();
    Strategy *s = _strat;
    _mutexStrategy.unlock();

    return s;
}
