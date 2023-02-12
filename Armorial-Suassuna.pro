TEMPLATE = app
DESTDIR  = ../bin
TARGET   = Armorial-Suassuna
VERSION  = 1.0.0

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

CONFIG += c++17 console
CONFIG -= app_bundle
QT += core network gui widgets opengl

DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lQt5Core -lprotobuf -lGLU -lfmt -lArmorial

system(echo "Generating internal message proto headers" && cd include/proto/services && protoc --cpp_out=../ *.proto && cd ../../..)
system(echo "Generating simulation proto headers" && cd include/proto/simulation && protoc --cpp_out=../ *.proto && cd ../../..)
system(echo "Generating ssl-vision proto headers" && cd include/proto/sslvision && protoc --cpp_out=../ *.proto && cd ../../..)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += PROJECT_PATH=\\\"$${PWD}\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    include/proto/command.pb.cc \
    include/proto/common.pb.cc \
    include/proto/messages.pb.cc \
    include/proto/packet.pb.cc \
    include/proto/replacement.pb.cc \
    main.cpp \
    src/common/constants/constants.cpp \
    src/common/types/robot/robot.cpp \
    src/common/utils/utils.cpp \
    src/entities/basestation/basestation.cpp \
    src/entities/basestation/sim/simstation.cpp \
    src/entities/coach/coach.cpp \
    src/entities/coach/team/team.cpp \
    src/entities/player/behavior/default/behavior_default.cpp \
    src/entities/player/behavior/intercept/behavior_intercept.cpp \
    src/entities/player/behavior/move/behavior_move.cpp \
    src/entities/player/role/default/role_default.cpp \
    src/entities/player/role/goalkeeper/role_goalkeeper.cpp \
    src/entities/player/role/role.cpp \
    src/entities/player/behavior/behavior.cpp \
    src/entities/player/player.cpp \
    src/entities/player/skill/goto/skill_goto.cpp \
    src/entities/player/skill/rotateto/skill_rotateto.cpp \
    src/entities/player/skill/spin/skill_spin.cpp \
    src/entities/player/skill/skill.cpp \
    src/entities/referee/referee.cpp \
    src/entities/vision/filters/kalman/kalmanfilter.cpp \
    src/entities/vision/filters/loss/lossfilter.cpp \
    src/entities/vision/filters/noise/noisefilter.cpp \
    src/entities/vision/object/visionobject.cpp \
    src/entities/vision/vision.cpp \
    src/entities/worldmap/worldmap.cpp \
    src/gui/fieldview/fieldview.cpp \
    src/gui/gui.cpp \
    src/suassuna.cpp

HEADERS += \
    include/proto/command.pb.h \
    include/proto/common.pb.h \
    include/proto/messages.pb.h \
    include/proto/packet.pb.h \
    include/proto/replacement.pb.h \
    src/common/constants/constants.h \
    src/common/types/robot/robot.h \
    src/common/utils/utils.h \
    src/entities/basestation/basestation.h \
    src/entities/basestation/sim/simstation.h \
    src/entities/basesuassuna.h \
    src/entities/coach/coach.h \
    src/entities/coach/team/team.h \
    src/entities/player/behavior/default/behavior_default.h \
    src/entities/player/behavior/intercept/behavior_intercept.h \
    src/entities/player/behavior/move/behavior_move.h \
    src/entities/player/role/default/role_default.h \
    src/entities/player/role/goalkeeper/role_goalkeeper.h \
    src/entities/player/role/role.h \
    src/entities/player/behavior/behavior.h \
    src/entities/player/player.h \
    src/entities/player/skill/goto/skill_goto.h \
    src/entities/player/skill/rotateto/skill_rotateto.h \
    src/entities/player/skill/spin/skill_spin.h \
    src/entities/player/skill/skill.h \
    src/entities/referee/referee.h \
    src/entities/vision/filters/filters.h \
    src/entities/vision/filters/kalman/kalmanfilter.h \
    src/entities/vision/filters/loss/lossfilter.h \
    src/entities/vision/filters/noise/noisefilter.h \
    src/entities/vision/object/visionobject.h \
    src/entities/vision/vision.h \
    src/entities/worldmap/worldmap.h \
    src/gui/fieldview/fieldview.h \
    src/gui/gui.h \
    src/suassuna.h

FORMS += \
    src/gui/gui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
