#include "univector.h"

Univector::Univector(quint8 playerId){
    _playerId = playerId;
    _radius = Suassuna::Constants::univectorRadius();
    _kr = Suassuna::Constants::univectorKr();
    _delta = Suassuna::Constants::univectorDelta();
    _dmin = Suassuna::Constants::univectorDmin();
    _ko = Suassuna::Constants::univectorKo();
}

double Univector::norm(double d_x, double d_y){
    return sqrt((d_x) * (d_x) + (d_y) * (d_y));
}

QVector<float> Univector::Nh(float phi){
    QVector<float> v = {cosf(phi), sinf(phi)};
    return v;
}

float Univector::wrapToPi(float angle){
    if(angle > static_cast<float>(M_PI)){
        return angle - 2 * static_cast<float>(M_PI);
    }
    else if(angle < -static_cast<float>(M_PI)){
        return 2 * static_cast<float>(M_PI) + angle;
    }
    return angle;
}

float Univector::gaussian(float r, float d_const){
    return (1/d_const) * (1/sqrtf(2 * static_cast<float>(M_PI))) * exp(-(r * r/(2 * d_const * d_const)));
}

Geometry::Vector2D Univector::closestObstacle(Geometry::Vector2D pos, QList<Geometry::Vector2D> obstacles){
    if(obstacles.size() == 0) {
        return Geometry::Vector2D(-500000,-500000);
    }

    float smallest = -1;
    float index = -1;
    for(int i = 0; i < obstacles.size(); i++){
        QPair<float, float> delta = delta_axis(pos.x(), pos.y(), obstacles[i].x(), obstacles[i].y());
        float dist = norm(delta.first, delta.second);
        if(dist < smallest || smallest == -1){
            smallest = dist;
            index = i;
        }
    }
    return obstacles[index];
}

QPair<float, float> Univector::delta_axis(float x1, float y1, float x2, float y2){
    return QPair<float, float>(x2-x1, y2-y1);
}

float Univector::phiAuf(Geometry::Vector2D obs_pos,  Geometry::Vector2D robot_pos, float robot_obs_dist, QPair<float, float> v_rob, QPair<float, float> v_obs){
    // Subtracao pode estar causando erro na movimentacao do robo
    QPair<float, float> s_vec = delta_axis(v_rob.first, v_rob.second, v_obs.first, v_obs.second);
    float s_norm = norm(s_vec.first * _ko, s_vec.second * _ko);

    QPair<float, float> p_line = {0, 0};
    if(robot_obs_dist >= s_norm){
        p_line = QPair<float, float>(obs_pos.x()+ s_vec.first, obs_pos.y() + s_vec.second);
    }
    else{
        p_line = QPair<float, float>(obs_pos.x() + robot_obs_dist * s_vec.first / s_norm, obs_pos.y() + robot_obs_dist * s_vec.second / s_norm);
    }

    QPair<float, float> delta = delta_axis(p_line.first, p_line.second, robot_pos.x(), robot_pos.y());
    float phi_auf = phiRepulsive(delta.first, delta.second);

    return wrapToPi(phi_auf);
}

double Univector::phiTuf(float theta, float d_x, float d_y){
    bool flag = true;
    if(Suassuna::Constants::teamPlaySide() == Common::Enums::Side::SIDE_LEFT){
        flag = false;
    }

    float y_l = d_y + _radius;
    float y_r = d_y - _radius;

    float ro_l = norm(d_x, d_y - _radius);
    float ro_r = norm(d_x, d_y + _radius);
    //std::cout << "ro_l: " << ro_l << " ro_r: " << ro_r << " radius: " << radius << std::endl;

    bool cw = flag;
    float phi_ccw = phiHyperbolic(ro_l, theta, cw);
    cw = !flag;
    float phi_cw = phiHyperbolic(ro_r, theta, cw);

    QVector<float> nh_ccw = Nh(phi_ccw);
    QVector<float> nh_cw = Nh(phi_cw);

    QVector<float> merge;

    for(int i = 0; i < nh_ccw.size(); i++){
        merge.push_back((fabs(y_l) * nh_ccw[i] + fabs(y_r) * nh_cw[i])/(2 * _radius));
    }

    float phiTuf = 0;

    if(-_radius <= d_y && d_y < _radius){
        phiTuf = atan2(merge[1], merge[0]);
    }
    else if(d_y < -_radius){
        phiTuf = phiHyperbolic(ro_l, theta, !flag);
    }
    else{
        phiTuf = phiHyperbolic(ro_r, theta, flag);
    }

    return wrapToPi(phiTuf);
}

