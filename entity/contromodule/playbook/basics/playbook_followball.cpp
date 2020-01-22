#include "playbook_followball.h"

<<<<<<< Updated upstream
playbook_followball::playbook_followball()
{

=======
QString Playbook_FollowBall::name() {
    return "Playbook_FollowBall";
}

Playbook_FollowBall::Playbook_FollowBall() {
}

int Playbook_FollowBall::maxNumPlayer() {
    return 3;
}

void Playbook_FollowBall::configure(int numPlayers) {
    _bh_followBall.clear();
    for(int i=0; i<numPlayers; i++) {
        Behaviour_FollowBall *_bh_fb = new Behaviour_FollowBall();
        
        usesBehaviour(_bh_fb);
        _bh_followBall.push_back(_bh_fb);
    }
}

void Playbook_FollowBall::run(int numPlayers) {
    for(int i=0; i<numPlayers; i++)
        setPlayerBehaviour(dist()->getPlayer(), _bh_followBall.at(i));
>>>>>>> Stashed changes
}
