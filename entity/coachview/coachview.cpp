#include "coachview.h"
#include <entity/coachview/mainwindow.h>
#include <const/constants.h>

QString CoachView::name(){
    return "Coach View";
}

void CoachView::setTeams(MRCTeam *our, MRCTeam *their){
    _ourTeam = our;
    _theirTeam = their;

    _suassunaUI->setupTeams(_ourTeam, _theirTeam);
}

CoachView::CoachView() : Entity(ENT_GUI)
{
    _suassunaUI = new MainWindow();
    _suassunaUI->show();
}

MainWindow* CoachView::getUI(){
    return _suassunaUI;
}

CoachView::~CoachView(){
    _suassunaUI->close();
}

void CoachView::initialization(){
    this->setLoopFrequency(MRCConstants::threadFrequency());
}

void CoachView::loop(){

}

void CoachView::finalization(){

}
