/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "fieldview.h"

#include <spdlog/spdlog.h>

#include <src/common/utils/utils.h>

FieldView::FieldView(const Common::Types::Field& field, const QString& centralLogoPath, QWidget* parent) : Common::Widgets::FieldView(field, centralLogoPath, parent) {
    // Setup quadrants as enabled by default
    magic_enum::enum_for_each<Common::Enums::Quadrant>([this] (Common::Enums::Quadrant quadrant) {
        _quadrantStatus.insert(quadrant, true);
    });

    this->setupIdSize(60);
}

void FieldView::draw() {
    _drawMutex.lock();

    // Draw field lines
    drawFieldLines();

    // Draw disabled quadrants
    magic_enum::enum_for_each<Common::Enums::Quadrant>([this] (Common::Enums::Quadrant quadrant) {
        glColor4d(1.0, 0.0, 0.0, 0.2);
        Geometry::Rectangle quadrantGeometry({0.0, 0.0}, {0.0, 0.0});

        if(quadrant == Common::Enums::QUADRANT_TOP_LEFT) quadrantGeometry = getFieldGeometry().topLeftQuadrant();
        else if(quadrant == Common::Enums::QUADRANT_TOP_RIGHT) quadrantGeometry = getFieldGeometry().topRightQuadrant();
        else if(quadrant == Common::Enums::QUADRANT_BOTTOM_LEFT) quadrantGeometry = getFieldGeometry().bottomLeftQuadrant();
        else if(quadrant == Common::Enums::QUADRANT_BOTTOM_RIGHT) quadrantGeometry = getFieldGeometry().bottomRightQuadrant();

        if(_quadrantStatus[quadrant] == false) {
            drawRect(quadrantGeometry.topLeft(), quadrantGeometry.bottomRight(), 1.0);
        }
    });

    // Draw received balls
    for(auto it = _balls.begin(); it != _balls.end(); it++) {
        Armorial::Position ballPosition = (*it).ballposition();
        drawBall(Suassuna::Utils::convertCoordinatesToVector2D(ballPosition));
    }

    // Draw received robots
    for(auto it = _robots.begin(); it != _robots.end(); it++) {
        Armorial::Position robotPosition = (*it).robotposition();
        Armorial::RobotIdentifier robotIdentifier = (*it).robotidentifier();
        Common::Enums::Color robotTeamColor = (robotIdentifier.robotcolor().isblue() ? Common::Enums::Color::BLUE : Common::Enums::Color::YELLOW);
        drawRobot(Suassuna::Utils::convertCoordinatesToVector2D(robotPosition),
                  (*it).robotorientation().value(), robotTeamColor, robotIdentifier.robotid());
    }

    _drawMutex.unlock();
}

void FieldView::updateBalls(const QList<Armorial::Ball>& balls) {
    _drawMutex.lock();
    _balls = balls;
    _drawMutex.unlock();
}

void FieldView::updateRobots(const QList<Armorial::Robot>& robots) {
    _drawMutex.lock();
    _robots = robots;
    _drawMutex.unlock();
}

void FieldView::updateFieldGeometry(const Common::Types::Field& field) {
    _drawMutex.lock();
    setupFieldGeometry(field);
    resetView();
    _drawMutex.unlock();
}

void FieldView::updateQuadrantStatus(const Common::Enums::Quadrant &quadrant, bool status) {
    _drawMutex.lock();
    _quadrantStatus[quadrant] = status;
    _drawMutex.unlock();
}

void FieldView::setupRobotDisplayList() {
    magic_enum::enum_for_each<Common::Enums::Color>([this] (auto color) {
        // Check if key for color is already registered
        if(!_robotShape.keys().contains(color)) {
            _robotShape.insert(color, GL_INVALID_VALUE);
        }

        // Check if robot shape was been already set and delete if True
        if(_robotShape.value(color) != GL_INVALID_VALUE) {
            glDeleteLists(_robotShape.value(color), 1);
        }

        // Generate a display list
        _robotShape.insert(color, glGenLists(1));
        if(_robotShape.value(color) == GL_INVALID_VALUE) {
            spdlog::error("Failed to create robot shape display list.");
        }

        // Draw in the display list
        glNewList(_robotShape.value(color), GL_COMPILE);

        // Color for border
        if(color == Common::Enums::Color::BLUE)        glColor3d(0.2549, 0.4941, 1.0);
        else if(color == Common::Enums::Color::YELLOW) glColor3d(1.0, 0.9529, 0.2431);
        else                                           glColor3d(0.5882,0.5882,0.5882);

        // Drawing robot border
        drawRect(QVector2D(-35, 35), QVector2D(35, -35), _robotZ);

        // Color for robot body
        if(color == Common::Enums::Color::BLUE)        glColor3d(0.0706, 0.2314, 0.6275);
        else if(color == Common::Enums::Color::YELLOW) glColor3d(0.8, 0.6157, 0.0);
        else                                           glColor3d(0.2745,0.2745,0.2745);

        // Draw robot body
        drawRect(QVector2D(-40, 40), QVector2D(40, 35), _robotZ+0.01);
        drawRect(QVector2D(-40, -40), QVector2D(40, -35), _robotZ+0.01);
        drawRect(QVector2D(40, -40), QVector2D(35, 40), _robotZ+0.01);
        drawRect(QVector2D(-40, -40), QVector2D(-35, 40), _robotZ+0.01);

        glColor3d(0.5, 0.5, 0.5);

        drawRect(QVector2D(-20, 47), QVector2D(20, 40), _robotZ+0.02);
        drawRect(QVector2D(-20,-47), QVector2D(20, -40), _robotZ+0.02);

        glEndList();
    });
}
