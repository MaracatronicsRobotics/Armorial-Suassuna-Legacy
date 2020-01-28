#include "behaviour_goalkeeper.h"
#include <utils/knn/knn.hh>
#include <entity/player/playerbus.h>

#define ATTACKER_MINBALLDIST 0.4f
#define GOALPOSTS_ERROR 0.05f
#define INTERCEPT_MINBALLVELOCITY 0.2f

QString Behaviour_GoalKeeper::name() {
    return "Behaviour_GoalKeeper";
}

Behaviour_GoalKeeper::Behaviour_GoalKeeper() {
    _skill_GoalKeeper = NULL;
    _skill_kick = NULL;
    _skill_goToLookTo = NULL;

    setRadius(0.3); // raio que define posse de bola para o goleiro dar takeout
    setTakeoutEnabled(true); // avançar na bola quando ela estiver na margem de aceitação (takeout vai dar goto e kick na bola)
    setTakeoutFactor(1.3); // fator de erro pra largura do gol
    useAttackerOri(true); // pra levar o atacante em consideração na projeção no gol
}

void Behaviour_GoalKeeper::configure() {
    usesSkill(_skill_GoalKeeper = new Skill_GoalKeeper());
    usesSkill(_skill_goToLookTo = new Skill_GoToLookTo());
    usesSkill(_skill_kick = new Skill_Kick());

    // goto
    setInitialSkill(_skill_goToLookTo);

    // todas as combinações
    addTransition(STATE_KICK, _skill_goToLookTo, _skill_kick);
    addTransition(STATE_KICK, _skill_GoalKeeper, _skill_kick);
    //
    addTransition(STATE_GK, _skill_kick, _skill_GoalKeeper);
    addTransition(STATE_GK, _skill_goToLookTo, _skill_GoalKeeper);
    //
    addTransition(STATE_GOTO, _skill_GoalKeeper, _skill_goToLookTo);
    addTransition(STATE_GOTO, _skill_kick, _skill_goToLookTo);
};

void Behaviour_GoalKeeper::run() {

    _skill_GoalKeeper->setInterceptAdvance(true);
    _skill_GoalKeeper->setPositionToLook(loc()->ball());

    _skill_kick->setIsPass(false);

    // goToLookTo (posicionamento do goleiro
    Position desiredPosition = getAttackerInterceptPosition();
    if(loc()->ourSide().isRight() && desiredPosition.x() > loc()->ourGoal().x()-GOALPOSTS_ERROR)        // cobrir angulos certos (manter goleiro na area do gol!)
        desiredPosition.setPosition(loc()->ourGoal().x()-GOALPOSTS_ERROR, desiredPosition.y(), 0.0);     // varia de lado pra lado, com erros nas barras p precisao
    else if(loc()->ourSide().isLeft() && desiredPosition.x() < loc()->ourGoal().x()+GOALPOSTS_ERROR)
        desiredPosition.setPosition(loc()->ourGoal().x()+GOALPOSTS_ERROR, desiredPosition.y(), 0.0);

    _skill_goToLookTo->setDesiredPosition(desiredPosition);
    _skill_goToLookTo->setAimPosition(loc()->ball());

    // machine if state begins for transitionsss
    if(player()->distBall() > _radius && isBallComingToGoal(INTERCEPT_MINBALLVELOCITY)){ // bola nao ta em posse do goleiro e ta indo pro gol
        enableTransition(STATE_GK); // defende!
    }else if(_takeoutEnabled){ // caso n esteja em posse, n esteja indo pro gol ou nenhum dos dois
        if(loc()->isInsideOurArea(loc()->ball(), _takeoutFactor)){ // ve se ta na nossa area com fator de takeout (uma area maiorzinha)
            enableTransition(STATE_KICK); // se tiver perto e na nossa area, chuta!!!!
        }else if(loc()->isInsideOurArea(loc()->ball(), 1.1 * _takeoutFactor) == false){ // evitar oscilação (ruido) do visao
            enableTransition(STATE_GOTO); // goTo na bolota
        }
    }else{
        enableTransition(STATE_GOTO); // caso n usemos takeout, fica dando só goToLookTo mesmo (tentativa de dominar bola)
    }

}

