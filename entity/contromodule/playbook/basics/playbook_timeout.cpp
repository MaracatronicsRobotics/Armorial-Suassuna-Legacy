#include "playbook_timeout.h"

QString Playbook_TimeOut::name() {
    return "Playbook_TimeOut";
}

Playbook_TimeOut::Playbook_TimeOut() {
}

int Playbook_TimeOut::maxNumPlayer() {
    return INT_MAX;
}

void Playbook_TimeOut::configure(int numPlayers) {
    _bh_to.clear();
    for(int i=0; i<numPlayers; i++) {
        Behaviour_TimeOut *bh_timeOut = new Behaviour_TimeOut();
        usesBehaviour(bh_timeOut);
        _bh_to.push_back(bh_timeOut);
    }
}

void Playbook_TimeOut::run(int numPlayers) {
/*
    if(numPlayers == 5){
        double x = loc()->ourGoalLeftPost().x();
        double y = loc()->ourGoalLeftPost().y();

        Position pos1 = Position(true, x - 2, y, 0);

        x = loc()->ourGoalRightPost().x();
        y = loc()->ourGoalRightPost().y();
        Position pos2 = Position(true, x - 2, y, 0);

        Position pos3 = Position(true, pos1.x() - 1, pos1.y() + 0.5, 0.0);
        Position pos4 = Position(true, pos2.x() - 1, pos2.y() - 0.5, 0.0);

        Position pos5 = Position(true, pos1.x() - 0.4, 0, 0);

        _bh_to.at(0)->setSkillPosition(pos1);
        _bh_to.at(1)->setSkillPosition(pos2);
        _bh_to.at(2)->setSkillPosition(pos3);
        _bh_to.at(3)->setSkillPosition(pos4);
        _bh_to.at(4)->setSkillPosition(pos5);
    }else{
        double x = loc()->ourGoalLeftPost().x();
        double y = loc()->ourGoalLeftPost().y();

        for(int i = 0; i < numPlayers; i++){
            _bh_to.at(i)->setSkillPosition(Position(true, x - 2, y - 0.5, 0.0));
            y = y - 0.5;
        }
    }
*/
    for(int i=0; i<numPlayers; i++){
        _bh_to.at(i)->setSkillPosition(loc()->ball());
        setPlayerBehaviour(dist()->getPlayer(), _bh_to.at(i));
    }
}
