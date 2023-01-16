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

#ifndef VISION_H
#define VISION_H

#include <Armorial/Base/Client/Client.h>
#include <Armorial/Common/Enums/Color/Color.h>
#include <Armorial/Common/Types/Field/Field.h>
#include <Armorial/Threaded/Entity/Entity.h>

#include <src/entities/vision/object/visionobject.h>

class Vision : public Threaded::Entity, public Base::UDP::Client
{
    Q_OBJECT
public:
    /*!
     * \brief Vision module class constructor.
     */
    Vision();

    /*!
     * \brief Vision module class destructor.
     */
    ~Vision();

private:
    /*!
     * \brief Entity inherited methods.
     */
    void initialization();
    void loop();
    void finalization();

    // Appeared control
    QMap<Common::Enums::Color, QMap<quint8, bool>*> _appeared;
    void resetAppearedControl();
    void updatePlayer(const Common::Enums::Color& teamColor, const quint8& playerId, const Geometry::Vector2D& pos, const Geometry::Angle& ori, const float& confidence);
    void updateNotAppearedRobotsFromColor(const Common::Enums::Color& teamColor);

    // Objects
    VisionObject _ball;
    QMap<Common::Enums::Color, QMap<quint8, VisionObject*>*> _robots;

signals:
    void sendRobots(const QList<Armorial::Robot>& robots);
    void sendBalls(const QList<Armorial::Ball>& balls);
    void sendField(const Common::Types::Field& field);
};

#endif // VISION_H
