#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <GEARSystem/gearsystem.hh>

class Obstacle{

private:
    Position _position; 
    float _radius; //raio do obstaculo
    float _finalAngle; 
    float _initialAngle; //faixa de angulos do obstaculo em relação ao observador
    quint8 _team;
    quint8 _id; //identidade do obstaculo

public: 
    Obstacle();

    Position getPosition(){
        return _position;
    }
    float getRadius(){
        return _radius;
    }
    float getFinalAngle(){
        return _finalAngle;
    }
    float getInitialAngle(){
        return _initialAngle;
    }
    quint8 getTeam(){
        return _team;
    }
    quint8 getId(){
        return _id;
    }

    void setPosition(Position position){
        _position = position;
    }
    void setRadius(float radius){
        _radius = radius;
    }
    void setFinalAngle(float finalAngle){
        _finalAngle = finalAngle;
    }
    void setInitialAngle(float initialAngle){
        _initialAngle = initialAngle;
    }
    void setTeam(quint8 team){
        _team = team;
    }
    void setId(quint8 id){
        _id = id;
    }

    void calcAngles(const Position &watcher);//descobrir para que serve o radius factor?

};

#endif //OBSTACLE_H
