#ifndef UNIVECTOR_H
#define UNIVECTOR_H

#include <math.h>
#include <QVector>
#include <QPair>

#include <src/entities/basesuassuna.h>
#include <src/common/constants/constants.h>

#define DE 200.0
#define KR 400.0
#define DELTA 100.0
#define DMIN 100.0
#define KO 0.12


class Univector
{
public:
    Univector(quint8 playerId);

    double norm(double d_x, double d_y);

    QVector<float> Nh(float phi);

    float wrapToPi(float angle);

    float gaussian(float r, float d_const);

    Geometry::Vector2D closestObstacle(Geometry::Vector2D pos, QList<Geometry::Vector2D> obstacles);

    QPair<float, float> delta_axis(float x1, float y1, float x2, float y2);

    float phiAuf(Geometry::Vector2D obs_pos,  Geometry::Vector2D robot_pos, float robot_obs_dist, QPair<float, float> v_rob, QPair<float, float> v_obs);

    double phiTuf(float theta, float d_x, float d_y);

    double phiHyperbolic(float rho, float theta, bool cw);

    double phiRepulsive(float dx, float dy);

    double phiComposed(float phi_tuf, float phi_auf, float R, QList<Geometry::Vector2D> obstacles);

    QVector<float> generateUnivectorField(Geometry::Vector2D robot_pos, Geometry::Vector2D target_pos, QList<Geometry::Vector2D> obstacles, QPair<float, float> v_rob, QPair<float, float> v_obs);

    QVector<float> generateHyperbolicField(Geometry::Vector2D target_pos, Geometry::Vector2D robot_pos);

private:
    float _radius;
    float _kr;
    float _delta;
    float _dmin;
    float _ko;

    float _playerOri;
    quint8 _playerId;

};

#endif // UNIVECTOR_H
