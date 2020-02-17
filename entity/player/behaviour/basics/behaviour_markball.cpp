#include "behaviour_markball.h"

QString Behaviour_MarkBall::name() {
    return "Behaviour_Marker";
}

Behaviour_MarkBall::Behaviour_MarkBall() {
    _sk_GoToLookTo = NULL;
}

void Behaviour_MarkBall::configure() {
    usesSkill(_sk_GoToLookTo = new Skill_GoToLookTo());
};

void Behaviour_MarkBall::run() {
    _sk_GoToLookTo->setOffsetToBall(0.15);
    _sk_GoToLookTo->setDesiredPosition(loc()->ball());
    for (int id = 0; id < 6; id++) {
        if(PlayerBus::theirPlayerAvailable(id)){
            if (PlayerBus::theirPlayer(id)->hasBallPossession()) {
                if (PlayerBus::theirPlayer(id)->orientation().value() < 0 && PlayerBus::theirPlayer(id)->orientation().value() > -3.141592654)
                    _sk_GoToLookTo->setAimPosition(PlayerBus::theirPlayer(id)->position());
            }
        }
        else _sk_GoToLookTo->setAimPosition(loc()->ball());
    }
}