double Univector::phiHyperbolic(float rho, float theta, bool cw){
    double angle = 0;
    if (rho > _radius){
        angle = static_cast<float>(M_PI)/2 * (2 - (_radius + _kr)/(rho + _kr));
    } else if(rho >= 0 && rho <= _radius){
        angle = static_cast<float>(M_PI)/2 * sqrt(rho/_radius);
    }

    if (cw){
        return wrapToPi(angle + theta);
    } else{
        return wrapToPi(theta - angle);
    }
}

double Univector::phiRepulsive(float dx, float dy){
    return atan2(dy, dx);
}

double Univector::phiComposed(float phi_tuf,
                              float phi_auf,
                              float R,
                              QList<Geometry::Vector2D> obstacles){
    float phi_composed = 0;
    if (obstacles.size() == 0){
        phi_composed = phi_tuf;
    } else {
        float gauss = gaussian(R - _dmin, _delta);

        if(R <= _dmin){
            phi_composed = phi_auf;
        }
        else{
            phi_composed = wrapToPi(phi_auf * gauss + phi_tuf * (1.0f - gauss));

//            float diff = wrapToPi(phi_auf - phi_tuf);
//            phi_composed = wrapToPi(gauss * diff + phi_tuf);
        }
    }
    return wrapToPi(phi_composed);
}

QVector<float> Univector::generateUnivectorField(Geometry::Vector2D robot_pos,
                                                 Geometry::Vector2D target_pos,
                                                 QList<Geometry::Vector2D> obstacles,
                                                 QPair<float, float> v_rob,
                                                 QPair<float, float> v_obs){

    QPair<float, float> d_target = delta_axis(target_pos.x(),
                                              target_pos.y(),
                                              robot_pos.x(),
                                              robot_pos.y());

    float theta = phiRepulsive(d_target.first, d_target.second);
    float phi_tuf = phiTuf(theta, d_target.first, d_target.second);

    Geometry::Vector2D obstacle = closestObstacle(robot_pos, obstacles);

    QPair<float, float> robot_obs_delta = delta_axis(obstacle.x(),
                                                     obstacle.y(),
                                                     robot_pos.x(),
                                                     robot_pos.y());

    float robot_obs_dist = norm(robot_obs_delta.first, robot_obs_delta.second);

    float phi_auf = phiAuf(obstacle, robot_pos, robot_obs_dist, v_rob, v_obs);
    float phi_composed = phiComposed(phi_tuf, phi_auf, robot_obs_dist, obstacles);

    QVector<float> nh_phi = Nh(phi_composed);
    QVector<Geometry::Vector2D> vectors;
    for (float angle : nh_phi) {
        vectors.push_back(Geometry::Vector2D(Geometry::Angle(angle), 1.0f));
    }
    return nh_phi;
}

QVector<float> Univector::generateHyperbolicField(Geometry::Vector2D target_pos, Geometry::Vector2D robot_pos){

    QPair<float, float> delta = delta_axis(target_pos.x(), target_pos.y(), robot_pos.x(), robot_pos.y());
    float theta = phiRepulsive(delta.first, delta.second);
    float rho = norm(delta.first, delta.second);
    float phi_h = phiHyperbolic(rho, theta, 0);

    return Nh(phi_h);
}
