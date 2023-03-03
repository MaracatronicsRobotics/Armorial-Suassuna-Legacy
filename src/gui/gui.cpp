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

#include "gui.h"
#include "ui_gui.h"

GUI::GUI(QWidget *parent) : QWidget(parent), ui(new Ui::GUI) {
    ui->setupUi(this);

    // Update theme
    setDarkTheme();

    // Setup timer
    _timer = new QTimer(this);
    _timer->setInterval(1000.0 / 60.0);
    _timer->setSingleShot(false);
    _timer->callOnTimeout([this](){
        _fieldView->redraw();
    });
    _timer->start();

    // Setup deafult field and create field view
    Common::Types::Field field_vssA(Common::Enums::Side::SIDE_LEFT, 250.0, 1500.0, 1300.0, 100.0, 400.0, 150.0, 700.0, 112.5);
    ui->fieldViewLayout->addWidget(_fieldView = new FieldView(field_vssA, "", this));

    QStringList nameList;
    for (int i = 0; i < Suassuna::Constants::maxNumPlayers(); i++) {
        nameList.push_back(QString("Robot %1").arg(i));
    }
    ui->comboBox->addItems(nameList);

    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(setRobotChoice(QString)));
    connect(ui->checkTargetPos, SIGNAL(toggled(bool)), this, SLOT(setShowInterestPoints(bool)));
    connect(ui->checkNavigation, SIGNAL(toggled(bool)), this, SLOT(setShowPath(bool)));
}

GUI::~GUI() {
    delete ui;
}

void GUI::setDarkTheme() {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
    darkPalette.setColor(QPalette::Base,QColor(42,42,42));
    darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
    darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
    darkPalette.setColor(QPalette::ToolTipText,Qt::white);
    darkPalette.setColor(QPalette::Text,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
    darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
    darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
    darkPalette.setColor(QPalette::Button,QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
    darkPalette.setColor(QPalette::BrightText,Qt::red);
    darkPalette.setColor(QPalette::Link,QColor(42,130,218));
    darkPalette.setColor(QPalette::Highlight,QColor(42,130,218));
    darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
    darkPalette.setColor(QPalette::HighlightedText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));

    this->setPalette(darkPalette);
}

void GUI::updateBalls(const QList<Armorial::Ball>& balls) {
    _fieldView->updateBalls(balls);
}

void GUI::updateRobots(const QList<Armorial::Robot>& robots) {
    _fieldView->updateRobots(robots);
}

void GUI::updateFieldGeometry(const Common::Types::Field& fieldGeometry) {
    _fieldView->updateFieldGeometry(fieldGeometry);
}

void GUI::updateInterestPoints(quint8 robotId, const QList<Geometry::Vector2D>& points) {
    _fieldView->updateInterestPoints(robotId, points);
}

void GUI::updateVectorsAngles(quint8 robotId, const QVector<Geometry::Vector2D>& angles) {
    _fieldView->updateVectorsAngles(robotId, angles);
}

void GUI::setRobotChoice(const QString& robotId) {
    for (quint8 i = 0; i < Suassuna::Constants::maxNumPlayers(); i++) {
        if (robotId == QString("Robot %1").arg(i)) {
            _fieldView->setRobotChoice(i);
        }
    }
}

void GUI::setShowInterestPoints(const bool& toShow) {
    _fieldView->setShowInterestPoints(toShow);
}

void GUI::setShowPath(const bool& toShow) {
    _fieldView->setShowPath(toShow);
}
