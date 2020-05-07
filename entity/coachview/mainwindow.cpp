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

#include "mainwindow.h"
#include <build/tmp/moc/ui_mainwindow.h>
#include <bits/stdc++.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <chrono>
#include <QStyle>
#include <QStyleFactory>

void MainWindow::resetRobots(){
    for(quint8 x = 0; x < MRCConstants::_qtPlayers; x++){
        setPlayerBattery(x, 0);
        setPlayerKickCharge(x, 0);
        setPlayerRole(x, "none");
        setDribble(x, false);
        setRadioConnect(x, false);
    }
}

void MainWindow::enableRobot(quint8 id){
    if(!this->playerBoxes.at(id)->isEnabled()) this->playerBoxes.at(id)->setDisabled(false);
    else return ;
}

void MainWindow::disableRobot(quint8 id){
    if(this->playerBoxes.at(id)->isEnabled()) this->playerBoxes.at(id)->setDisabled(true);
    else return ;
}

void MainWindow::updateGUI(MRCTeam *ourTeam, MRCTeam *theirTeam, Locations *loc){
    ui->openGLWidget->updateDetection(ourTeam, theirTeam);
    ui->openGLWidget->updateFieldGeometry(loc);

    ui->openGLWidget->setDrawBallVel(enableBallVelocityVector());
    ui->openGLWidget->setDrawAllieVel(enableAllieVelocityVector());
    ui->openGLWidget->setDrawEnemyVel(enableEnemyVelocityVector());
    ui->openGLWidget->setDrawPlayerConfidency(enablePlayerConfidency());

}

void MainWindow::setAgressivity(QString agressivity){
    QPixmap pixmp;

    if(agressivity.toLower() == ui->agressivity_txt->text().toLower()) return;

    if(agressivity == "high_attack"){
        ui->agressivity_txt->setText("High_Attack");
        pixmp.load(":/textures/textures/ui/ag.png");
    }
    else if(agressivity == "medium_attack"){
        ui->agressivity_txt->setText("Medium_Attack");
        pixmp.load(":/textures/textures/ui/ag.png");
    }
    else if(agressivity == "equilibrated"){
        ui->agressivity_txt->setText("Equilibrated");
        pixmp.load(":/textures/textures/ui/eq.png");
    }
    else if(agressivity == "medium_defense"){
        ui->agressivity_txt->setText("Medium_Defense");
        pixmp.load(":/textures/textures/ui/def.png");
    }
    else if(agressivity == "high_defense"){
        ui->agressivity_txt->setText("High_Defense");
        pixmp.load(":/textures/textures/ui/def.png");
    }

    ui->agressivity_img->setPixmap(pixmp);
}

void MainWindow::setPlayerBattery(quint8 id, int qt){
    this->playerBatteries.at(id)->setValue(qt);
}

void MainWindow::setPlayerKickCharge(quint8 id, int qt){
    this->playerKickCharges.at(id)->setValue(qt);
}

void MainWindow::setRadioConnect(quint8 id, bool isOnline){
    if(!isOnline){
        playerConnections.at(id)->setText("Off");
    }else{
        playerConnections.at(id)->setText("On");
    }
}

void MainWindow::setPlayerRole(quint8 id, QString role){
    QPixmap pixmp;

    if(role.toLower() == playerRoles.at(id).second->text().toLower()) return;

    if(role.toLower() == "role_default"){ // teste
        pixmp.load(":/textures/textures/ui/gk.png");
        playerRoles.at(id).first->setPixmap(pixmp);
        playerRoles.at(id).second->setText(role);
    }else if(role.toLower() == "role_barrier"){
        pixmp.load(":/textures/textures/ui/bar.png");
        playerRoles.at(id).first->setPixmap(pixmp);
        playerRoles.at(id).second->setText(role);
    }else if(role.toLower() == "role_attacker"){
        pixmp.load(":/textures/textures/ui/atk.png");
        playerRoles.at(id).first->setPixmap(pixmp);
        playerRoles.at(id).second->setText(role);
    }else if(role.toLower() == "role_support"){
        pixmp.load(":/textures/textures/ui/sup.png");
        playerRoles.at(id).first->setPixmap(pixmp);
        playerRoles.at(id).second->setText(role);
    }else{
        pixmp.load(":/textures/textures/ui/none.png");
        playerRoles.at(id).first->setPixmap(pixmp);
        playerRoles.at(id).second->setText("Undefined");
    }
}

