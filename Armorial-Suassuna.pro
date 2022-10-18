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
LIBS += -lQt5Core -lprotobuf -lgrpc -lgrpc++ -lGLU -lfmt -lArmorial

system(echo "Generating service GRPC headers" && cd include/proto/services && protoc --grpc_out=../ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto && cd ../../..)
system(echo "Generating service proto headers" && cd include/proto/services && protoc --cpp_out=../ *.proto && cd ../../..)
system(echo "Generating referee proto headers" && cd include/proto/referee && protoc --cpp_out=../ *.proto && cd ../../..)

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
        include/proto/actuatorservice.grpc.pb.cc \
        include/proto/actuatorservice.pb.cc \
        include/proto/messages.grpc.pb.cc \
        include/proto/messages.pb.cc \
        include/proto/sensorservice.grpc.pb.cc \
        include/proto/sensorservice.pb.cc \
        include/proto/ssl_gc_api.pb.cc \
        include/proto/ssl_gc_change.pb.cc \
        include/proto/ssl_gc_ci.pb.cc \
        include/proto/ssl_gc_common.pb.cc \
        include/proto/ssl_gc_engine.pb.cc \
        include/proto/ssl_gc_engine_config.pb.cc \
        include/proto/ssl_gc_game_event.pb.cc \
        include/proto/ssl_gc_geometry.pb.cc \
        include/proto/ssl_gc_rcon.pb.cc \
        include/proto/ssl_gc_rcon_autoref.pb.cc \
        include/proto/ssl_gc_rcon_remotecontrol.pb.cc \
        include/proto/ssl_gc_rcon_team.pb.cc \
        include/proto/ssl_gc_referee_message.pb.cc \
        include/proto/ssl_gc_state.pb.cc \
        include/proto/ssl_vision_detection.pb.cc \
        include/proto/ssl_vision_detection_tracked.pb.cc \
        include/proto/ssl_vision_geometry.pb.cc \
        include/proto/ssl_vision_wrapper.pb.cc \
        include/proto/ssl_vision_wrapper_tracked.pb.cc \
        include/proto/visionservice.grpc.pb.cc \
        include/proto/visionservice.pb.cc \
        main.cpp \
        src/constants/constants.cpp \
        src/entities/coach/coach.cpp \
        src/entities/coach/sslteam/sslteam.cpp \
        src/entities/controller/controller.cpp \
        src/entities/player/PID/anglepid.cpp \
        src/entities/player/PID/pid.cpp \
        src/entities/player/behavior/behavior.cpp \
        src/entities/player/behavior/catch/behavior_catch.cpp \
        src/entities/player/behavior/chaseeeeeeeeer/behavior_chaser.cpp \
        src/entities/player/behavior/default/behavior_default.cpp \
        src/entities/player/behavior/moveto/behavior_moveto.cpp \
        src/entities/player/player.cpp \
        src/entities/player/role/attacker/role_attacker.cpp \
        src/entities/player/role/default/role_default.cpp \
        src/entities/player/role/goalkeeper/role_goalkeeper.cpp \
        src/entities/player/role/goalkeeper/role_gk.cpp \
        src/entities/player/role/role.cpp \
        src/entities/player/skill/charge/skill_charge.cpp \
        src/entities/player/skill/goto/skill_goto.cpp \
        src/entities/player/skill/rotate/skill_rotateto.cpp \
        src/entities/player/skill/skill.cpp \
        src/entities/player/skill/skill_move.cpp \
        src/entities/referee/gameinfo/sslgameinfo.cpp \
        src/entities/referee/sslreferee.cpp \
        src/entities/worldmap/worldmap.cpp \
        src/gui/gui.cpp \
        src/suassuna.cpp \
        src/utils/utils.cpp \
        src/entities/player/behavior/intercept/behavior_intercept.cpp \
        src/entities/player/skill/spin/skill_spin.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/proto/actuatorservice.grpc.pb.h \
    include/proto/actuatorservice.pb.h \
    include/proto/messages.grpc.pb.h \
    include/proto/messages.pb.h \
    include/proto/sensorservice.grpc.pb.h \
    include/proto/sensorservice.pb.h \
    include/proto/ssl_gc_api.pb.h \
    include/proto/ssl_gc_change.pb.h \
    include/proto/ssl_gc_ci.pb.h \
    include/proto/ssl_gc_common.pb.h \
    include/proto/ssl_gc_engine.pb.h \
    include/proto/ssl_gc_engine_config.pb.h \
    include/proto/ssl_gc_game_event.pb.h \
    include/proto/ssl_gc_geometry.pb.h \
    include/proto/ssl_gc_rcon.pb.h \
    include/proto/ssl_gc_rcon_autoref.pb.h \
    include/proto/ssl_gc_rcon_remotecontrol.pb.h \
    include/proto/ssl_gc_rcon_team.pb.h \
    include/proto/ssl_gc_referee_message.pb.h \
    include/proto/ssl_gc_state.pb.h \
    include/proto/ssl_vision_detection.pb.h \
    include/proto/ssl_vision_detection_tracked.pb.h \
    include/proto/ssl_vision_geometry.pb.h \
    include/proto/ssl_vision_wrapper.pb.h \
    include/proto/ssl_vision_wrapper_tracked.pb.h \
    include/proto/visionservice.grpc.pb.h \
    include/proto/visionservice.pb.h \
    src/constants/constants.h \
    src/entities/coach/coach.h \
    src/entities/coach/sslteam/sslteam.h \
    src/entities/controller/controller.h \
    src/entities/player/PID/anglepid.h \
    src/entities/player/PID/pid.h \
    src/entities/player/behavior/behavior.h \
    src/entities/player/behavior/behaviors.h \
    src/entities/player/behavior/catch/behavior_catch.h \
    src/entities/player/behavior/chaseeeeeeeeer/behavior_chaser.h \
    src/entities/player/behavior/default/behavior_default.h \
    src/entities/player/behavior/moveto/behavior_moveto.h \
    src/entities/player/player.h \
    src/entities/player/role/attacker/role_attacker.h \
    src/entities/player/role/default/role_default.h \
    src/entities/player/role/goalkeeper/role_goalkeeper.h \
    src/entities/player/role/goalkeeper/role_gk.h \
    src/entities/player/role/role.h \
    src/entities/player/skill/charge/skill_charge.h \
    src/entities/player/skill/goto/skill_goto.h \
    src/entities/player/skill/rotate/skill_rotateto.h \
    src/entities/player/skill/skill.h \
    src/entities/player/skill/skill_move.h \
    src/entities/player/skill/skills.h \
    src/entities/referee/gameinfo/sslgameinfo.h \
    src/entities/referee/sslreferee.h \
    src/entities/worldmap/worldmap.h \
    src/gui/gui.h \
    src/suassuna.h \
    src/utils/utils.h \
    src/entities/player/behavior/intercept/behavior_intercept.h \
    src/entities/player/skill/spin/skill_spin.h

FORMS += \
    src/gui/gui.ui

RESOURCES +=

DISTFILES +=
