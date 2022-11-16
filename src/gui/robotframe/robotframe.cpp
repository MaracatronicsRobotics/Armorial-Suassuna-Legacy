#include "robotframe.h"

RobotFrame::RobotFrame(QWidget *parent) : QFrame(parent) {
    // Setting default data
    setFixedSize(230, 80);

    // Set pointers to nullptr
    _robotPattern = nullptr;
    _robotId = nullptr;
    _radioStatus = nullptr;
    _robotBatteryIcon = nullptr;
    _robotBattery = nullptr;
    _roleIcon = nullptr;
    _roleName = nullptr;

    // Reset
    reset();
}

void RobotFrame::reset() {
    setRobotData(Common::Enums::BLUE, 0, DEFAULT_PATTERN_SIZE);
    setRobotRadioStatus(false);
    setRobotBattery(0, width() - (DEFAULT_PATTERN_SIZE + 2 * DEFAULT_MARGIN_BORDER) - DEFAULT_ICON_SIZE - (DEFAULT_MARGIN_BORDER * 2), DEFAULT_ICON_SIZE, DEFAULT_FONT_SIZE);
    setRobotRole("No Role");
}

void RobotFrame::setRobotData(Common::Enums::Color teamColor, quint8 robotId, int patternSize) {
    // Create robot pattern label if it hasn't been created yet
    if(_robotPattern == nullptr) {
        _robotPattern = new QLabel(this);
    }

    // Load pattern image from path
    QString path = QString(":/resources/robots/%1/%2.png")
            .arg((teamColor == Common::Enums::YELLOW ? "yellow" : "blue"))
            .arg((teamColor == Common::Enums::YELLOW ? QString("y%1").arg(robotId) : QString("b%1").arg(robotId)));
    QPixmap pixmap(path);

    // Check if loaded pattern is null
    if(pixmap.isNull()) {
        spdlog::error("{}", fmt::format(fmt::emphasis::bold, "[setRobotData] Failed to load QPixMap at RobotFrame"));
        return ;
    }

    // Set pattern geometry
    _robotPattern->setGeometry(DEFAULT_MARGIN_BORDER, (height() - patternSize) / 2 + DEFAULT_MARGIN_BORDER, patternSize, patternSize);
    _robotPattern->setPixmap(pixmap);
    _robotPattern->setScaledContents(true);

    // Create robot id if it hasn't been created yet
    if(_robotId == nullptr) {
        _robotId = new QLabel(this);
    }

    // Set text, font and alignment
    _robotId->setText(QString("Robot %1").arg(robotId));
    _robotId->setFont(QFont("Ubuntu", 11));
    _robotId->setAlignment(Qt::AlignLeft | Qt::AlignHCenter);

    // Set robot id label geometry
    _robotId->setGeometry(DEFAULT_MARGIN_BORDER, DEFAULT_MARGIN_BORDER, width() - DEFAULT_MARGIN_BORDER, DEFAULT_ICON_SIZE);
}

void RobotFrame::setRobotRadioStatus(bool status) {
    // Create infrared status if it hasn't been created yet
    if(_radioStatus == nullptr) {
        _radioStatus = new QLabel(this);
    }

    // Load status image from path
    QString path = QString(":/resources/gui/robotframe/connected%1.png").arg(status ? "ok" : "not");
    QPixmap pixmap(path);

    // Check if loaded pattern is null
    if(pixmap.isNull()) {
        spdlog::error("{}", fmt::format(fmt::emphasis::bold, "[setRobotRadioStatus] Failed to load QPixMap at RobotFrame"));
        return ;
    }

    // Set pattern geometry
    _radioStatus->setGeometry(width() - DEFAULT_ICON_SIZE - DEFAULT_MARGIN_BORDER, DEFAULT_MARGIN_BORDER, DEFAULT_ICON_SIZE, DEFAULT_ICON_SIZE);
    _radioStatus->setPixmap(pixmap);
    _radioStatus->setScaledContents(true);
}

void RobotFrame::setRobotBattery(int value, int batteryWidth, int batteryHeight, int fontSize) {
    // Create battery progressbar if it hasn't been created yet
    if(_robotBattery == nullptr) {
        _robotBattery = new QProgressBar(this);
        _robotBattery->setGeometry(DEFAULT_PATTERN_SIZE + DEFAULT_ICON_SIZE + 2 * DEFAULT_MARGIN_BORDER + DEFAULT_MARGIN_BORDER, (height() - DEFAULT_PATTERN_SIZE) / 2 + (DEFAULT_ICON_SIZE*2) + DEFAULT_MARGIN_BORDER/2 + DEFAULT_MARGIN_BORDER, batteryWidth, batteryHeight);
        _robotBattery->setFont(QFont("Ubuntu", fontSize));
    }

    // Create battery icon if it hasn't been created yet
    if(_robotBatteryIcon == nullptr) {
        _robotBatteryIcon = new QLabel(this);
        _robotBatteryIcon->setGeometry(DEFAULT_PATTERN_SIZE + 2 * DEFAULT_MARGIN_BORDER, (height() - DEFAULT_PATTERN_SIZE) / 2 + (DEFAULT_ICON_SIZE*2) + DEFAULT_MARGIN_BORDER/2 + DEFAULT_MARGIN_BORDER, DEFAULT_ICON_SIZE, DEFAULT_ICON_SIZE);
        _robotBatteryIcon->setPixmap(QPixmap(":/resources/gui/robotframe/battery.png"));
        _robotBatteryIcon->setScaledContents(true);
    }

    // Set value
    _robotBattery->setValue(value);
}

void RobotFrame::setRobotRole(QString roleName) {
    // Create role icon label if it hasn't been created yet
    if(_roleIcon == nullptr) {
        _roleIcon = new QLabel(this);
    }

    // Load icon image from path
    QString path = QString(":/resources/gui/roles/%1.png").arg(roleName.toLower());
    QPixmap pixmap(path);

    // Check if loaded pattern is null
    if(pixmap.isNull()) {
        path = QString(":/resources/gui/none.png");
        pixmap = QPixmap(path);
    }

    // Set pattern geometry
    _roleIcon->setGeometry(DEFAULT_PATTERN_SIZE + 2 * DEFAULT_MARGIN_BORDER, (height() - DEFAULT_PATTERN_SIZE) / 2 + DEFAULT_ICON_SIZE + DEFAULT_MARGIN_BORDER, DEFAULT_ICON_SIZE, DEFAULT_ICON_SIZE);
    _roleIcon->setPixmap(pixmap);
    _roleIcon->setScaledContents(true);

    // Create role name label if it hasn't been created yet
    if(_roleName == nullptr) {
        _roleName = new QLabel(this);
    }

    // Set text, font and alignment
    _roleName->setText(roleName);
    _roleName->setFont(QFont("Ubuntu", DEFAULT_FONT_SIZE));
    _roleName->setAlignment(Qt::AlignLeft);

    // Set robot id label geometry
    _roleName->setGeometry(DEFAULT_PATTERN_SIZE + DEFAULT_ICON_SIZE + 3 * DEFAULT_MARGIN_BORDER, (height() - DEFAULT_PATTERN_SIZE) / 2 + DEFAULT_ICON_SIZE + DEFAULT_MARGIN_BORDER, width()/2, DEFAULT_ICON_SIZE);
}
