#include "role_default.h"

QString Role_Default::name(){
    return "Role_Default";
}

Role_Default::Role_Default() {
}

void Role_Default::initializeBehaviours(){
    // Aqui são inseridos os behaviours possíveis de serem usados
    // na ordem: ID do behaviour, instanciação dele
    usesBehaviour(BHV_DONOTHING, _bh_dn = new Behaviour_MarkBall());
}

void Role_Default::configure(){
    // Aqui são setados parametros que devem ser configurados
}

void Role_Default::run(){
    /*
     * Aqui devem ocorrer os sets de parametros de acordo com o behaviour
     * que estiver sendo executado, de preferencia declare todos os parametros
     * na classe da role, sete-os aqui e envie para o behaviour (usando as funções
     * set presentes neles)
    */

    setBehaviour(BHV_DONOTHING);

}