bool Behaviour_GoalKeeper::isBallComingToGoal(float minSpeed, float postsFactor){
    // postFactor é caso queiramos alongar mais a posição y da barra (margem de erro)
    const Position posBall = loc()->ball();
    const Position posRightPost(true, loc()->ourGoalRightPost().x(), loc()->ourGoalRightPost().y()*postsFactor, 0.0);
    const Position posLeftPost(true, loc()->ourGoalLeftPost().x(), loc()->ourGoalLeftPost().y()*postsFactor, 0.0);

    // Check ball velocity
    if(loc()->ballVelocity().abs()<minSpeed)
         return false;

    // Angle ball velocity
    float angVel = loc()->ballVelocity().arg().value();
    float angRightPost = WR::Utils::getAngle(posBall, posRightPost);
    float angLeftPost = WR::Utils::getAngle(posBall, posLeftPost);
    float angDiffPosts = fabs(WR::Utils::angleDiff(angRightPost, angLeftPost));

    // Check angle difference with posts
    float angDiffRight = fabs(WR::Utils::angleDiff(angVel, angRightPost));
    float angDiffLeft = fabs(WR::Utils::angleDiff(angVel, angLeftPost));

    return (angDiffRight<angDiffPosts && angDiffLeft<angDiffPosts);
}

Position Behaviour_GoalKeeper::getAttackerInterceptPosition() {
    // Defense position
    Position interceptPosition = WR::Utils::threePoints(loc()->ourGoal(), loc()->ball(), _radius, 0.0);
    if(_useAttackerOri==false)
        return interceptPosition;

    float goal_left = loc()->ourGoalLeftPost().y();
    float goal_right = loc()->ourGoalRightPost().y();

    // Calc ball impact based on attacker ori and check if its going to the goal
    Position posImpact = calcAttackerBallImpact();

    if(loc()->ourSide().isRight()) {
        goal_left += GOALPOSTS_ERROR;
        goal_right -= GOALPOSTS_ERROR;
        if(goal_left >= posImpact.y() && posImpact.y() >= goal_right)
            interceptPosition = WR::Utils::threePoints(posImpact, loc()->ball(), _radius, 0.0);
    } else {
        goal_left -= GOALPOSTS_ERROR;
        goal_right += GOALPOSTS_ERROR;
        if(goal_right >= posImpact.y() && posImpact.y() >= goal_left)
            interceptPosition = WR::Utils::threePoints(posImpact, loc()->ball(), _radius, 0.0);
    }

    return interceptPosition;
}

Position Behaviour_GoalKeeper::calcAttackerBallImpact() {
    // fazer logica para calcular o impacto da bola
    // verificar se esta suficientemente perto da bola, se a bola ta na frente dele e ver se o caminho p o chute dele está livre, anyway posicionado para nosso gol pra testar impacto centrado
    //int poss = -1, size = loc()->getOpPlayers().size();
    int poss = -1, size = loc()->getMRCPlayers().size();
    for(int x = 0; x < size; x++){
        /*if(loc()->getOpPlayers()[x]->hasBallPossession()){
            poss = x;
            break;
        }*/
        if(loc()->getMRCPlayers()[x]->hasBallPossession()){
            poss = x;
            break;
        }
    }

    if(poss == -1) // n tem ngm com a posse, mantem no centro do gol
        return loc()->ourGoal();

    /* calculando posicao de impacto no y */
    Angle angleAtk = PlayerBus::theirPlayer(poss)->orientation();
    Position posImpact; // impacto no nosso gol (pegar x da barra)

    float angleValue = angleAtk.value();

    if(loc()->ourSide().isLeft()){ // ajustando pra o lado esquerdo
        angleValue = GEARSystem::Angle::pi - angleValue;
    }

    // Check 2pi
    if(angleValue==GEARSystem::Angle::twoPi)
        angleValue = 0.0f;

    // Check high angle
    if(fabs(angleValue)>=GEARSystem::Angle::pi)
        angleValue += (angleValue>=0? -GEARSystem::Angle::twoPi : GEARSystem::Angle::twoPi);

    // Check impossible impact (infinite tg)
    if(fabs(angleValue)>=GEARSystem::Angle::pi/2.0f)
        return Position(false, 0.0, 0.0, 0.0);

    // Triagulate
    float x = fabs(loc()->ourGoal().x() - loc()->ball().x());
    float tg = tan(angleValue);
    float y = tg*x;

    // Impact point
    float impact_y = loc()->ball().y() + y;
    posImpact = Position(true, loc()->ourGoal().x(), impact_y, 0.0); // posicao de impacto (mudando só o y em teoria)
                                                                      // verificar dps a ideia de mover ele pra frente e reduzir angulação
    /* calculando posicao de impacto no y */

    return posImpact; // retorna o impacto, em caso de alguem ter posse da bola
}
