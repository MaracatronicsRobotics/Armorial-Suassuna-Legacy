/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef BEHAVIOUR_ATTACKER_H
#define BEHAVIOUR_ATTACKER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>
#include <utils/mrctimer/mrctimer.h>

#include <utils/basics/circle.h>
#include <utils/basics/triangle.h>
#include <const/mlp.h>

#define RECEIVER_DECISION_TIME_AT_FOUL 4.0f //seconds
#define RECEIVER_DECISION_TIME         0.2f // seconds
#define AIM_DECISION_TIME              0.2f // seconds
#define SHOOT_PASS_DECISION_TIME       0.75f // seconds

typedef struct{
    bool valid;
    quint8 id;
    Position position;
    float distToKicker;
} playerInfo;

class Behaviour_Attacker : public Behaviour {
private:
    void configure();
    void run();

    // Machine state
    int _state;
    enum {
        STATE_CANTKICK,
        STATE_PUSH
    };

    // Skill Transitions
    enum {
        SKT_POS,
        SKT_PUSH,
        SKT_KICK
    };

    // Receivers
    QList<quint8> _receiversList;
    std::pair<float, quint8> getBestReceiver();
    QMutex receiversListMutex;
    quint8 _bestRcv;
    float _rcvScore;

    QList<FreeAngles::Interval> intervalList;

    // Utils functions
    bool canTakeBall();
    quint8 getTheirClosestPlayerToGoal();
    std::pair<float, Position> getBestAimPosition();
    std::pair<float, Position> getBestAssumedPosition(quint8 _id);
    float MLP_result();

    //MLP AuxFunctions
    playerInfo calc(playerInfo jogador);
    playerInfo OppGoalie(playerInfo kicker);
    playerInfo AllyGoalie(playerInfo kicker);
    void Ally(playerInfo kicker, playerInfo allyGoalie, playerInfo ally[]);
    void Opp(playerInfo kicker, playerInfo oppGoalie, playerInfo opp[]);
    bool isObst(playerInfo kicker, playerInfo player);

    double getActualKickChance(quint8 _id);
    double getFutureKickChance(quint8 _id);
    float getPlayerPassingChance(quint8 _id);
    float getFinalPassingChance(quint8 _id);
    double getsizebar(Point2d recpos, Point2d directbarra);
    std::pair<Point2d, Point2d> calc_freeangles_points(quint8 bestReceiver);

    float _distScore;
    float _angleScore;

    float _mlpResult;

    double _firstSide;
    double _secondSide;
    double _oppositeSide;
    double _cosAngle;
    float _angle;
    float _recDist;
    std::pair<Point2d, Point2d> PFA;
    float _angleDisp;
    float _angleTotal;
    float _receiverDistScore;
    float _receiverAngleScore;

    Position _receiverAngleUp;
    Position _receiverAngleDown;

    // Aim
    std::pair<float, Position> _aim;
    bool firstAim;

    // Shoot / pass decision
    bool canShoot;

    // Decision timer
    Timer receiverDecisionTimer;
    Timer shootPassDecisionTimer;
    Timer aimDecisionTimer;

    // Debug
    void mostrar_niveis(quint8 _id);
    float _PassingChanceVec[20];
    float _receivertoGoalVec[20];

public:
    Behaviour_Attacker();
    QString name();

    // Skills
    Skill_GoToLookTo *_sk_goToLookTo;
    Skill_PushBall *_sk_push;
    Skill_Kick *_sk_kick;

    // Receivers add
    void addReceiver(quint8 id);
    void clearReceivers();
};

#endif // BEHAVIOUR_ATTACKER_H
