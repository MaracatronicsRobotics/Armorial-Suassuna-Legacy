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
QT += core \
        gui \
        widgets \
        network \
        opengl

DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lQt5Core -lprotobuf -lgrpc++ -lGLU

system(echo "compiling protobuf" && cd proto/services && protoc --grpc_out=../ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto && cd ../../..)
system(echo "compiling protobuf" && cd proto/services && protoc --cpp_out=../ *.proto && cd ../../..)
system(echo "generating grsim .proto" && cd proto/grsim && protoc --cpp_out=../ *.proto && cd ../../..)
system(echo "generating refereee .proto" && cd proto/referee && protoc --cpp_out=../ *.proto && cd ../../..)

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
    main.cpp \
    proto/actuatorservice.grpc.pb.cc \
    proto/actuatorservice.pb.cc \
    proto/coachservice.grpc.pb.cc \
    proto/coachservice.pb.cc \
    proto/grSim_Commands.pb.cc \
    proto/grSim_Packet.pb.cc \
    proto/grSim_Replacement.pb.cc \
    proto/messages.grpc.pb.cc \
    proto/messages.pb.cc \
    proto/messages_robocup_ssl_robot_status.pb.cc \
    proto/sensorservice.grpc.pb.cc \
    proto/sensorservice.pb.cc \
    proto/ssl_gc_api.pb.cc \
    proto/ssl_gc_change.pb.cc \
    proto/ssl_gc_ci.pb.cc \
    proto/ssl_gc_common.pb.cc \
    proto/ssl_gc_engine.pb.cc \
    proto/ssl_gc_engine_config.pb.cc \
    proto/ssl_gc_game_event.pb.cc \
    proto/ssl_gc_geometry.pb.cc \
    proto/ssl_gc_rcon.pb.cc \
    proto/ssl_gc_rcon_autoref.pb.cc \
    proto/ssl_gc_rcon_remotecontrol.pb.cc \
    proto/ssl_gc_rcon_team.pb.cc \
    proto/ssl_gc_referee_message.pb.cc \
    proto/ssl_gc_state.pb.cc \
    proto/ssl_vision_detection.pb.cc \
    proto/ssl_vision_detection_tracked.pb.cc \
    proto/ssl_vision_geometry.pb.cc \
    proto/ssl_vision_wrapper.pb.cc \
    proto/ssl_vision_wrapper_tracked.pb.cc \
    proto/visionservice.grpc.pb.cc \
    proto/visionservice.pb.cc \
    src/constants/constants.cpp \
    src/entities/entity.cpp \
    src/entities/player/behaviour/behaviour.cpp \
    src/entities/player/behaviour/gotolookto/gotolookto.cpp \
    src/entities/referee/ballplay/ballplay.cpp \
    src/entities/referee/gameinfo/gameinfo.cpp \
    src/entities/referee/referee.cpp \
    src/entities/player/player.cpp \
    src/entities/role/role.cpp \
    src/entities/skill/goto/goto.cpp \
    src/entities/skill/kick/kick.cpp \
    src/entities/skill/rotateto/rotateto.cpp \
    src/entities/skill/skill.cpp \
    src/entities/world/world.cpp \
    src/entities/worldmap/locations/locations.cpp \
    src/entities/worldmap/worldmap.cpp \
    src/exithandler/exithandler.cpp \
    src/gui/gui.cpp \
    src/services/actuator/actuatorservice.cpp \
    src/services/coach/coachservice.cpp \
    src/suassuna.cpp \
    src/utils/text/text.cpp \
    src/utils/timer/timer.cpp \
    src/utils/utils.cpp \
    src/utils/types/fieldside/fieldside.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    proto/actuatorservice.grpc.pb.h \
    proto/actuatorservice.pb.h \
    proto/coachservice.grpc.pb.h \
    proto/coachservice.pb.h \
    proto/grSim_Commands.pb.h \
    proto/grSim_Packet.pb.h \
    proto/grSim_Replacement.pb.h \
    proto/messages.grpc.pb.h \
    proto/messages.pb.h \
    proto/messages_robocup_ssl_robot_status.pb.h \
    proto/sensorservice.pb.h \
    proto/ssl_gc_api.pb.h \
    proto/ssl_gc_change.pb.h \
    proto/ssl_gc_ci.pb.h \
    proto/ssl_gc_common.pb.h \
    proto/ssl_gc_engine.pb.h \
    proto/ssl_gc_engine_config.pb.h \
    proto/ssl_gc_game_event.pb.h \
    proto/ssl_gc_geometry.pb.h \
    proto/ssl_gc_rcon.pb.h \
    proto/ssl_gc_rcon_autoref.pb.h \
    proto/ssl_gc_rcon_remotecontrol.pb.h \
    proto/ssl_gc_rcon_team.pb.h \
    proto/ssl_gc_referee_message.pb.h \
    proto/ssl_gc_state.pb.h \
    proto/ssl_vision_detection.pb.h \
    proto/ssl_vision_detection_tracked.pb.h \
    proto/ssl_vision_geometry.pb.h \
    proto/ssl_vision_wrapper.pb.h \
    proto/ssl_vision_wrapper_tracked.pb.h \
    proto/visionservice.grpc.pb.h \
    proto/visionservice.pb.h \
    src/constants/constants.h \
    src/entities/baseCoach.h \
    src/entities/entity.h \
    src/entities/player/behaviour/behaviour.h \
    src/entities/player/behaviour/gotolookto/gotolookto.h \
    src/entities/referee/ballplay/ballplay.h \
    src/entities/referee/gameinfo/gameinfo.h \
    src/entities/referee/referee.h \
    src/entities/referee/refereeflags.h \
    src/entities/player/player.h \
    src/entities/role/role.h \
    src/entities/skill/goto/goto.h \
    src/entities/skill/kick/kick.h \
    src/entities/skill/rotateto/rotateto.h \
    src/entities/skill/skill.h \
    src/entities/world/world.h \
    src/entities/worldmap/locations/locations.h \
    src/entities/worldmap/worldmap.h \
    src/exithandler/exithandler.h \
    src/gui/gui.h \
    src/services/actuator/actuatorservice.h \
    src/services/coach/coachservice.h \
    src/suassuna.h \
    src/utils/text/text.h \
    src/utils/timer/timer.h \
    src/utils/utils.h \
    src/utils/types/fieldside/fieldside.h \
    src/utils/types/fieldside/side.h

DISTFILES += \
    proto/LICENSE \
    proto/README.md

FORMS += \
    src/gui/gui.ui
