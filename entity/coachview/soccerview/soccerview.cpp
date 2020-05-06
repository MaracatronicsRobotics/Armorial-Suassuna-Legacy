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
\file    soccerview.cpp
\brief   C++ Implementation: GLSoccerView
\author  Joydeep Biswas (C) 2011
*/
//========================================================================

#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "soccerview.h"
#include "util/field_default_constants.h"
#include <utils/utils.hh>
#include <cmath>

#define METER2MM 1000.0

GLSoccerView::FieldDimensions::FieldDimensions() :
  field_length(FieldConstantsLarc2020::kFieldLength),
  field_width(FieldConstantsLarc2020::kFieldWidth),
  boundary_width(FieldConstantsLarc2020::kBoundaryWidth) {
  for (size_t i = 0; i < FieldConstantsLarc2020::kNumFieldLines; ++i) {
    lines.push_back(new FieldLine(FieldConstantsLarc2020::kFieldLines[i]));
  }
  for (size_t i = 0; i < FieldConstantsLarc2020::kNumFieldArcs; ++i) {
    arcs.push_back(new FieldCircularArc(FieldConstantsLarc2020::kFieldArcs[i]));
  }
}

GLSoccerView::GLSoccerView(QWidget* parent) :
    QGLWidget(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::SampleBuffers),parent) {
    viewScale = (fieldDim.field_length + fieldDim.boundary_width) / sizeHint().width();
    viewScale = max(viewScale, (fieldDim.field_width + fieldDim.boundary_width) / sizeHint().height());

    viewXOffset = viewYOffset = 0.0;
    setAutoFillBackground(false); //Do not let painter auto fill the widget's background: we'll do it manually through openGl
    connect(this, SIGNAL(postRedraw()), this, SLOT(redraw()));
    blueRobotShape = GL_INVALID_VALUE;
    yellowRobotShape = GL_INVALID_VALUE;
    greyRobotShape = GL_INVALID_VALUE;
    blueCircleRobotShape = GL_INVALID_VALUE;
    yellowCircleRobotShape = GL_INVALID_VALUE;
    greyCircleRobotShape = GL_INVALID_VALUE;
    QFont RobotIDFont = this->font();
    RobotIDFont.setWeight(QFont::Bold);
    RobotIDFont.setPointSize(80);
    glText = GLText(RobotIDFont);
    tLastRedraw = 0;

    updateDefaultFieldDimensions();
    resetView();
}

void GLSoccerView::updateDefaultFieldDimensions() {
    fieldDim.field_length = FieldConstantsLarc2020::kFieldLength;
    fieldDim.field_width = FieldConstantsLarc2020::kFieldWidth;
    fieldDim.boundary_width = FieldConstantsLarc2020::kBoundaryWidth;
    fieldDim.lines.clear();
    for(size_t i=0; i<FieldConstantsLarc2020::kNumFieldLines; i++)
        fieldDim.lines.push_back(new FieldLine(FieldConstantsLarc2020::kFieldLines[i]));

    fieldDim.arcs.clear();

    for(size_t i=0; i<FieldConstantsLarc2020::kNumFieldArcs; i++)
        fieldDim.arcs.push_back(new FieldCircularArc(FieldConstantsLarc2020::kFieldArcs[i]));

}

