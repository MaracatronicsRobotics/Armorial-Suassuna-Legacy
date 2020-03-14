#ifndef ROLE_DEFAULT_H
#define ROLE_DEFAULT_H

#include <entity/player/behaviour/mrcbehaviours.h>
#include <entity/player/role/role.h>

class Role_Default : public Role
{
private:
    // Behaviours
    Behaviour_DoNothing *_bh_dn;
    Behaviour_Barrier *_bh_def;

    // Behaviours ids!
    enum{
        BHV_DONOTHING,
        BHV_BARRIER
    };

    // Inherited functions
    void configure();
    void run();

    // Mutex
    QMutex _mutex;

public:
    Role_Default();
    void initializeBehaviours();
    QString name();
};

#endif // ROLE_DEFAULT_H
