#ifndef PLAYERBUS_H
#define PLAYERBUS_H

#include <entity/player/baseplayer.h>

class PlayerBus {
private:
    static MRCTeam *_ourTeam;
    static MRCTeam *_theirTeam;
public:
    static void initialize(MRCTeam *ourTeam, MRCTeam *theirTeam);
    static bool ourPlayerAvailable(quint8 id);
    static bool theirPlayerAvailable(quint8 id);
    static PlayerAccess* ourPlayer(quint8 id);
    static PlayerAccess* theirPlayer(quint8 id);
};

#endif // PLAYERBUS_H
