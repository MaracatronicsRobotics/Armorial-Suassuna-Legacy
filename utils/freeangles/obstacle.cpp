#include "obstacle.h"
#include <math.h>
#include <utils/utils.hh>
#include <iostream>

//Desenvolvimento das funções da class Obstacle

Obstacle::Obstacle(){

    _position.setUnknown();
    _finalAngle = _initialAngle = 0;
    _radius = 0;
    
}

void Obstacle::calcAngles(const Position &watcher){ //Refazer essa parte depois como victor fez

    if(_radius == 0){
        std::cout <<"[ERROR] Obstacle::calcAngles() With _radius == 0\n";
        return;
    }

    float distance;
    float gap; //metade do angulo de bloqueio
    float angle; //angulo formado da reta que une os centros com a linha de referencia

    //Calculo do angulo obstruido pelo obstaculo
    distance = WR::Utils::distance(watcher, _position); //distancia de um centro a outro
    gap = atan(_radius/distance);                       
 
    angle = WR::Utils::getAngle(watcher, _position);
    _initialAngle = angle - gap;
    _finalAngle = angle + gap;

    //Garantir que a faixa de valor do Angulo final e inicial estejam entre 0-360
    WR::Utils::angleLimitZeroTwoPi(&_initialAngle);
    WR::Utils::angleLimitZeroTwoPi(&_finalAngle);


}
