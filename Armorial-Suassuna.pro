QT -= gui

CONFIG += c++14 console
CONFIG -= app_bundle
QT += core \
	gui \
	widgets \
	network \
	opengl

DEFINES += QT_DEPRECATED_WARNINGS
LIBS *= -lprotobuf -lGLU -pthread -lsfml-graphics -lsfml-window -lsfml-system -lGEARSystem -lomniORB4 -lomnithread -lQt5Core -lpthread


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	entity/entity.cpp \
    entity/player/skills/basics/skill_goto.cpp \
    entity/player/skills/basics/skill_gotolookto.cpp \
    entity/player/skills/basics/skill_rotateto.cpp \
	entity/world/world.cpp \
	entity/world/worldmapupdater.cpp \
	exithandler.cpp \
	include/3rd_party/messages_robocup_ssl_detection.pb.cc \
	include/3rd_party/netraw.cpp \
	include/3rd_party/robocup_ssl_client.cpp \
	include/3rd_party/messages_robocup_ssl_geometry.pb.cc \
	include/3rd_party/messages_robocup_ssl_refbox_log.pb.cc \
	include/3rd_party/messages_robocup_ssl_wrapper.pb.cc \
	include/3rd_party/game_event.pb.cc \
	include/3rd_party/grSim_Commands.pb.cc \
	include/3rd_party/grSim_Packet.pb.cc \
	include/3rd_party/grSim_Replacement.pb.cc \
	include/3rd_party/referee.pb.cc \
	entity/referee/SSLReferee/sslgameinfo.cpp \
	entity/referee/referee.cpp \
	entity/referee/SSLReferee/sslreferee.cpp \
	instancechecker.cpp \
	main.cpp \
	suassuna.cpp \
	utils/basics/color.cc \
	utils/basics/competition.cc \
	utils/basics/quadrant.cc \
	utils/basics/side.cc \
	utils/basics/wall.cc \
	utils/fields/field_ssl2014.cc \
	utils/fields/field_ssl2015.cc \
	utils/fields/field_vss2008.cc \
	utils/fields/fields.cc \
	utils/fieldside/fieldside.cc \
	utils/graph/edge.cc \
	utils/graph/graph.cc \
	utils/graph/vertex.cc \
	utils/line/line.cc \
	utils/utils.cc \
	utils/wrtimer/wrtimer.cc \
    entity/player/player.cpp \
	entity/contromodule/mrcteam.cpp \
    entity/player/skills/skill.cpp \
    entity/locations.cpp \
    entity/contromodule/grsSimulator/grsSimulator.cpp \
    entity/contromodule/coach.cpp \
    entity/player/playerbus.cpp \
    entity/player/playeraccess.cpp \
    entity/contromodule/strategy/strategy.cpp \
    entity/contromodule/playersdistribution.cpp \
    entity/contromodule/strategy/strategystate.cpp \
    entity/contromodule/coachutils.cpp \
    entity/contromodule/playbook/playbook.cpp \
    entity/player/behaviour/behaviour.cpp \
    entity/player/behaviour/basics/behaviour_donothing.cpp \
    entity/player/skills/basics/skill_donothing.cpp \
    utils/knn/knn.cc \
    entity/contromodule/strategy/basics/mrcstrategy.cpp \
    entity/contromodule/strategy/basics/sslstrategy.cpp \
    entity/contromodule/controlmodule.cpp \
    entity/contromodule/strategy/basics/sslstrategy_halt.cpp \
    entity/contromodule/playbook/basics/playbook_donothing.cpp \
    entity/player/behaviour/basics/behaviour_followball.cpp \
    entity/contromodule/playbook/basics/playbook_followball.cpp \
    entity/contromodule/playbook/basics/playbook_timeout.cpp \
    entity/player/behaviour/basics/behaviour_timeout.cpp \
    entity/contromodule/strategy/basics/sslstrategy_timeout.cpp \
    entity/player/control/pid.cpp \
    utils/mrctimer/mrctimer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	entity/baseentity.h \
	entity/entity.h \
    entity/player/skills/basics/skill_goto.h \
    entity/player/skills/basics/skill_gotolookto.h \
    entity/player/skills/basics/skill_rotateto.h \
	entity/world/world.h \
	entity/world/worldmapupdater.h \
	exithandler.h \
	include/3rd_party/netraw.h \
	include/3rd_party/robocup_ssl_client.h \
	include/3rd_party/util.h \
	include/filters.h \
	include/3rd_party/messages_robocup_ssl_detection.pb.h \
	include/3rd_party/messages_robocup_ssl_geometry.pb.h \
	include/3rd_party/messages_robocup_ssl_wrapper.pb.h \
	include/3rd_party/game_event.pb.h \
	include/3rd_party/messages_robocup_ssl_refbox_log.pb.h \
	include/3rd_party/referee.pb.h \
	include/3rd_party/grSim_Commands.pb.h \
	include/3rd_party/grSim_Packet.pb.h \
	include/3rd_party/grSim_Replacement.pb.h \
	entity/referee/SSLReferee/sslgameinfo.h \
	entity/referee/referee.h \
	entity/referee/SSLReferee/sslreferee.h \
	instancechecker.h \
	suassuna.h \
	utils/basics/color.hh \
	utils/basics/competition.hh \
	utils/basics/quadrant.hh \
	utils/basics/side.hh \
	utils/basics/wall.hh \
	utils/fields/field_ssl2014.hh \
	utils/fields/field_ssl2015.hh \
	utils/fields/field_vss2008.hh \
	utils/fields/fields.hh \
	utils/fields/wrfields.hh \
	utils/fieldside/fieldside.hh \
	utils/graph/basegraph.hh \
	utils/graph/edge.hh \
	utils/graph/graph.hh \
	utils/graph/vertex.hh \
	utils/line/line.hh \
	utils/utils.hh \
	utils/wrtimer/wrtimer.hh \
    entity/player/player.h \
	entity/contromodule/mrcteam.h \
    entity/player/skills/skill.h \
    entity/locations.h \
    entity/player/skills/skills_include.h \
    entity/contromodule/grsSimulator/grsSimulator.h \
    entity/contromodule/coach.h \
    entity/player/playerbus.h \
    entity/player/playeraccess.h \
    entity/contromodule/strategy/strategy.h \
    entity/contromodule/basecoach.h \
    entity/contromodule/playersdistribution.h \
    entity/contromodule/strategy/strategystate.h \
    entity/contromodule/coachutils.h \
    entity/contromodule/playbook/playbook.h \
    entity/player/behaviour/behaviour.h \
    entity/player/behaviour/mrcbehaviours.h \
    entity/player/behaviour/basics/behaviour_donothing.h \
    entity/player/skills/basics/skill_donothing.h \
    utils/knn/knn.hh \
    entity/contromodule/strategy/basics/mrcstrategy.h \
    entity/contromodule/strategy/basics/sslstrategy.h \
    entity/contromodule/controlmodule.h \
    entity/contromodule/strategy/basics/sslstrategy_halt.h \
    entity/contromodule/playbook/basics/playbook_donothing.h \
    entity/contromodule/playbook/mrcplaybook.h \
    entity/player/behaviour/basics/behaviour_followball.h \
    entity/contromodule/playbook/basics/playbook_followball.h \
    entity/contromodule/playbook/basics/playbook_timeout.h \
    entity/player/behaviour/basics/behaviour_timeout.h \
    entity/contromodule/strategy/basics/sslstrategy_timeout.h \
    entity/contromodule/strategy/mrcstrategies.h \
    entity/player/control/pid.h \
    utils/mrctimer/mrctimer.h
