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

    /* tudo aqui eh pra debug! */
    bool vai = false;
    for(int x = 0; x < gaussians.size(); x++){
        if(proportionsOld.size() == 0){
            vai = true;
            break;
        }
        if(gaussians[x] != proportionsOld[x]) vai = true;
    }

    proportionsOld = gaussians;

    if(vai){

        /* debugando no file */
        FILE *arq = fopen("tests.txt", "a+");
        int locBall;
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

        char str[500];
        for(int x = 0; x < agressivity.size(); x++){
            str[x] = agressivity[x];
            str[x+1] = '\0';
        }


        fprintf(arq, "[%.2lf , %.2lf , %.2lf , %.2lf , %.2lf], %d  -  %s\n", gaussians[0], gaussians[1], gaussians[2], gaussians[3], gaussians[4], locBall, str);
        fclose(arq);
        /* debugando no file */

        _lastAgressivity = agressivity;

        std::cout << "[COACH] Agressivity update to: [ " << agressivity << " ]" <<std::endl;
        std::cout << "Our high area: "     << gaussians[0] << std::endl;
        std::cout << "Our medium area: "   << gaussians[1] << std::endl;
        std::cout << "The midfield: "      << gaussians[2] << std::endl;
        std::cout << "Their medium area: " << gaussians[3] << std::endl;
        std::cout << "Their high area: "   << gaussians[4] << std::endl;

        std::cout << std::endl;
    }

    /* tudo aqui eh pra debug! */

    /*
    if(agressivity != _lastAgressivity){
        _lastAgressivity = agressivity;

        std::cout << "[COACH] Agressivity update to: [ " << agressivity << " ]" <<std::endl;
        std::cout << "Our high area: "     << gaussians[0] << std::endl;
        std::cout << "Our medium area: "   << gaussians[1] << std::endl;
        std::cout << "The midfield: "      << gaussians[2] << std::endl;
        std::cout << "Their medium area: " << gaussians[3] << std::endl;
        std::cout << "Their high area: "   << gaussians[4] << std::endl;

        std::cout << std::endl;
    }
    */

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
