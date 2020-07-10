QT -= gui


TEMPLATE = app
DESTDIR  = ../bin
TARGET   = Armorial-Suassuna
VERSION  = 0.0.5

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

CONFIG += c++14 console
CONFIG -= app_bundle
QT += core \
        gui \
        widgets \
        network \
        opengl

DEFINES += QT_DEPRECATED_WARNINGS
LIBS *= -lprotobuf -lGLU -pthread -lGEARSystem -lomniORB4 -lomnithread -lQt5Core -lpthread


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
    const/constants.cpp \
    entity/ballprediction/ballprediction.cpp \
    entity/ballsensor/ballsensor.cpp \
    entity/coachview/soccerview/gltext.cpp \
    entity/coachview/soccerview/soccerview.cpp \
    entity/coachview/soccerview/util/field.cpp \
    entity/entity.cpp \
    entity/player/behaviour/basics/behaviour_goalkeeper.cpp \
    entity/player/behaviour/basics/behaviour_intercept.cpp \
    entity/player/behaviour/basics/behaviour_markball.cpp \
    entity/player/behaviour/basics/behaviour_penalty_cf.cpp \
    entity/player/behaviour/basics/behaviour_test.cpp \
    entity/player/navigation/fana/fana.cpp \
    entity/player/navigation/fpp/fastpathplanning.cpp \
    entity/player/navigation/navalgorithm.cpp \
    entity/player/navigation/navigation.cpp \
    entity/player/navigation/rrt/rrt.cpp \
    entity/player/navigation/rrt/rrtnode.cpp \
    entity/player/navigation/rrt/rrtobstacle.cpp \
    entity/player/navigation/rrt/rrttree.cpp \
    entity/player/navigation/rrt/rrtvertex.cpp \
    entity/player/role/basics/role_barrier.cpp \
    entity/player/role/basics/role_defender.cpp \
    entity/player/role/basics/role_defensive_midfielder.cpp \
    entity/player/role/basics/role_goalkeeper.cpp \
    entity/player/skills/basics/skill_aroundtheball.cpp \
    entity/player/skills/basics/skill_dribble.cpp \
    entity/player/skills/basics/skill_goto.cpp \
    entity/player/skills/basics/skill_gotolookto.cpp \
    entity/player/skills/basics/skill_interceptball.cpp \
    entity/player/skills/basics/skill_placement.cpp \
    entity/player/skills/basics/skill_pushball2.cpp \
    entity/player/skills/basics/skill_rotateto.cpp \
    entity/player/skills/basics/skill_test.cpp \
    entity/referee/mrcreferee.cpp \
    entity/world/world.cpp \
    entity/world/worldmapupdater.cpp \
    exithandler.cpp \
    include/3rd_party/netraw.cpp \
    entity/referee/SSLReferee/sslgameinfo.cpp \
    entity/referee/SSLReferee/sslreferee.cpp \
    include/3rd_party/ssl_gc_api.pb.cc \
    include/3rd_party/ssl_gc_change.pb.cc \
    include/3rd_party/ssl_gc_ci.pb.cc \
    include/3rd_party/ssl_gc_common.pb.cc \
    include/3rd_party/ssl_gc_engine.pb.cc \
    include/3rd_party/ssl_gc_engine_config.pb.cc \
    include/3rd_party/ssl_gc_game_event.pb.cc \
    include/3rd_party/ssl_gc_geometry.pb.cc \
    include/3rd_party/ssl_gc_rcon.pb.cc \
    include/3rd_party/ssl_gc_rcon_autoref.pb.cc \
    include/3rd_party/ssl_gc_rcon_remotecontrol.pb.cc \
    include/3rd_party/ssl_gc_rcon_team.pb.cc \
    include/3rd_party/ssl_gc_referee_message.pb.cc \
    include/3rd_party/ssl_gc_state.pb.cc \
    include/3rd_party/ssl_vision_detection.pb.cc \
    include/3rd_party/ssl_vision_detection_tracked.pb.cc \
    include/3rd_party/ssl_vision_geometry.pb.cc \
    include/3rd_party/ssl_vision_wrapper.pb.cc \
    include/3rd_party/ssl_vision_wrapper_tracked.pb.cc \
    instancechecker.cpp \
    main.cpp \
    suassuna.cpp \
    utils/basics/circle.cpp \
    utils/basics/color.cc \
    utils/basics/competition.cc \
    utils/basics/quadrant.cc \
    utils/basics/side.cc \
    utils/basics/wall.cc \
    utils/fields/field_ssl2014.cc \
    utils/fields/field_ssl2015.cc \
    utils/fields/field_ssl2020.cc \
    utils/fields/field_vss2008.cc \
    utils/fields/fields.cc \
    utils/fieldside/fieldside.cc \
    utils/filters/kalman/kalman.cpp \
    utils/filters/kalman/kalmanstate.cpp \
    utils/filters/kalman/matrix.cpp \
    utils/freeangles/freeangles.cpp \
    utils/freeangles/obstacle.cpp \
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
    entity/player/behaviour/basics/behaviour_timeout.cpp \
    entity/player/control/pid.cpp \
    utils/mrctimer/mrctimer.cpp \
    entity/player/skills/basics/skill_kick.cpp \
    entity/player/skills/basics/skill_gkick.cpp \
    entity/player/behaviour/basics/behaviour_receiver.cpp \
    entity/player/behaviour/basics/behaviour_attacker.cpp \
    entity/player/behaviour/basics/behaviour_barrier.cpp \
    entity/player/behaviour/basics/behaviour_markplayer.cpp \
    entity/player/behaviour/basics/behaviour_penalty_gk.cpp \
    entity/player/skills/basics/skill_pushball.cpp \
    entity/player/role/role.cpp \
    entity/player/role/basics/role_default.cpp \
    entity/coachview/coachview.cpp \
    entity/coachview/mainwindow.cpp \
    entity/player/skills/basics/skill_sample.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    const/constants.h \
    entity/ballprediction/ballprediction.h \
    entity/ballsensor/ballsensor.h \
    entity/baseentity.h \
    entity/coachview/soccerview/gltext.h \
    entity/coachview/soccerview/soccerview.h \
    entity/coachview/soccerview/util/field.h \
    entity/coachview/soccerview/util/field_default_constants.h \
    entity/coachview/soccerview/util/geometry.h \
    entity/coachview/soccerview/util/gvector.h \
    entity/coachview/soccerview/util/timer.h \
    entity/coachview/soccerview/util/util.h \
    entity/entity.h \
    entity/player/behaviour/basics/behaviour_intercept.h \
    entity/player/behaviour/basics/behaviour_test.h \
    entity/player/navigation/fana/fana.h \
    entity/player/navigation/fpp/fastpathplanning.h \
    entity/player/navigation/navalgorithm.h \
    entity/player/navigation/navigation.h \
    entity/player/behaviour/basics/behaviour_markball.h \
    entity/player/behaviour/basics/behaviour_penalty_cf.h \
    entity/player/navigation/rrt/rrt.h \
    entity/player/navigation/rrt/rrtnode.h \
    entity/player/navigation/rrt/rrtobstacle.h \
    entity/player/navigation/rrt/rrttree.h \
    entity/player/navigation/rrt/rrtvertex.h \
    entity/player/role/basics/role_barrier.h \
    entity/player/role/basics/role_defender.h \
    entity/player/role/basics/role_defensive_midfielder.h \
    entity/player/role/basics/role_goalkeeper.h \
    entity/player/skills/basics/skill_aroundtheball.h \
    entity/player/skills/basics/skill_dribble.h \
    entity/player/skills/basics/skill_goto.h \
    entity/player/skills/basics/skill_gotolookto.h \
    entity/player/skills/basics/skill_interceptball.h \
    entity/player/skills/basics/skill_placement.h \
    entity/player/skills/basics/skill_pushball2.h \
    entity/player/skills/basics/skill_rotateto.h \
    entity/player/skills/basics/skill_test.h \
    entity/referee/mrcreferee.h \
    entity/world/world.h \
    entity/world/worldmapupdater.h \
    exithandler.h \
    include/3rd_party/netraw.h \
    include/3rd_party/ssl_gc_api.pb.h \
    include/3rd_party/ssl_gc_change.pb.h \
    include/3rd_party/ssl_gc_ci.pb.h \
    include/3rd_party/ssl_gc_common.pb.h \
    include/3rd_party/ssl_gc_engine.pb.h \
    include/3rd_party/ssl_gc_engine_config.pb.h \
    include/3rd_party/ssl_gc_game_event.pb.h \
    include/3rd_party/ssl_gc_geometry.pb.h \
    include/3rd_party/ssl_gc_rcon.pb.h \
    include/3rd_party/ssl_gc_rcon_autoref.pb.h \
    include/3rd_party/ssl_gc_rcon_remotecontrol.pb.h \
    include/3rd_party/ssl_gc_rcon_team.pb.h \
    include/3rd_party/ssl_gc_referee_message.pb.h \
    include/3rd_party/ssl_gc_state.pb.h \
    include/3rd_party/ssl_vision_detection.pb.h \
    include/3rd_party/ssl_vision_detection_tracked.pb.h \
    include/3rd_party/ssl_vision_geometry.pb.h \
    include/3rd_party/ssl_vision_wrapper.pb.h \
    include/3rd_party/ssl_vision_wrapper_tracked.pb.h \
    include/3rd_party/util.h \
    entity/referee/SSLReferee/sslgameinfo.h \
    entity/referee/SSLReferee/sslreferee.h \
    instancechecker.h \
    suassuna.h \
    utils/basics/circle.h \
    utils/basics/color.hh \
    utils/basics/competition.hh \
    utils/basics/point2d.h \
    utils/basics/quadrant.hh \
    utils/basics/side.hh \
    utils/basics/wall.hh \
    utils/fields/field_ssl2014.hh \
    utils/fields/field_ssl2015.hh \
    utils/fields/field_ssl2020.hh \
    utils/fields/field_vss2008.hh \
    utils/fields/fields.hh \
    utils/fields/wrfields.hh \
    utils/fieldside/fieldside.hh \
    utils/filters/kalman/kalman.hpp \
    utils/filters/kalman/kalmanstate.h \
    utils/filters/kalman/matrix.h \
    utils/freeangles/freeangles.h \
    utils/freeangles/obstacle.h \
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
    entity/player/behaviour/basics/behaviour_timeout.h \
    entity/contromodule/strategy/mrcstrategies.h \
    entity/player/control/pid.h \
    utils/mrctimer/mrctimer.h \
    entity/player/skills/basics/skill_kick.h \
    entity/player/behaviour/basics/behaviour_goalkeeper.h \
    entity/player/skills/basics/skill_gkick.h \
    entity/player/behaviour/basics/behaviour_receiver.h \
    entity/player/behaviour/basics/behaviour_attacker.h \
    entity/player/behaviour/basics/behaviour_barrier.h \
    entity/player/behaviour/basics/behaviour_markplayer.h \
    entity/player/behaviour/basics/behaviour_penalty_gk.h \
    entity/player/skills/basics/skill_pushball.h \
    entity/player/role/role.h \
    entity/player/role/basics/role_default.h \
    entity/player/role/mrcroles.h \
    entity/coachview/coachview.h \
    entity/coachview/mainwindow.h \
    build/tmp/moc/ui_mainwindow.h \
    entity/player/skills/basics/skill_sample.h

FORMS += \
    entity/coachview/mainwindow.ui

RESOURCES += \
    rsc.qrc

DISTFILES += \
    constraints/agressivity_clusters.json
