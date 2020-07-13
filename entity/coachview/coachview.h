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

#ifndef COACHVIEW_H
#define COACHVIEW_H

#include <entity/entity.h>
#include <entity/baseentity.h>
#include <entity/contromodule/coach.h>
#include <entity/referee/SSLReferee/sslreferee.h>
#include <utils/mrctimer/mrctimer.h>
#include <include/timer.h>
#include <const/constants.h>
#include <entity/coachview/soccerview/soccerview.h>
#include <const/constants.h>

class CoachView : public Entity
{
public:
    CoachView(MRCConstants *mrcconstants);
    ~CoachView();

    QString name();
    static void setTeams(MRCTeam *our, MRCTeam *their);
    static void setCoach(Coach *coach);
    static void setReferee(SSLReferee *ref);
    static void updateTree(StrategyState *strat);
    static MRCTeam* getOurTeam() { return _ourTeam; }

    double timeToUpdate;

private:
    void initialization();
    void loop();
    void finalization();

    // UI
    static MainWindow *_suassunaUI;

    // Ref
    QList<SSLGameInfo*> _gameInfo;

    // Teams
    static MRCTeam *_ourTeam;
    static MRCTeam *_theirTeam;

    // Coach
    static Coach *_coach;

    // Referee
    static SSLReferee *_ref;

    // Timer
    Timer _timer;

    MRCConstants *_mrcconstants;
};

#endif // COACHVIEW_H
