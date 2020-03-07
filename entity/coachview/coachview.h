#ifndef COACHVIEW_H
#define COACHVIEW_H

#include <entity/entity.h>
#include <entity/baseentity.h>

class CoachView : public Entity
{
public:
    CoachView();
    ~CoachView();

    QString name();
    MainWindow* getUI();
    void setTeams(MRCTeam *our, MRCTeam *their);
    MRCTeam* getOurTeam() { return _ourTeam; }

private:
    void initialization();
    void loop();
    void finalization();

    // UI
    MainWindow *_suassunaUI;

    // Teams
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;
};

#endif // COACHVIEW_H