void GLSoccerView::updateFieldGeometry(Locations *loc) {
    graphicsMutex.lock();

    const double kFieldLength = fieldDim.field_length = loc->fieldLength()*METER2MM;
    const double kFieldWidth = fieldDim.field_width = loc->fieldWidth()*METER2MM;
    const double kGoalWidth = 2*fabs(loc->ourGoalLeftPost().y())*METER2MM;
    const double kGoalDepth = loc->fieldGoalDepth()*METER2MM;
    fieldDim.boundary_width = 500.0;
    const double kCenterRadius = loc->fieldCenterRadius()*METER2MM;
    const double kDefenseRadius = loc->fieldDefenseRadius()*METER2MM;
    const double kDefenseStretch = loc->fieldDefenseStretch()*METER2MM;
    const double kDefenseWidth = loc->fieldDefenseWidth()*METER2MM;
    const double kLineThickness = 20.0;

    const double kXMax = (kFieldLength-2*kLineThickness)/2;
    const double kXMin = -kXMax;
    const double kYMax = (kFieldWidth-kLineThickness)/2;
    const double kYMin = -kYMax;

    QList<FieldLine> fieldLines;
    QList<FieldCircularArc> fieldArcs;

    // Field Lines
    fieldLines.append(FieldLine("TopTouchLine", kXMin-kLineThickness/2, kYMax, kXMax+kLineThickness/2, kYMax, kLineThickness));
    fieldLines.append(FieldLine("BottomTouchLine", kXMin-kLineThickness/2, kYMin, kXMax+kLineThickness/2, kYMin, kLineThickness));
    fieldLines.append(FieldLine("LeftGoalLine", kXMin, kYMin, kXMin, kYMax, kLineThickness));
    fieldLines.append(FieldLine("RightGoalLine", kXMax, kYMin, kXMax, kYMax, kLineThickness));
    fieldLines.append(FieldLine("HalfwayLine", 0, kYMin, 0, kYMax, kLineThickness));
    fieldLines.append(FieldLine("CenterLine", kXMin, 0, kXMax, 0, kLineThickness));
    fieldLines.append(FieldLine("LeftPenaltyStretch", kXMin+kDefenseRadius-kLineThickness/2, -kDefenseStretch/2, kXMin+kDefenseRadius-kLineThickness/2, kDefenseStretch/2, kLineThickness));
    fieldLines.append(FieldLine("RightPenaltyStretch", kXMax-kDefenseRadius+kLineThickness/2, -kDefenseStretch/2, kXMax-kDefenseRadius+kLineThickness/2, kDefenseStretch/2, kLineThickness));

    // Center circle
    fieldArcs.append(FieldCircularArc("CenterCircle", 0, 0, kCenterRadius-kLineThickness/2, 0, 2*M_PI, kLineThickness));

    // Goal lines
    fieldLines.append(FieldLine("LeftGoalLeftLine", kXMin, kGoalWidth/2, kXMin-kGoalDepth-kLineThickness/2, kGoalWidth/2, kLineThickness));
    fieldLines.append(FieldLine("LeftGoalRightLine", kXMin, -kGoalWidth/2, kXMin-kGoalDepth-kLineThickness/2, -kGoalWidth/2, kLineThickness));
    fieldLines.append(FieldLine("RightGoalLeftLine", kXMax, kGoalWidth/2, kXMax+kGoalDepth+kLineThickness/2, kGoalWidth/2, kLineThickness));
    fieldLines.append(FieldLine("RightGoalRightLine", kXMax, -kGoalWidth/2, kXMax+kGoalDepth+kLineThickness/2, -kGoalWidth/2, kLineThickness));

    fieldLines.append(FieldLine("LeftGoalStretch", kXMin-kGoalDepth, -kGoalWidth/2, kXMin-kGoalDepth, kGoalWidth/2, kLineThickness));
    fieldLines.append(FieldLine("RightGoalStretch", kXMax+kGoalDepth, -kGoalWidth/2, kXMax+kGoalDepth, kGoalWidth/2, kLineThickness));

    // Goal area
    if(loc->fieldDefenseRoundedRadius() == 0.0) { // Squared goal area
        fieldLines.append(FieldLine("LeftFieldLeftDefenseLine", kXMin, kDefenseStretch/2, kXMin+kDefenseRadius+kLineThickness/2, kDefenseStretch/2, kLineThickness));
        fieldLines.append(FieldLine("LeftFieldRightDefenseLine", kXMin, -kDefenseStretch/2, kXMin+kDefenseRadius+kLineThickness/2, -kDefenseStretch/2, kLineThickness));
        fieldLines.append(FieldLine("RightFieldLeftDefenseLine", kXMax, kDefenseStretch/2, kXMax-kDefenseRadius-kLineThickness/2, kDefenseStretch/2, kLineThickness));
        fieldLines.append(FieldLine("RightFieldRightDefenseLine", kXMax, -kDefenseStretch/2, kXMax-kDefenseRadius-kLineThickness/2, -kDefenseStretch/2, kLineThickness));
    }

    // Add new lines
    fieldDim.lines.clear();
    for(int i=0; i<fieldLines.size(); i++)
        fieldDim.lines.push_back(new FieldLine(fieldLines[i]));

    // Add new arcs
    fieldDim.arcs.clear();
    for(int i=0; i<fieldArcs.size(); i++)
        fieldDim.arcs.push_back(new FieldCircularArc(fieldArcs[i]));

    // Add new triangles
    fieldDim.triangles.clear();

    graphicsMutex.unlock();
    emit postRedraw();
}

