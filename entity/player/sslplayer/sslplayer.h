#ifndef SSLPLAYER_H
#define SSLPLAYER_H

#include <entity/player/player.h>

class SSLPlayer : public Player
{
public:
    SSLPlayer(World *world, quint8 playerID, SSLReferee *ref);
};

#endif // SSLPLAYER_H
