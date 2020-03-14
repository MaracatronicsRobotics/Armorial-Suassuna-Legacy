#ifndef PLAYERACCESS_H
#define PLAYERACCESS_H

#include <entity/player/baseplayer.h>

class PlayerAccess {
private:
    bool _self;
    Player *_player;
    Locations *_loc;
public:
    PlayerAccess(bool self, Player *player, Locations *loc);
    virtual ~PlayerAccess();
    // Kick
    void kick(float power = 8.0);
    // General player info
    quint8 playerId() const;
    quint8 teamId() const;
    quint8 opTeamId() const;
    // Robot auxiliary methods
    // Position
    Position position() const;
    Position nextPosition() const;
    // Orientation
    virtual Angle orientation() const;
    Angle nextOrientation() const;
    // Velocity
    Velocity velocity() const;
    float lastSpeed() const;
    // Direction
    Angle nextDirection() const;
    // Booleans
    bool isLookingTo(const Position &pos) const;
    bool isLookingTo(const Position &pos, float error) const;
    bool isAtPosition(const Position &pos) const;
    bool isNearbyPosition(const Position &pos, float error) const;
    bool hasBallPossession() const;
    bool canKickBall() const;
    // Distances
    float distanceTo(const Position &pos) const;
    float distBall() const;
    float distOurGoal() const;
    float distTheirGoal() const;
    // Angles
    Angle angleTo(const Position &pos) const;
    // Roles
    QString roleName();
};


#endif // PLAYERACCESS_H
