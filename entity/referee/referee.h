#ifndef REFEREE_HH
#define REFEREE_HH

#include <entity/entity.h>

class Referee : public Entity {
public:
    Referee();
    virtual void setBallKicked() = 0;
};


#endif // REFEREE_HH
