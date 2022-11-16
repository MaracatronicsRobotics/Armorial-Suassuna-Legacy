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

#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <QReadWriteLock>

#include <Armorial/Base/Client/Client.h>
#include <Armorial/Common/Enums/Enums.h>
#include <Armorial/Common/Types/Field/Field.h>
#include <Armorial/Threaded/Entity/Entity.h>

#include <include/proto/visionservice.grpc.pb.h>

class SSLTeam;

class WorldMap : public Threaded::Entity, Base::GRPC::Client<Armorial::Vision::VisionService>
{
public:
    /*!
     * \brief Constructor for the WorldMap class.
     * \param serviceAddress, servicePort The given parameters for the connection with the Vision Service.
     */
    WorldMap(QString serviceAddress, quint16 servicePort);

    void setupTeams(QMap<Common::Enums::Color, SSLTeam*>& teams);

    /*!
     * \return A Common::Types::Field filled with the locations and parameters of the field
     * which the team is playing.
     */
    Common::Types::Field getField();
    Common::Types::Object getBall();
    SSLTeam* getTeam(Common::Enums::Color);

    // Common::Types::Object getPlayer(const Common::Enums::Color& teamColor, quint8 playerId);

    // QList<Common::Types::Object> getPlayersFromTeam(const Common::Enums::Color &teamColor);

protected:
    /*!
     * \brief Update the Common::Types::Field internal variable based on the received data
     * from service and available constants.
     */
    void updateField();

    /*!
     * \brief Update the Common::Types::Object internal variable that stores the ball data.
     */
    void updateBall();

    /*!
     * \brief Update the list of Common::Types::Object internal variable that stores the players
     * positions and data.
     */
    void updatePlayers();
private:
    /*!
     * \brief initialization
     */
    void initialization();

    /*!
     * \brief loop
     */
    void loop();

    /*!
     * \brief finalization
     */
    void finalization();

    // Internal
    QMap<Common::Enums::Color, SSLTeam*> _teams;
    QReadWriteLock _mutex;

    Common::Types::Field _field;
    Common::Types::Object _ball;
};

#endif // WORLDMAP_H
