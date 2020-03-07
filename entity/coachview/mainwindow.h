#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <entity/entity.h>
#include <entity/contromodule/mrcteam.h>
#include <QMainWindow>
#include <QGroupBox>
#include <QProgressBar>
#include <QLabel>
#include <QMutex>
#include <entity/coachview/samico.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // general
    void resetRobots();
    void disableRobot(quint8 id);
    void enableRobot(quint8 id);

    // utils
    QString transformIntToString(int value);
    void setupTeams(MRCTeam *our, MRCTeam *their, QString opTeam = "");

    // setters
    void setPlayerRole(quint8 id, QString role);
    void setPlayerBattery(quint8 id, int qt);
    void setRadioConnect(quint8 id, bool isOnline);
    void setDribble(quint8 id, bool isActive);

    // updaters
    void updateScores(int score_yellow, int yellowcards_yellow, int redcards_yellow, int tout_yellow,
                      int score_blue, int yellowcards_blue, int redcards_blue, int tout_blue);
    void updateGameStage(QString stage);
    void updateRefereeCommand(QString command);
    void updateTimeLeft(QString timeleft);

private:
    Ui::MainWindow *ui;
    std::vector<QGroupBox*> playerBoxes;
    std::vector<QProgressBar*> playerBatteries;
    std::vector<std::pair<QLabel*, QLabel*>> playerRoles;
    std::vector<QLabel*> playerConnections;
    std::vector<QLabel*> playerDribbles;

    MRCTeam* _ourTeam;
    MRCTeam* _theirTeam;

};
#endif // MAINWINDOW_H
