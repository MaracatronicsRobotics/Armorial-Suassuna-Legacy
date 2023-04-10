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

#include <QObject>

#include <src/entities/basesuassuna.h>
#include <src/entities/coach/team/team.h>

class WorldMap : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor for the WorldMap class.
     */
    WorldMap();

    /*!
     * \brief Setup the internal pointers for the teams. This method is called via the Suassuna core interface.
     * \param teams The given teams.
     */
    void setupTeams(QMap<Common::Enums::Color, Team*>& teams);

    // Getters for internal objects
    Common::Types::Field getField();
    Common::Types::Object getBall();
    Team* getTeam(const Common::Enums::Color& teamColor);

private:
    // Internal objects
    QMap<Common::Enums::Color, Team*> _teams;
    Common::Types::Field _field;
    Common::Types::Object _ball;

    // Internal mutex for object management
    QReadWriteLock _mutex;

public slots:
    void updatePlayers(const QList<Armorial::Robot>& robots);
    void updateBall(const QList<Armorial::Ball>& balls);
    void updateField(const Common::Types::Field& field);
};

#endif // WORLDMAP_H