void GLSoccerView::redraw()
{
    if(GetTimeSec()-tLastRedraw<MinRedrawInterval)
        return;
    graphicsMutex.lock();
    update();
    graphicsMutex.unlock();
    tLastRedraw = GetTimeSec();
}


void GLSoccerView::mousePressEvent(QMouseEvent* event)
{
    leftButton = event->buttons().testFlag(Qt::LeftButton);
    midButton = event->buttons().testFlag(Qt::MidButton);
    rightButton = event->buttons().testFlag(Qt::RightButton);
//  bool shiftKey = event->modifiers().testFlag(Qt::ShiftModifier);
//  bool ctrlKey = event->modifiers().testFlag(Qt::ControlModifier);

    if(leftButton)
        setCursor(Qt::ClosedHandCursor);
    if(midButton)
        setCursor(Qt::SizeVerCursor);
    if(leftButton || midButton) {
        // Start Pan / Zoom
        mouseStartX = event->x();
        mouseStartY = event->y();
        redraw();
    }
}

void GLSoccerView::mouseReleaseEvent(QMouseEvent* event)
{
//  bool shiftKey = event->modifiers().testFlag(Qt::ShiftModifier);
//  bool ctrlKey = event->modifiers().testFlag(Qt::ControlModifier);
    setCursor(Qt::ArrowCursor);
}

void GLSoccerView::mouseMoveEvent(QMouseEvent* event)
{
    static const bool debug = false;
    bool leftButton = event->buttons().testFlag(Qt::LeftButton);
    bool midButton = event->buttons().testFlag(Qt::MidButton);
    bool rightButton = event->buttons().testFlag(Qt::RightButton);

    if(debug) printf("MouseMove Event, Left:%d Mid:%d Right:%d\n", leftButton?1:0, midButton?1:0, rightButton?1:0);
    if(leftButton) {
        //Pan
        viewXOffset -= viewScale*double(event->x() - mouseStartX);
        viewYOffset += viewScale*double(event->y() - mouseStartY);
        mouseStartX = event->x();
        mouseStartY = event->y();
        recomputeProjection();
        redraw();
    } else if(midButton) {
        //Zoom
        double zoomRatio = double(event->y() - mouseStartY)/500.0;
        //    double oldScale = viewScale;
        viewScale = viewScale*(1.0+zoomRatio);
        recomputeProjection();
        mouseStartX = event->x();
        mouseStartY = event->y();
        redraw();
    }
}

void GLSoccerView::wheelEvent(QWheelEvent* event)
{
  static const bool debug = false;
  double zoomRatio = -double(event->delta())/1000.0;
//  double oldScale = viewScale;
  viewScale = viewScale*(1.0+zoomRatio);
  recomputeProjection();
  if(debug) printf("Zoom: %5.3f\n",viewScale);
  redraw();
}

void GLSoccerView::keyPressEvent(QKeyEvent* event)
{
  static const bool debug = false;
  if(debug) printf("KeyPress: 0x%08X\n",event->key());
  if(event->key() == Qt::Key_Space)
    resetView();
  if(event->key() == Qt::Key_Escape)
    close();
}

void GLSoccerView::resetView()
{
    viewScale = (fieldDim.field_length + fieldDim.boundary_width) / width();
    viewScale = max(viewScale, (fieldDim.field_width + fieldDim.boundary_width) / height());

    viewXOffset = viewYOffset = 0.0;
    recomputeProjection();
    redraw();
}

void GLSoccerView::resizeEvent(QResizeEvent* event)
{
    QGLWidget::resizeEvent(event);
    redraw();

    resetView();
}

void GLSoccerView::recomputeProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5*viewScale*width()+viewXOffset, 0.5*viewScale*width()+viewXOffset, -0.5*viewScale*height()+viewYOffset, 0.5*viewScale*height()+viewYOffset, minZValue, maxZValue);
    glMatrixMode(GL_MODELVIEW);
}

void GLSoccerView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    recomputeProjection();
}

