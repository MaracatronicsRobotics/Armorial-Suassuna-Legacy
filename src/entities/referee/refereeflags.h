#ifndef REFEREEFLAGS_H
#define REFEREEFLAGS_H

enum RefereeFlag {
    // commands
    GAME_ON        = 1 << 0,
    GAME_OFF       = 1 << 1,
    HALT           = 1 << 2,
    KICKOFF        = 1 << 3,
    PENALTY        = 1 << 4,
    DIRECT_KICK    = 1 << 5,
    INDIRECT_KICK  = 1 << 6,
    BALL_PLACEMENT = 1 << 7,
    TIMEOUT        = 1 << 8,
    // team colors
    TEAM_YELLOW    = 1 << 9,
    TEAM_BLUE      = 1 << 10,
    // state flags
    READY          = 1 << 11,
    NOT_READY      = 1 << 12
};

#endif // REFEREEFLAGS_H
