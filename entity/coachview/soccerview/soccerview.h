//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
\file    soccerview.h
\brief   C++ Interface: GLSoccerView
\author  Joydeep Biswas (C) 2011
*/
//========================================================================

#ifndef SOCCERVIEW_H
#define SOCCERVIEW_H

#include <QMouseEvent>
#include <QWidget>
#include <QGLWidget>
#include <QMutex>
#include <QVector>
#include <GL/glu.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <cstdio>
#include "util/timer.h"
#include "util/geometry.h"
#include "util/field.h"
#include "util/util.h"
#include "gltext.h"

#include <entity/locations.h>
#include <include/timer.h>
#include <entity/player/player.h>
#include <entity/contromodule/mrcteam.h>

using namespace std;

#define FIELD_COLOR 0.0,0.5686,0.0980,1.0
#define FIELD_LINES_COLOR 1.0,1.0,1.0,1.0

#define PATHDRAWING_FRAMES 20
#define PATHDRAWING_FREQUENCY_HZ 20

class GLSoccerView : public QGLWidget {
    Q_OBJECT

public:
    struct FieldDimensions {
        vector<FieldLine*> lines;
        vector<FieldCircularArc*> arcs;
        vector<FieldTriangle*> triangles;
        double field_length;
        double field_width;
        double boundary_width;
        FieldDimensions();
    };

    struct SoccerViewRobot {
        bool hasAngle;
        vector2d loc;
        double angle;
        int id;
        double conf;
        int team;
        int cameraID;
    };

    typedef enum {
        teamUnknown = 0,
        teamBlue,
        teamYellow
    } TeamTypes;

    void setDrawAllieVel(bool status)         { drawAllieVel         = status; }
    void setDrawEnemyVel(bool status)         { drawEnemyVel         = status; }
    void setDrawBallVel(bool status)          { drawBallVel          = status; }
    void setDrawPlayerConfidency(bool status) { drawPlayerConfidency = status; }

private:
    static constexpr double minZValue = -10;
    static constexpr double maxZValue = 10;
    static constexpr double FieldZ = 1.0;
    static constexpr double RobotZ = 2.0;
    static constexpr double BallZ = 3.0;
    static const int PreferedWidth = 1024;
    static const int PreferedHeight = 768;
    static constexpr double MinRedrawInterval = 0.01; ///Minimum time between graphics updates (limits the fps)
    static const int unknownRobotID = -1;

    QVector<SoccerViewRobot> robots;
    QVector<vector2d> robotsVelocities;
    QVector<std::pair<int, vector2d> > robotsNextPositions;
    vector2d ball;
    vector2d ballVelocity;
    QMutex graphicsMutex;
    GLText glText;

    GLuint blueRobotShape;
    GLuint yellowRobotShape;
    GLuint greyRobotShape;
    GLuint blueCircleRobotShape;
    GLuint yellowCircleRobotShape;
    GLuint greyCircleRobotShape;

    double viewScale; /// Ratio of world space to screen space coordinates
    double viewXOffset;
    double viewYOffset;

    bool leftButton;
    bool midButton;
    bool rightButton;
    int mouseStartX;
    int mouseStartY;

    bool drawAllieVel;
    bool drawEnemyVel;
    bool drawBallVel;
    bool drawPlayerConfidency;

    double tLastRedraw;

    FieldDimensions fieldDim;

private:
    void drawFieldLines(FieldDimensions &dimensions);
    void drawRobots();
    void drawRobotsVelocities();
    void drawRobotsNextPositions();
    void drawBalls();
    void drawBallsVelocities();
    void drawX(vector2d nextPos);
    void drawStippleLine(vector2d pos, vector2d nextPos);
    void drawQuad(vector2d loc1, vector2d loc2, double z=0.0);
    void drawQuad(double x1, double y1, double x2, double y2, double z=0.0){drawQuad(vector2d(x1,y1),vector2d(x2,y2),z);}
    void drawQuad(vector2d v1, vector2d v2, vector2d v3, vector2d v4, double z);
    void drawArc(vector2d loc, double r1, double r2, double theta1, double theta2, double z=0.0, double dTheta = -1);
    void drawArc(double x, double y, double r1, double r2, double theta1, double theta2, double z=0.0, double dTheta = -1){drawArc(vector2d(x,y),r1,r2,theta1,theta2,z,dTheta);}
    void drawTriangle(vector2d v1, vector2d v2, vector2d v3, double z);
    void recomputeProjection();
    void drawRobot(vector2d loc, double theta, double conf, int robotID, int team, bool hasAngle);
    void drawRobot(int team, bool hasAngle, bool useDisplayLists);
    void drawBall(vector2d loc);
    void drawVector(vector2d v1, vector2d v2, double z);
    void vectorTextTest();

    void updateDefaultFieldDimensions();

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void initializeGL();
    void resizeGL(int width, int height);
    QSize sizeHint() const { return QSize(PreferedWidth,PreferedHeight); }

public:
    GLSoccerView(QWidget *parent = 0);
    void updateDetection(MRCTeam *ourTeam, MRCTeam *theirTeam);
    void updateFieldGeometry(Locations *loc);

public slots:
    void resetView();
private slots:
    void redraw();
signals:
    void postRedraw();
};

#endif
