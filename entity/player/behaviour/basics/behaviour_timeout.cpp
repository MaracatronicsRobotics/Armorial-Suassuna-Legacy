
    _offsetBall = 0.01;
    _desiredPosition = Position(true, 0.0, 0.0, 0.0);
>>>>>>> createPID
    _skill_GoToLookTo->setDesiredPosition(_desiredPosition);
    _skill_GoToLookTo->setOffsetToBall(_offsetBall);
}

void Behaviour_TimeOut::setSkillPosition(Position pos){
    _desiredPosition = pos;
}