void MainWindow::setDribble(quint8 id, bool isActive){
    if(!isActive){
        playerDribbles.at(id)->setText("Off");
    }else{
        playerDribbles.at(id)->setText("On");
    }
}

void MainWindow::setupTeams(MRCTeam *our, MRCTeam *their, QString opTeam){
    _ourTeam = our;
    _theirTeam = their;

    if(_ourTeam->teamColor() == Colors::Color::YELLOW){
        ui->team_y->setPixmap(QPixmap(":/textures/textures/ui/armorial.ico"));
        ui->team_b->setPixmap(QPixmap(":/textures/textures/ui/defaultteam.png"));
    }else{
        ui->team_b->setPixmap(QPixmap(":/textures/textures/ui/armorial.ico"));
        ui->team_y->setPixmap(QPixmap(":/textures/textures/ui/defaultteam.png"));
    }

    std::vector<QPixmap> pixmapVector;

    for(int x = 0; x < MRCConstants::_qtPlayers; x++){
        char str[50];
        if(_ourTeam->teamColor() == Colors::Color::YELLOW) sprintf(str, ":/textures/textures/robots/yellow/y%d.png", x);
        else sprintf(str, ":/textures/textures/robots/blue/b%d.png", x);

        pixmapVector.push_back(QPixmap(str));
    }

    ui->sprite_1->setPixmap(pixmapVector.at(0));
    ui->sprite_2->setPixmap(pixmapVector.at(1));
    ui->sprite_3->setPixmap(pixmapVector.at(2));
    ui->sprite_4->setPixmap(pixmapVector.at(3));
    ui->sprite_5->setPixmap(pixmapVector.at(4));
    ui->sprite_6->setPixmap(pixmapVector.at(5));
    ui->sprite_7->setPixmap(pixmapVector.at(6));
    ui->sprite_8->setPixmap(pixmapVector.at(7));
    ui->sprite_9->setPixmap(pixmapVector.at(8));
    ui->sprite_10->setPixmap(pixmapVector.at(9));
    ui->sprite_11->setPixmap(pixmapVector.at(10));
    ui->sprite_12->setPixmap(pixmapVector.at(11));

    // Initial setups
    resetRobots();
    updateScores(0, 0, 0, 0, 0, 0, 0, 0);

    updateGameStage("FIRST HALF");
    updateRefereeCommand("GAME_HALT");
    updateTimeLeft("0.0s");

    // test
    setAgressivity("equilibrated");
}

QString MainWindow::transformIntToString(int value){
    char assistant[10];
    sprintf(assistant, "%d", value);

    QString newStr(assistant);

    return newStr;
}

void MainWindow::updateScores(int score_yellow, int yellowcards_yellow, int redcards_yellow, int tout_yellow,
                              int score_blue, int yellowcards_blue, int redcards_blue, int tout_blue){

    // scores for yellow
    ui->score_y->setText(transformIntToString(score_yellow));
    ui->yelc_y->setText(transformIntToString(yellowcards_yellow));
    ui->redc_y->setText(transformIntToString(redcards_yellow));
    ui->tout_y->setText(transformIntToString(tout_yellow));

    // scores for blue
    ui->score_b->setText(transformIntToString(score_blue));
    ui->yelc_b->setText(transformIntToString(yellowcards_blue));
    ui->redc_b->setText(transformIntToString(redcards_blue));
    ui->tout_b->setText(transformIntToString(tout_blue));
}

void MainWindow::updateGameStage(QString stage){
    ui->label_stage->setText(stage);
}

void MainWindow::updateRefereeCommand(QString command){
    ui->label_command->setText(command);
}

void MainWindow::updateTimeLeft(QString timeleft){
    ui->label_time->setText(timeleft);
}

bool MainWindow::enableAllieVelocityVector(){
    return (ui->allievel->checkState() == Qt::CheckState::Checked) ? true : false;
}

bool MainWindow::enableEnemyVelocityVector(){
    return (ui->enemyvel->checkState() == Qt::CheckState::Checked) ? true : false;
}

bool MainWindow::enableBallVelocityVector(){
    return (ui->ballvel->checkState() == Qt::CheckState::Checked) ? true : false;
}

