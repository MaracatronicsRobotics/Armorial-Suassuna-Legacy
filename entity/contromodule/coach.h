#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <entity/contromodule/basecoach.h>
#include <entity/contromodule/controlmodule.h>

class Coach : public ControlModule
{
public:
    Coach(SSLReferee *ref, MRCTeam *ourTeam, MRCTeam *theirTeam);
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

    // Debug
    bool _showRoles;
};

#endif // CONTROLLER_H
