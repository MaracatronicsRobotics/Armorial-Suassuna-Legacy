#include "freeangles.h"
#include <iostream>
#include <entity/contromodule/mrcteam.h>
#include <entity/player/player.h>
#include <utils/utils.hh>
#include <GEARSystem/gearsystem.hh>

#define twoPi 6.283185307

MRCTeam* FreeAngles:: _ourTeam = NULL;
MRCTeam* FreeAngles:: _opTeam = NULL;

QList<Obstacle> FreeAngles::getObstacles(const Position &watcher, float distanceRadius, int defineObstacle){

    if(_ourTeam == NULL || _opTeam == NULL){
        std::cout << "[ERROR] FreeAngles::getObstacles() _ourTeam == NULL || _opTeam == NULL \n";
        return QList<Obstacle>();
    }

    QList<Obstacle> obstacles;

    //Loop mais externo para definir os obstaculos
    for(int team = 0; team < 2; team++){

        MRCTeam *mrcTeam;

        if(team == 0)
            mrcTeam = _ourTeam;
        else
            mrcTeam = _opTeam;
    
        const QList<Player*> players = mrcTeam->avPlayers().values();
        QList<Player*>::const_iterator it;

        if(defineObstacle == 1 && team == 0)
            continue;
        if(defineObstacle == 0 && team == 1)
            continue;

        //Loop mais interno para passar por todos os jogadores
        for(it = players.constBegin(); it != players.constEnd();it++){

            const Player *player = *it;
            const Position posPlayer = player->position();

            //Verificar se o player em questão é diferente do observador
            if(watcher.isUnknown() == false && watcher.x() == posPlayer.x() & watcher.y() == posPlayer.y())
                continue;
            
            //Verificar se o player em analise esta dentro do raio limite
            if(watcher.isUnknown() == false){
                float distPos = WR::Utils::distance(watcher, posPlayer);

                if(distPos > distanceRadius){
                    continue;
                 }
            }

            //Armazenar as informacoes do jogador
            Obstacle obst; //variavel auxiliar
            obst.setId(player->playerId());
            obst.setTeam(player->teamId());
            obst.setPosition(player->position());
            obst.setRadius(0.09);

            //Enviar para a lista geral
            obstacles.push_back(obst);
        }
    }
    return obstacles;
}

QList<FreeAngles> FreeAngles::getFreeAngles(const Position &watcher, float angleMin, float angleMax, int defineObstacle, float distanceRadius){

    QList<Obstacle> obstacles = getObstacles(watcher, distanceRadius, defineObstacle);

    //Redefine o valor de angleMin, caso esse seja maior que o angleMax
    if(angleMin > angleMax){
        angleMin -= twoPi;
    }
    
    QList<FreeAngles> Intervals; //Lista que será retornada
    FreeAngles interval; //Elemento auxiliar

    //Caso a lista de obstaculos esteja vazia o metodo é encerrado indicando que todo o intervalo de analise
    //esta livre
    if(obstacles.size()==0){
        interval.setInitialAngle(angleMin);
        interval.setFinalAngle(angleMax);

        Intervals.push_back(interval);
        return Intervals;

    }


    //Preencher as informações do initialAngle e finalAngle
    QList<Obstacle> tmpObstacles = getObstaclesToWatcher(watcher, obstacles);

    //Nessa parte é armazenada em um vetor auxiliar todos os angulos recebidos atraves da lista obstacles
    float organizeAngle[2*tmpObstacles.size()];
    for(int i = 0; i < tmpObstacles.size(); i++){

        Obstacle obst = tmpObstacles.at(i);

        organizeAngle[i] = obst.getInitialAngle();
        organizeAngle[i + tmpObstacles.size()] = obst.getFinalAngle();
    }

    //Remover angulos que estão fora do intervalo selecionado "angleMin - angleMax"
    int reduction = 0;
    for(int i = 0; i < 2*tmpObstacles.size(); i++){
        if(organizeAngle[i] < angleMin){
            organizeAngle[i] = 0;
            reduction++;

        }
            
        if(organizeAngle[i] > angleMax){
            organizeAngle[i] = 0;
            reduction++;
        }
    }

    //Ordenar os angulos de forma crescente para depois organizar os intervalos
    float aux = 0;

    for(int i = 0; i < 2*tmpObstacles.size(); i++){
        for(int j = 0; j < 2*tmpObstacles.size(); j++){

            if(organizeAngle[i] < organizeAngle[j]){

                aux = organizeAngle[i];
                organizeAngle[i] = organizeAngle[j];
                organizeAngle[j] = aux;
            }
        }
    }

    //Organizacao dos intervalos
    //reduction é uma variavel auxiliar para pular os elementos anulados dentro do vetor "organize"
    int actualSize = 2*tmpObstacles.size();
    for(int i = reduction; i <= actualSize; i++){

        if(i == reduction){
            interval.setInitialAngle(angleMin);
            interval.setFinalAngle(organizeAngle[i]);
        }
        if(i > reduction && i < actualSize){
        interval.setInitialAngle(organizeAngle[i-1]);
        interval.setFinalAngle(organizeAngle[i]);
        }
        if(i == actualSize){
            interval.setInitialAngle(organizeAngle[i-1]);
            interval.setFinalAngle(angleMax);
        }

        //Definir se o intervalo atual esta obstruido ou nao
        float mediaAngle = (interval.getInitialAngle()+interval.getFinalAngle())/2;

        for(int j = 0; j < tmpObstacles.size(); j++){

            Obstacle auxObst = tmpObstacles.at(j);

            if(auxObst.getInitialAngle() > auxObst.getFinalAngle()){

                if(mediaAngle > (auxObst.getInitialAngle()-GEARSystem::Angle::pi*2) && mediaAngle < auxObst.getFinalAngle()){
                    interval.setObstructed(true);
                    break;
                }
                if(interval.getFinalAngle() > auxObst.getInitialAngle()){
                    interval.setObstructed(true);
                    break;
                }
            }
            if(mediaAngle > auxObst.getInitialAngle() && mediaAngle < auxObst.getFinalAngle()){
                    interval.setObstructed(true);
                    break;
            }
            interval.setObstructed(false);
        }
        Intervals.push_back(interval);
    }
    return Intervals;
}

QList<Obstacle> FreeAngles::getObstaclesToWatcher(const Position &watcher, const QList<Obstacle> &obstacles) {
    // Iterate through obstacles and calc obstruction
    QList<Obstacle> retnObstacles;
    for(int i=0; i<obstacles.size(); i++) {
        Obstacle obst = obstacles.at(i);

        // Continue if obstacle is the watcher
        if(watcher.x()==obst.getPosition().x() && watcher.y()==obst.getPosition().y())
            continue;

        obst.calcAngles(watcher);
        retnObstacles.push_back(obst);
    }
    return retnObstacles;
}