void GLSoccerView::initializeGL()
{
    blueRobotShape = glGenLists(1);
    if(blueRobotShape==GL_INVALID_VALUE) {
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(blueRobotShape, GL_COMPILE);
    drawRobot(teamBlue,true,false);
    glEndList();

    yellowRobotShape = glGenLists(1);
    if(yellowRobotShape==GL_INVALID_VALUE) {
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(yellowRobotShape, GL_COMPILE);
    drawRobot(teamYellow,true,false);
    glEndList();

    greyRobotShape = glGenLists(1);
    if(greyRobotShape==GL_INVALID_VALUE) {
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(greyRobotShape, GL_COMPILE);
    drawRobot(teamUnknown,true,false);
    glEndList();

    blueCircleRobotShape = glGenLists(1);
    if(blueRobotShape==GL_INVALID_VALUE) {
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(blueCircleRobotShape, GL_COMPILE);
    drawRobot(teamBlue,false,false);
    glEndList();

    yellowCircleRobotShape = glGenLists(1);
    if(yellowRobotShape==GL_INVALID_VALUE) {
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(yellowCircleRobotShape, GL_COMPILE);
    drawRobot(teamYellow,false,false);
    glEndList();

    greyCircleRobotShape = glGenLists(1);
    if(greyRobotShape==GL_INVALID_VALUE) {
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(greyCircleRobotShape, GL_COMPILE);
    drawRobot(teamUnknown,false,false);
    glEndList();
}


void GLSoccerView::vectorTextTest()
{
    #define TextTest(loc,angle,size,str,halign,valign) \
    {glText.drawString((loc),angle,size,str,halign,valign); \
    vector2d l1,l2; \
    l1.heading(M_PI/180.0*angle); \
    l1 = loc+l1*size*(glText.getWidth(str)); \
    l2.heading(M_PI/180.0*angle); \
    l2 = loc-l2*size*(glText.getWidth(str)); \
    glBegin(GL_LINES); \
    glVertex3d(l1.x,l1.y,9); \
    glVertex3d(l2.x,l2.y,9); \
    glEnd();}

    glColor3d(1,1,1);
    TextTest(vector2d(1,1)*353.6,45,500,"123agdo0",GLText::LeftAligned,GLText::MedianAligned)
    TextTest(vector2d(fieldDim.field_length*0.5,0),0,500,"123agdo0",GLText::RightAligned,GLText::BottomAligned)
    TextTest(vector2d(0,-fieldDim.field_width*0.5),0,500,"123agdo0",GLText::CenterAligned,GLText::TopAligned)
    TextTest(vector2d(-fieldDim.field_length*0.5,0),0,500,"1\ub023agdo0",GLText::CenterAligned,GLText::MiddleAligned)
}

void GLSoccerView::paintEvent(QPaintEvent* event)
{
    graphicsMutex.lock();
    makeCurrent();
    glClearColor(FIELD_COLOR);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    drawFieldLines(fieldDim);
    drawRobots();
    drawRobotsVelocities();
    drawRobotsNextPositions();
    drawBalls();
    drawBallsVelocities();
    //vectorTextTest();
    glPopMatrix();
    swapBuffers();
    graphicsMutex.unlock();
}

void GLSoccerView::drawQuad(vector2d loc1, vector2d loc2, double z)
{
    glBegin(GL_QUADS);
    glVertex3d(loc1.x,loc1.y,z);
    glVertex3d(loc2.x,loc1.y,z);
    glVertex3d(loc2.x,loc2.y,z);
    glVertex3d(loc1.x,loc2.y,z);
    glEnd();
}

void GLSoccerView::drawQuad(vector2d v1, vector2d v2, vector2d v3, vector2d v4, double z)
{
    glBegin(GL_QUADS);
    glVertex3d(v1.x,v1.y,z);
    glVertex3d(v2.x,v2.y,z);
    glVertex3d(v3.x,v3.y,z);
    glVertex3d(v4.x,v4.y,z);
    glEnd();
}

void GLSoccerView::drawArc(vector2d loc, double r1, double r2, double theta1, double theta2, double z, double dTheta)
{
    static const double tesselation = 1.0;
    if(dTheta<0) {
        dTheta = tesselation/r2;
    }
    glBegin(GL_QUAD_STRIP);
    for(double theta=theta1; theta<theta2; theta+=dTheta) {
        double c1 = cos(theta), s1 = sin(theta);
        glVertex3d(r2*c1+loc.x,r2*s1+loc.y,z);
        glVertex3d(r1*c1+loc.x,r1*s1+loc.y,z);
    }
    double c1 = cos(theta2), s1 = sin(theta2);
    glVertex3d(r2*c1+loc.x,r2*s1+loc.y,z);
    glVertex3d(r1*c1+loc.x,r1*s1+loc.y,z);
    glEnd();
}

void GLSoccerView::drawTriangle(vector2d v1, vector2d v2, vector2d v3, double z) {
    glBegin(GL_TRIANGLES);
    glVertex3d(v1.x, v1.y, z);
    glVertex3d(v2.x, v2.y, z);
    glVertex3d(v3.x, v3.y, z);
    glEnd();
}

void GLSoccerView::drawRobot(int team, bool hasAngle, bool useDisplayLists)
{
  if(useDisplayLists) {
    switch (team) {
      case teamBlue: {
        if(hasAngle)
          glCallList(blueRobotShape);
        else
          glCallList(blueCircleRobotShape);
        break;
      }
      case teamYellow: {
        if(hasAngle)
          glCallList(yellowRobotShape);
        else
          glCallList(yellowCircleRobotShape);
        break;
      }
      default: {
        if(hasAngle)
          glCallList(greyRobotShape);
        else
          glCallList(greyCircleRobotShape);
        break;
      }
    }
    return;
  }

  switch (team) {
    case teamBlue: {
      glColor3d(0.2549, 0.4941, 1.0);
      break;
    }
    case teamYellow: {
      glColor3d(1.0, 0.9529, 0.2431);
      break;
    }
    default: {
      glColor3d(0.5882,0.5882,0.5882);
      break;
    }
  }
  double theta1 = hasAngle?RAD(40):0.0;
  double theta2 = 2.0*M_PI - theta1;
  drawArc(0,0,0,90,theta1, theta2, RobotZ);
  if(hasAngle){
      glBegin(GL_TRIANGLES);
      glVertex3d(0,0,RobotZ);
      glVertex3d(90.0*cos(theta1),90.0*sin(theta1),RobotZ);
      glVertex3d(90.0*cos(theta2),90.0*sin(theta2),RobotZ);
      glEnd();
  }


  switch ( team ){
    case teamBlue:{
      glColor3d(0.0706, 0.2314, 0.6275);
      break;
    }
    case teamYellow:{
      glColor3d(0.8, 0.6157, 0.0);
      break;
    }
    default:{
      glColor3d(0.2745,0.2745,0.2745);
      break;
    }
  }

  drawArc(0,0,80,90,theta1, theta2, RobotZ+0.01);

  if(hasAngle) {
      drawQuad(90.0*cos(theta1)-10,90.0*sin(theta1), 90.0*cos(theta2),90.0*sin(theta2),RobotZ+0.01);
  }
}

void GLSoccerView::drawRobot(vector2d loc, double theta, double conf, int robotID, int team, bool hasAngle)
{
  glPushMatrix();
  glLoadIdentity();
  glTranslated(loc.x,loc.y,0);

  switch ( team ){
    case teamBlue:{
      glColor3d(0.2549, 0.4941, 1.0);
      break;
    }
    case teamYellow:{
      glColor3d(1.0, 0.9529, 0.2431);
      break;
    }
    default:{
      glColor3d(0.5882,0.5882,0.5882);
      break;
    }
  }

  if(drawPlayerConfidency)
    drawQuad(-90,130,-90.0+180.0*conf,160,RobotZ);


  glColor3d(0.0,0.0,0.0);
  char buf[1024];
  if(robotID!=unknownRobotID)
    snprintf(buf,1023,"%d",robotID);
  else
    snprintf(buf,1023,"?");
    glText.drawString(loc,0,100,buf,GLText::CenterAligned,GLText::MiddleAligned);
  switch ( team ){
    case teamBlue:{
      glColor3d(0.0706, 0.2314, 0.6275);
      break;
    }
    case teamYellow:{
      glColor3d(0.8, 0.6157, 0.0);
      break;
    }
    default:{
      glColor3d(0.2745,0.2745,0.2745);
      break;
    }
  }

  if(drawPlayerConfidency){
      drawQuad(-96,124,96.0,130,RobotZ+0.01);
      drawQuad(-96,124,-90.0,166,RobotZ+0.01);
      drawQuad(-96,160,96.0,166,RobotZ+0.01);
      drawQuad(90,124,96.0,166,RobotZ+0.01);
  }


  glRotated(theta,0,0,1.0);
  drawRobot(team, hasAngle, true);
  glPopMatrix();
}

void GLSoccerView::drawFieldLines(FieldDimensions& dimensions)
{
    glColor4f(FIELD_LINES_COLOR);
    for (size_t i = 0; i < fieldDim.lines.size(); ++i) {
        const FieldLine& line = *fieldDim.lines[i];
        const double half_thickness = 0.5 * line.thickness;
        const vector2d p1(line.p1_x, line.p1_y);
        const vector2d p2(line.p2_x, line.p2_y);
        const vector2d perp = (p2 - p1).norm().perp();
        //    const vector2d corner1 = p1 - half_thickness * perp;
        //    const vector2d corner2 = p2 + half_thickness * perp;
        //    drawQuad(corner1, corner2, FieldZ);
        const vector2d v1 = p1 - half_thickness * perp;
        const vector2d v2 = p1 + half_thickness * perp;
        const vector2d v3 = p2 + half_thickness * perp;
        const vector2d v4 = p2 - half_thickness * perp;
        drawQuad(v1, v2, v3, v4, FieldZ);
    }

    for (size_t i = 0; i < fieldDim.arcs.size(); ++i) {
        const FieldCircularArc& arc = *fieldDim.arcs[i];
        const double half_thickness = 0.5 * arc.thickness;
        const double radius = arc.radius;
        const vector2d center(arc.center_x, arc.center_y);
        const double a1 = arc.a1;
        const double a2 = arc.a2;
        drawArc(center, radius - half_thickness, radius + half_thickness, a1, a2, FieldZ);
    }

    for(size_t i=0; i<fieldDim.triangles.size(); i++) {
        const FieldTriangle& triangle = *fieldDim.triangles[i];
        const vector2d v1(triangle.p1_x, triangle.p1_y);
        const vector2d v2(triangle.p2_x, triangle.p2_y);
        const vector2d v3(triangle.p3_x, triangle.p3_y);
        drawTriangle(v1, v2, v3, FieldZ);
    }
}

void GLSoccerView::drawBall(vector2d loc) {
    glColor3d(1.0,0.5059,0.0);
    drawArc(loc,0,16,-M_PI,M_PI,BallZ);
    glColor3d(0.8706,0.3490,0.0);
    drawArc(loc,15,21,-M_PI,M_PI,BallZ);
}

void GLSoccerView::drawVector(vector2d v1, vector2d v2, double z) {
    glColor3d(1.0, 0.0, 0.0);

    const vector2d norm = (v2 - v1).norm();
    const vector2d perp = (v2 - v1).norm().perp();

    // Line
    const double quad_half_thickness = 0.5 * 10;
    const vector2d qv1 = v1 - quad_half_thickness * perp;
    const vector2d qv2 = v1 + quad_half_thickness * perp;
    const vector2d qv3 = v2 + quad_half_thickness * perp;
    const vector2d qv4 = v2 - quad_half_thickness * perp;
    drawQuad(qv1, qv2, qv3, qv4, z);

    // Triangle
    const double triangle_thickness = 50;
    const vector2d tv1 = v2 + 2*triangle_thickness*norm;
    const vector2d tv2 = v2 + triangle_thickness*perp;
    const vector2d tv3 = v2 - triangle_thickness*perp;
    drawTriangle(tv1, tv2, tv3, z);
}

void GLSoccerView::drawBalls() {
    drawBall(ball);
}

void GLSoccerView::drawBallsVelocities() {
    if(ballVelocity.length()>0.05 && drawBallVel){
        drawVector(ball, ball+ballVelocity*100, BallZ+0.02);
        glPushAttrib(GL_ENABLE_BIT);
        glLineStipple(2, 0x0F0F);
        glLineWidth(1.0);
        glEnable(GL_LINE_STIPPLE);
        drawStippleLine(ball,ball+ballVelocity.norm()*10000);
    }
}

void GLSoccerView::drawRobots() {
    if(robots.empty())
        return;

    for(int j=0; j<robots.size(); j++){
      SoccerViewRobot r = robots.at(j);
      drawRobot(r.loc, r.angle, r.conf, r.id, r.team, r.hasAngle);
    }
}

void GLSoccerView::drawRobotsVelocities() {
    for (int i=0; i<robotsVelocities.size(); i++) {
        if(robotsVelocities[i].length()>0.025)
            drawVector(robots.at(i).loc, robots.at(i).loc+robotsVelocities[i]*100, RobotZ+0.02);
    }
}

void GLSoccerView::drawX(vector2d nextPos) {
    float halfThickness = 4;
    vector2d p1(nextPos.x - 37.5, nextPos.y + 37.5);
    vector2d p2(nextPos.x + 37.5, nextPos.y + 37.5);
    vector2d p3(nextPos.x + 37.5, nextPos.y - 37.5);
    vector2d p4(nextPos.x - 37.5, nextPos.y - 37.5);

    // First half of the X
    vector2d perp1 = (p1 - p3).norm().perp();
    vector2d v1 = p1 - halfThickness * perp1;
    vector2d v2 = p1 + halfThickness * perp1;
    vector2d v3 = p3 + halfThickness * perp1;
    vector2d v4 = p3 - halfThickness * perp1;

    // Second half of the X
    vector2d perp2 = (p2 - p4).norm().perp();
    vector2d v5 = p2 - halfThickness * perp2;
    vector2d v6 = p2 + halfThickness * perp2;
    vector2d v7 = p4 + halfThickness * perp2;
    vector2d v8 = p4 - halfThickness * perp2;


    drawQuad(v1, v2, v3, v4, 0.0);
    drawQuad(v5, v6, v7, v8, 0.0);
}

void GLSoccerView::drawStippleLine(vector2d nextPos, vector2d pos) {
    glBegin(GL_LINES);
    glVertex3d(pos.x, pos.y, 0.0);
    glVertex3d(nextPos.x, nextPos.y, 0.0);
    glEnd();
}

void GLSoccerView::drawRobotsNextPositions() {
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(2, 0x0F0F);
    glLineWidth(1.0);
    glEnable(GL_LINE_STIPPLE);
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	glEnable(GL_LINE_SMOOTH);
    for (int i = 0; i < robotsNextPositions.size(); i++) {
        if (robotsNextPositions.at(i).first == teamYellow) {
            glColor3d(1.0, 1.0, 0.0);
        } else {
            glColor3d(0.0, 0.0, 1.0);
        }
        drawX(robotsNextPositions.at(i).second);
//		drawStippleLine(robots.at(i).loc, robotsNextPositions.at(i).second);
    }
    glPopAttrib();
}

void GLSoccerView::updateDetection(MRCTeam *ourTeam, MRCTeam *theirTeam) {
    graphicsMutex.lock();
    QHash<quint8, Player*>::iterator it;
    QHash<quint8, Player*> ourAvPlayers = ourTeam->avPlayers();
    QHash<quint8, Player*> theirAvPlayers = theirTeam->avPlayers();

    robots.clear();
    robotsVelocities.clear();
    robotsNextPositions.clear();
    ballVelocity.set(0,0);

    vector2d velocity;
    SoccerViewRobot robot;

    for(it = ourAvPlayers.begin(); it != ourAvPlayers.end(); it++) {
        Player *player = it.value();

        vector2d next(player->nextPosition().x() * 1000.0, player->nextPosition().y() * 1000.0);

        robot.loc.set(player->position().x()*1000.0, player->position().y()*1000.0);
        robot.id = player->playerId();
        robot.hasAngle = true;
        robot.angle = DEG(player->orientation().value());
        robot.team = player->playerTeam()->teamColor()==Colors::YELLOW ? teamYellow : teamBlue;
        robot.cameraID = 0;
        robot.conf = 1.0;
        robots.append(robot);

        robotsNextPositions.append(std::make_pair(robot.team, next));

        if(drawAllieVel)
            velocity.set(player->velocity().x(), player->velocity().y());
        else
            velocity.set(0.0, 0.0);

        robotsVelocities.append(velocity);
    }

    for(it = theirAvPlayers.begin(); it != theirAvPlayers.end(); it++) {
        Player *player = it.value();

        robot.loc.set(player->position().x()*1000.0, player->position().y()*1000.0);
        robot.id = player->playerId();
        robot.hasAngle = true;
        robot.angle = DEG(player->orientation().value());
        robot.team = player->playerTeam()->teamColor()==Colors::YELLOW ? teamYellow : teamBlue;
        robot.cameraID = 0;
        robot.conf = 1.0;
        robots.append(robot);

        if(drawEnemyVel)
            velocity.set(player->velocity().x(), player->velocity().y());
        else
            velocity.set(0.0, 0.0);

        robotsVelocities.append(velocity);
    }

    ball.set(ourTeam->loc()->ball().x() * 1000.0, ourTeam->loc()->ball().y() * 1000.0);

    if(drawBallVel)
        ballVelocity.set(ourTeam->loc()->ballVelocity().x(), ourTeam->loc()->ballVelocity().y());

    graphicsMutex.unlock();
    emit postRedraw();
}
