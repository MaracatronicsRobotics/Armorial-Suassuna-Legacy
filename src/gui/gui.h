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

#include <QTimer>
#include <QWidget>

#include <src/gui/fieldview/fieldview.h>

namespace Ui {
class GUI;
}

class GUI : public QWidget
{
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = nullptr);
    ~GUI();

private:
    Ui::GUI *ui;

    // FieldView
    FieldView *_fieldView;
    QTimer *_timer;

    // Theme
    void setDarkTheme();

public slots:
    void updateBalls(const QList<Armorial::Ball>& balls);
    void updateRobots(const QList<Armorial::Robot>& robots);
    void updateFieldGeometry(const Common::Types::Field& fieldGeometry);
};

#endif // GUI_H
