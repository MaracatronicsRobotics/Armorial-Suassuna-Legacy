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

class CoachView : public Entity
{
public:
    CoachView();
    ~CoachView();

    QString name();
    MainWindow* getUI();
    void setTeams(MRCTeam *our, MRCTeam *their);
    void setCoach(Coach *coach);
    void setReferee(SSLReferee *ref);
    MRCTeam* getOurTeam() { return _ourTeam; }

private:
    void initialization();
    void loop();
    void finalization();

    // UI
    MainWindow *_suassunaUI;

    // Ref
    QList<SSLGameInfo*> _gameInfo;

    // Mutex
    QMutex *_UIMutex;

    // Teams
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;

    // Coach
    Coach *_coach;

    // Referee
    SSLReferee *_ref;
};

#endif // COACHVIEW_H
