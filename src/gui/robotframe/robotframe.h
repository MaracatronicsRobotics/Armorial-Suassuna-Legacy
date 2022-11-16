#ifndef ROBOTFRAME_H
#define ROBOTFRAME_H

#include <QFrame>
#include <QLabel>
#include <QProgressBar>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/color.h>

#include <Armorial/Common/Enums/Color/Color.h>

#define DEFAULT_MARGIN_BORDER 5
#define DEFAULT_ICON_SIZE 16
#define DEFAULT_PATTERN_SIZE 64
#define DEFAULT_FONT_SIZE 9

class RobotFrame : public QFrame
{
public:
    RobotFrame(QWidget *parent = nullptr);
    void reset();
    void setRobotData(Common::Enums::Color teamColor, quint8 robotId, int patternSize = DEFAULT_PATTERN_SIZE);
    void setRobotRadioStatus(bool status);
    void setRobotBattery(int value, int batteryWidth, int batteryHeight = DEFAULT_ICON_SIZE, int fontSize = DEFAULT_FONT_SIZE);
    void setRobotRole(QString roleName);

private:
    // Robot id / pattern
    QLabel *_robotPattern;
    QLabel *_robotId;

    // Robot radio status
    QLabel *_radioStatus;

    // Robot battery
    QLabel *_robotBatteryIcon;
    QProgressBar *_robotBattery;

    // Role icon / name
    QLabel *_roleIcon;
    QLabel *_roleName;
};

#endif // ROBOTFRAME_H