bool MainWindow::enablePlayerConfidency(){
    return (ui->playerconf->checkState() == Qt::CheckState::Checked) ? true : false;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // creating vector for groupboxes
    playerBoxes.push_back(ui->groupBox);
    playerBoxes.push_back(ui->groupBox_2);
    playerBoxes.push_back(ui->groupBox_3);
    playerBoxes.push_back(ui->groupBox_4);
    playerBoxes.push_back(ui->groupBox_5);
    playerBoxes.push_back(ui->groupBox_6);
    playerBoxes.push_back(ui->groupBox_7);
    playerBoxes.push_back(ui->groupBox_8);
    playerBoxes.push_back(ui->groupBox_9);
    playerBoxes.push_back(ui->groupBox_10);
    playerBoxes.push_back(ui->groupBox_11);
    playerBoxes.push_back(ui->groupBox_12);

    // creating vector for battery
    playerBatteries.push_back(ui->battery_1);
    playerBatteries.push_back(ui->battery_2);
    playerBatteries.push_back(ui->battery_3);
    playerBatteries.push_back(ui->battery_4);
    playerBatteries.push_back(ui->battery_5);
    playerBatteries.push_back(ui->battery_6);
    playerBatteries.push_back(ui->battery_7);
    playerBatteries.push_back(ui->battery_8);
    playerBatteries.push_back(ui->battery_9);
    playerBatteries.push_back(ui->battery_10);
    playerBatteries.push_back(ui->battery_11);
    playerBatteries.push_back(ui->battery_12);

    // creating vector for kick chages
    playerKickCharges.push_back(ui->chute_1);
    playerKickCharges.push_back(ui->chute_2);
    playerKickCharges.push_back(ui->chute_3);
    playerKickCharges.push_back(ui->chute_4);
    playerKickCharges.push_back(ui->chute_5);
    playerKickCharges.push_back(ui->chute_6);
    playerKickCharges.push_back(ui->chute_7);
    playerKickCharges.push_back(ui->chute_8);
    playerKickCharges.push_back(ui->chute_9);
    playerKickCharges.push_back(ui->chute_10);
    playerKickCharges.push_back(ui->chute_11);
    playerKickCharges.push_back(ui->chute_12);

    // creating vector for roles (label img and label text)
    playerRoles.push_back(std::make_pair(ui->imgrole_1, ui->role_1));
    playerRoles.push_back(std::make_pair(ui->imgrole_2, ui->role_2));
    playerRoles.push_back(std::make_pair(ui->imgrole_3, ui->role_3));
    playerRoles.push_back(std::make_pair(ui->imgrole_4, ui->role_4));
    playerRoles.push_back(std::make_pair(ui->imgrole_5, ui->role_5));
    playerRoles.push_back(std::make_pair(ui->imgrole_6, ui->role_6));
    playerRoles.push_back(std::make_pair(ui->imgrole_7, ui->role_7));
    playerRoles.push_back(std::make_pair(ui->imgrole_8, ui->role_8));
    playerRoles.push_back(std::make_pair(ui->imgrole_9, ui->role_9));
    playerRoles.push_back(std::make_pair(ui->imgrole_10, ui->role_10));
    playerRoles.push_back(std::make_pair(ui->imgrole_11, ui->role_11));
    playerRoles.push_back(std::make_pair(ui->imgrole_12, ui->role_12));

    // creating vector for radio connection
    playerConnections.push_back(ui->status_1);
    playerConnections.push_back(ui->status_2);
    playerConnections.push_back(ui->status_3);
    playerConnections.push_back(ui->status_4);
    playerConnections.push_back(ui->status_5);
    playerConnections.push_back(ui->status_6);
    playerConnections.push_back(ui->status_7);
    playerConnections.push_back(ui->status_8);
    playerConnections.push_back(ui->status_9);
    playerConnections.push_back(ui->status_10);
    playerConnections.push_back(ui->status_11);
    playerConnections.push_back(ui->status_12);

    // creating vector for dribble connection
    playerDribbles.push_back(ui->dribble_1);
    playerDribbles.push_back(ui->dribble_2);
    playerDribbles.push_back(ui->dribble_3);
    playerDribbles.push_back(ui->dribble_4);
    playerDribbles.push_back(ui->dribble_5);
    playerDribbles.push_back(ui->dribble_6);
    playerDribbles.push_back(ui->dribble_7);
    playerDribbles.push_back(ui->dribble_8);
    playerDribbles.push_back(ui->dribble_9);
    playerDribbles.push_back(ui->dribble_10);
    playerDribbles.push_back(ui->dribble_11);
    playerDribbles.push_back(ui->dribble_12);

    // dark pallete
    this->setStyle(QStyleFactory::create("Fusion"));
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

MainWindow::~MainWindow()
{
    delete ui;
}

