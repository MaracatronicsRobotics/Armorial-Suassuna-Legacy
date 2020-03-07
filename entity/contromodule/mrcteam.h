#ifndef MRCTEAM_H
#define MRCTEAM_H

#include <utils/fieldside/fieldside.hh>
#include <entity/player/player.h>

#define BALLPOSS_NONE 100

class MRCTeam
{
public:
    MRCTeam(quint8 teamId, WorldMap *wm);
    virtual ~MRCTeam();

    // Constant infos access
    quint8 teamId() const { return _teamId; }

    // Infos getters
    Colors::Color teamColor() const;
    FieldSide fieldSide() const;

    // Infos setters
    void setTeamColor(Colors::Color color);
    void setFieldSide(FieldSide fieldSide);

    // Opponent team info
    void setOpponentTeam(MRCTeam *team) { _opTeam = team; }
    MRCTeam *opTeam() const { return _opTeam; }

    // Ball possession
    bool hasBallPossession();
    quint8 ballPossession();

    // Add players
    void addPlayer(Player *player);

    // Available players access
    int avPlayersSize();
    QHash<quint8, Player*> avPlayers();
    void updateAvailablePlayers();

    // WorldMap access
    WorldMap *wm() { return _wm; }
    Locations* loc() { return _loc; }
    Position ballPosition();

private:
    // Team id
    quint8 _teamId;

    // Team color
    Colors::Color _teamColor;
    mutable QMutex _mutexTeamColor;

    // Team field side
    FieldSide _fieldSide;
    mutable QMutex _mutexFieldSide;

    // Opp team access
    MRCTeam *_opTeam;

    // Players
    QMutex _muxPlayers;
    QHash<quint8, Player*> _players;
    QHash<quint8, Player*> _avPlayers;

    // Locations
    Locations *_loc;

    // Worldmap
    WorldMap *_wm;
};

#endif // MRCTEAM_H
