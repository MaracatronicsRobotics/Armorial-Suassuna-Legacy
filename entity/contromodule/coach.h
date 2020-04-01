#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <entity/contromodule/basecoach.h>
#include <entity/contromodule/controlmodule.h>
#include <entity/coachview/coachview.h>

class Coach : public ControlModule
{
public:
    Coach(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam, CoachView *ourGUI);
    virtual ~Coach();
    QString name();

    void setStrategy(Strategy *strat);
private:
    // run controller
    void run();

    // Game info
    SSLReferee *_ref;
    CoachUtils *_utils;

    // Teams
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;

    // Strategy
    QMutex _mutexStrategy;
    Strategy *_strat;
    Strategy* strategy();

    // Suassuna UI
    CoachView *_ourGUI;
    bool _updateRoles;

    // Agressivity
    QHash<std::string, std::vector<std::vector<double>>> _agressivityClusters;
    void loadClusters();
    std::string calculateAgressivity(std::vector<double> &gaussians);
    std::vector<double> getEnemyGaussiansInAreas();
    std::string _lastAgressivity;

    std::vector<double> proportionsOld;
};

#endif // CONTROLLER_H
