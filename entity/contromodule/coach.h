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
};

#endif // CONTROLLER_H
