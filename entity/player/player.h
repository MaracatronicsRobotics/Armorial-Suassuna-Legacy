#ifndef PLAYER_H
#define PLAYER_H

#include <entity/entity.h>
#include <entity/world/world.h>
#include <utils/utils.hh>
#include <entity/player/baseplayer.h>
#include <bits/stdc++.h>
#include <entity/locations.h>
#include <entity/player/control/pid.h>

#define IDLE_COUNT 10

class Player : public Entity
{
public:
    Player(World *world, MRCTeam *team, Controller *ctr, quint8 playerID, Role *defaultRole, SSLReferee *ref, grsSimulator *grSim, PID *vxPID, PID *vyPID, PID *vwPID);
    ~Player();
    QString name();
    void reset();

    // Player infos
    quint8 playerId() const;
    quint8 teamId() const;
    quint8 opTeamId() const;
    PlayerAccess* access() const;
    Colors::Color teamColor() const;

    // Robot locomotion
    // algoritmos de controle básicos ficam aqui
    void idle();

    // Auxiliary methods
    // Position
    Position position() const;
    Position nextPosition() const;

    // Orientation
    Angle orientation() const;
    Angle nextOrientation() const;

    // Velocity
    Velocity velocity() const;
    float lastSpeed() const;
    AngularSpeed angularSpeed() const;

    // Direction
    Angle nextDirection() const;

    // Booleans (checks)
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

    // Player skills
    void setSpeed(float x, float y, float theta);
    std::pair<float, float> goTo(double robot_x, double robot_y, double point_x, double point_y, double robotAngle, double _distBall = 0.2);
    std::pair<double, double> rotateTo(double robot_x, double robot_y, double point_x, double point_y, double robotAngle);
    void goToLookTo(double robot_x, double robot_y, double point_x, double point_y, double aim_x, double aim_y, double angleOrigin2Robot, double offset = 0.2);
    void aroundTheBall(double robot_x, double robot_y, double point_x, double point_y, double robotAngle, double offset);
    void kick(bool isPass, float kickZPower = 0.0);
    void dribble(bool isActive);

    // Role
    void setRole(Role *b);
    QString getRoleName();

private:
    // Entity inherit virtual methods
    void initialization();
    void loop();
    void finalization();

    // Player info
    quint8 _playerId;
    int _idleCount;

    // Acceleration control
    float _lastSpeedAbs;

    // Next states info
    Position _nextPosition;
    Angle _nextOrientation;
    Angle _nextDirection;

    // Player related objects
    SSLReferee *_ref;
    MRCTeam *_team;
    PlayerAccess *_playerAccessSelf;
    PlayerAccess *_playerAccessBus;

    // World for sync
    World *_world;

    // Controller
    Controller *_ctr;

    // Role
    Role *_role;
    Role *_defaultRole;
    QMutex _mutexRole;

    // errors
    float _lError;
    float _aError;

    // grSimulator for tests
    grsSimulator *_grSim;

    // PID's
    PID *_vxPID;
    PID *_vyPID;
    PID *_vwPID;


};

#endif // PLAYER_H
