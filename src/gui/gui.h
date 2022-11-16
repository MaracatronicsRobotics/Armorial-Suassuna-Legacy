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

#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QLabel>

#include <src/entities/worldmap/worldmap.h>
#include <src/entities/referee/sslreferee.h>

#include <Armorial/Geometry/Vector2D/Vector2D.h>

#include <Armorial/Common/Widgets/Widgets.h>
#include <Armorial/Common/Widgets/FieldView/FieldView.h>
#include <Armorial/Common/Widgets/GLText/GLText.h>


namespace Ui {
class GUI;
}
class RobotFrame;

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(WorldMap *worldMap, VSSReferee *referee, QWidget *parent = nullptr);
    ~GUI();

private:
    Ui::GUI *_ui;
    void setDarkTheme();

    // Tree management
    QMap<quint8, QTreeWidgetItem*> _players;
    QMap<QTreeWidgetItem*, QTreeWidgetItem*> _playerRole;
    Geometry::Vector2D _defPos;
    QPixmap getRobotPixMap(quint8 playerID);
    QPixmap getRolePixMap(QString roleName);
    void treeInitialSetup();
    void updatePlaybookName(QString name);
    void updatePlayers(QList<quint8> &players);

    // Frames management
    QList<RobotFrame*> _robotFrames;
    void loadRobotFrames();

    // WorldMap
    WorldMap *_worldMap;
    WorldMap* getWorldMap();

    // VSSSReferee
    VSSReferee *_referee;
    VSSReferee* getReferee();
};

#endif // GUI_H
