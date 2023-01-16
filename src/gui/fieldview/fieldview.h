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

#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QMap>

#include <Armorial/Common/Widgets/FieldView/FieldView.h>
#include <Armorial/Common/Enums/Quadrant/Quadrant.h>

#include <include/proto/messages.pb.h>

#include <src/common/types/robot/robot.h>

class FieldView : public Common::Widgets::FieldView
{
public:
    /*!
     * \brief FieldView constructor which uses the constructor of the Common::Widgets::FieldView as backbone.
     * \param field The field params which the application will start with.
     * \param centralLogoPath The central logo path which the application will load to be draw at the center.
     * \param parent The parent of this FieldView instance (generally used as the GUI which create this pointer).
     */
    FieldView(const Common::Types::Field& field, const QString& centralLogoPath = "", QWidget *parent = nullptr);

    /*!
     * \brief setBall
     * \param ball
     */
    void updateBalls(const QList<Armorial::Ball>& balls);

    /*!
     * \brief addRobot
     * \param robot
     */
    void updateRobots(const QList<Armorial::Robot>& robots);

    /*!
     * \brief updateFieldGeometry
     * \param field
     */
    void updateFieldGeometry(const Common::Types::Field& field);

    /*!
     * \brief updateQuadrantStatus
     * \param quadrant
     * \param status
     */
    void updateQuadrantStatus(const Common::Enums::Quadrant& quadrant, bool status);

protected:
    /*!
     * \brief Setup robot display list override (draw VSS robots instead of SSL)
     */
    void setupRobotDisplayList();

private:
    /*!
     * \brief draw
     */
    void draw();

    // Internal objects
    QList<Armorial::Ball> _balls;
    QList<Armorial::Robot> _robots;
    QMap<Common::Enums::Quadrant, bool> _quadrantStatus;
    QMutex _drawMutex;
};

#endif // FIELDVIEW_H
