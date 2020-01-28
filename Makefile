
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o behaviour_timeout.o entity/player/behaviour/basics/behaviour_timeout.cpp

sslstrategy_timeout.o: entity/contromodule/strategy/basics/sslstrategy_timeout.cpp entity/contromodule/strategy/basics/sslstrategy_timeout.h \
		entity/contromodule/strategy/strategystate.h \
		entity/contromodule/basecoach.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/contromodule/playbook/mrcplaybook.h \
		entity/contromodule/playbook/basics/playbook_donothing.h \
		entity/contromodule/playbook/playbook.h \
		entity/referee/SSLReferee/sslreferee.h \
		entity/referee/referee.h \
		entity/entity.h \
		entity/contromodule/mrcteam.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		entity/player/player.h \
		entity/world/world.h \
		entity/baseentity.h \
		entity/player/baseplayer.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		utils/utils.hh \
		entity/locations.h \
		utils/basics/wall.hh \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h \
		entity/contromodule/playersdistribution.h \
		utils/wrtimer/wrtimer.hh \
		entity/player/behaviour/behaviour.h \
		entity/player/playeraccess.h \
		entity/player/behaviour/mrcbehaviours.h \
		entity/player/behaviour/basics/behaviour_donothing.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/skill.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/behaviour/basics/behaviour_followball.h \
		entity/player/behaviour/basics/behaviour_timeout.h \
		entity/player/behaviour/basics/behaviour_goalkeeper.h \
		entity/contromodule/playbook/basics/playbook_followball.h \
		entity/contromodule/playbook/basics/playbook_timeout.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sslstrategy_timeout.o entity/contromodule/strategy/basics/sslstrategy_timeout.cpp

pid.o: entity/player/control/pid.cpp entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o pid.o entity/player/control/pid.cpp

mrctimer.o: utils/mrctimer/mrctimer.cpp utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mrctimer.o utils/mrctimer/mrctimer.cpp

skill_kick.o: entity/player/skills/basics/skill_kick.cpp entity/player/skills/basics/skill_kick.h \
		entity/player/skills/skill.h \
		entity/player/baseplayer.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o skill_kick.o entity/player/skills/basics/skill_kick.cpp

behaviour_goalkeeper.o: entity/player/behaviour/basics/behaviour_goalkeeper.cpp entity/player/behaviour/basics/behaviour_goalkeeper.h \
		entity/player/behaviour/behaviour.h \
		entity/player/baseplayer.h \
		entity/player/playeraccess.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/skill.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h \
		utils/knn/knn.hh \
		entity/contromodule/basecoach.h \
		entity/player/playerbus.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o behaviour_goalkeeper.o entity/player/behaviour/basics/behaviour_goalkeeper.cpp

skill_goalkeeper.o: entity/player/skills/basics/skill_goalkeeper.cpp entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/skills/skill.h \
		entity/player/baseplayer.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o skill_goalkeeper.o entity/player/skills/basics/skill_goalkeeper.cpp

skill_gkick.o: entity/player/skills/basics/skill_gkick.cpp entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/skill.h \
		entity/player/baseplayer.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o skill_gkick.o entity/player/skills/basics/skill_gkick.cpp

moc_playbook.o: moc_playbook.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_playbook.o moc_playbook.cpp

moc_behaviour.o: moc_behaviour.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_behaviour.o moc_behaviour.cpp

####### Install

install_target: first FORCE
	@test -d $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin || mkdir -p $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin
	-$(QINSTALL_PROGRAM) $(QMAKE_TARGET) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/$(QMAKE_TARGET)
	-$(STRIP) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/$(QMAKE_TARGET)

uninstall_target: FORCE
	-$(DEL_FILE) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/$(QMAKE_TARGET)
	-$(DEL_DIR) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/ 


install: install_target  FORCE

uninstall: uninstall_target  FORCE

FORCE:


		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/skills/basics/skill_gkick.h entity/entity.cpp \
	$(COPY_FILE) --parents entity/baseentity.h entity/entity.h entity/player/skills/basics/skill_aroundtheball.h entity/player/skills/basics/skill_dribble.h entity/player/skills/basics/skill_goto.h entity/player/skills/basics/skill_gotolookto.h entity/player/skills/basics/skill_rotateto.h entity/world/world.h entity/world/worldmapupdater.h exithandler.h include/3rd_party/netraw.h include/3rd_party/robocup_ssl_client.h include/3rd_party/util.h include/filters.h include/3rd_party/messages_robocup_ssl_detection.pb.h include/3rd_party/messages_robocup_ssl_geometry.pb.h include/3rd_party/messages_robocup_ssl_wrapper.pb.h include/3rd_party/game_event.pb.h include/3rd_party/messages_robocup_ssl_refbox_log.pb.h include/3rd_party/referee.pb.h include/3rd_party/grSim_Commands.pb.h include/3rd_party/grSim_Packet.pb.h include/3rd_party/grSim_Replacement.pb.h entity/referee/SSLReferee/sslgameinfo.h entity/referee/referee.h entity/referee/SSLReferee/sslreferee.h instancechecker.h suassuna.h utils/basics/color.hh utils/basics/competition.hh utils/basics/quadrant.hh utils/basics/side.hh utils/basics/wall.hh utils/fields/field_ssl2014.hh utils/fields/field_ssl2015.hh utils/fields/field_vss2008.hh utils/fields/fields.hh utils/fields/wrfields.hh utils/fieldside/fieldside.hh utils/graph/basegraph.hh utils/graph/edge.hh utils/graph/graph.hh utils/graph/vertex.hh utils/line/line.hh utils/utils.hh utils/wrtimer/wrtimer.hh entity/player/player.h entity/contromodule/mrcteam.h entity/player/skills/skill.h entity/locations.h entity/player/skills/skills_include.h entity/contromodule/grsSimulator/grsSimulator.h entity/contromodule/coach.h entity/player/playerbus.h entity/player/playeraccess.h entity/contromodule/strategy/strategy.h entity/contromodule/basecoach.h entity/contromodule/playersdistribution.h entity/contromodule/strategy/strategystate.h entity/contromodule/coachutils.h entity/contromodule/playbook/playbook.h entity/player/behaviour/behaviour.h entity/player/behaviour/mrcbehaviours.h entity/player/behaviour/basics/behaviour_donothing.h entity/player/skills/basics/skill_donothing.h utils/knn/knn.hh entity/contromodule/strategy/basics/mrcstrategy.h entity/contromodule/strategy/basics/sslstrategy.h entity/contromodule/controlmodule.h entity/contromodule/strategy/basics/sslstrategy_halt.h entity/contromodule/playbook/basics/playbook_donothing.h entity/contromodule/playbook/mrcplaybook.h entity/player/behaviour/basics/behaviour_followball.h entity/contromodule/playbook/basics/playbook_followball.h entity/contromodule/playbook/basics/playbook_timeout.h entity/player/behaviour/basics/behaviour_timeout.h entity/contromodule/strategy/basics/sslstrategy_timeout.h entity/contromodule/strategy/mrcstrategies.h entity/player/control/pid.h utils/mrctimer/mrctimer.h entity/player/skills/basics/skill_kick.h entity/player/behaviour/basics/behaviour_goalkeeper.h entity/player/skills/basics/skill_goalkeeper.h entity/player/skills/basics/skill_gkick.h $(DISTDIR)/
	$(COPY_FILE) --parents entity/entity.cpp entity/player/skills/basics/skill_aroundtheball.cpp entity/player/skills/basics/skill_dribble.cpp entity/player/skills/basics/skill_goto.cpp entity/player/skills/basics/skill_gotolookto.cpp entity/player/skills/basics/skill_rotateto.cpp entity/world/world.cpp entity/world/worldmapupdater.cpp exithandler.cpp include/3rd_party/messages_robocup_ssl_detection.pb.cc include/3rd_party/netraw.cpp include/3rd_party/robocup_ssl_client.cpp include/3rd_party/messages_robocup_ssl_geometry.pb.cc include/3rd_party/messages_robocup_ssl_refbox_log.pb.cc include/3rd_party/messages_robocup_ssl_wrapper.pb.cc include/3rd_party/game_event.pb.cc include/3rd_party/grSim_Commands.pb.cc include/3rd_party/grSim_Packet.pb.cc include/3rd_party/grSim_Replacement.pb.cc include/3rd_party/referee.pb.cc entity/referee/SSLReferee/sslgameinfo.cpp entity/referee/referee.cpp entity/referee/SSLReferee/sslreferee.cpp instancechecker.cpp main.cpp suassuna.cpp utils/basics/color.cc utils/basics/competition.cc utils/basics/quadrant.cc utils/basics/side.cc utils/basics/wall.cc utils/fields/field_ssl2014.cc utils/fields/field_ssl2015.cc utils/fields/field_vss2008.cc utils/fields/fields.cc utils/fieldside/fieldside.cc utils/graph/edge.cc utils/graph/graph.cc utils/graph/vertex.cc utils/line/line.cc utils/utils.cc utils/wrtimer/wrtimer.cc entity/player/player.cpp entity/contromodule/mrcteam.cpp entity/player/skills/skill.cpp entity/locations.cpp entity/contromodule/grsSimulator/grsSimulator.cpp entity/contromodule/coach.cpp entity/player/playerbus.cpp entity/player/playeraccess.cpp entity/contromodule/strategy/strategy.cpp entity/contromodule/playersdistribution.cpp entity/contromodule/strategy/strategystate.cpp entity/contromodule/coachutils.cpp entity/contromodule/playbook/playbook.cpp entity/player/behaviour/behaviour.cpp entity/player/behaviour/basics/behaviour_donothing.cpp entity/player/skills/basics/skill_donothing.cpp utils/knn/knn.cc entity/contromodule/strategy/basics/mrcstrategy.cpp entity/contromodule/strategy/basics/sslstrategy.cpp entity/contromodule/controlmodule.cpp entity/contromodule/strategy/basics/sslstrategy_halt.cpp entity/contromodule/playbook/basics/playbook_donothing.cpp entity/player/behaviour/basics/behaviour_followball.cpp entity/contromodule/playbook/basics/playbook_followball.cpp entity/contromodule/playbook/basics/playbook_timeout.cpp entity/player/behaviour/basics/behaviour_timeout.cpp entity/contromodule/strategy/basics/sslstrategy_timeout.cpp entity/player/control/pid.cpp utils/mrctimer/mrctimer.cpp entity/player/skills/basics/skill_kick.cpp entity/player/behaviour/basics/behaviour_goalkeeper.cpp entity/player/skills/basics/skill_goalkeeper.cpp entity/player/skills/basics/skill_gkick.cpp $(DISTDIR)/
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o behaviour_timeout.o entity/player/behaviour/basics/behaviour_timeout.cpp

sslstrategy_timeout.o: entity/contromodule/strategy/basics/sslstrategy_timeout.cpp entity/contromodule/strategy/basics/sslstrategy_timeout.h \
		entity/contromodule/strategy/strategystate.h \
		entity/contromodule/basecoach.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/contromodule/playbook/mrcplaybook.h \
		entity/contromodule/playbook/basics/playbook_donothing.h \
		entity/contromodule/playbook/playbook.h \
		entity/referee/SSLReferee/sslreferee.h \
		entity/referee/referee.h \
		entity/entity.h \
		entity/contromodule/mrcteam.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		entity/player/player.h \
		entity/world/world.h \
		entity/baseentity.h \
		entity/player/baseplayer.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		utils/utils.hh \
		entity/locations.h \
		utils/basics/wall.hh \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h \
		entity/contromodule/playersdistribution.h \
		utils/wrtimer/wrtimer.hh \
		entity/player/behaviour/behaviour.h \
		entity/player/playeraccess.h \
		entity/player/behaviour/mrcbehaviours.h \
		entity/player/behaviour/basics/behaviour_donothing.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/skill.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/behaviour/basics/behaviour_followball.h \
		entity/player/behaviour/basics/behaviour_timeout.h \
		entity/player/behaviour/basics/behaviour_goalkeeper.h \
		entity/contromodule/playbook/basics/playbook_followball.h \
		entity/contromodule/playbook/basics/playbook_timeout.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o sslstrategy_timeout.o entity/contromodule/strategy/basics/sslstrategy_timeout.cpp

pid.o: entity/player/control/pid.cpp entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o pid.o entity/player/control/pid.cpp

mrctimer.o: utils/mrctimer/mrctimer.cpp utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mrctimer.o utils/mrctimer/mrctimer.cpp

skill_kick.o: entity/player/skills/basics/skill_kick.cpp entity/player/skills/basics/skill_kick.h \
		entity/player/skills/skill.h \
		entity/player/baseplayer.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o skill_kick.o entity/player/skills/basics/skill_kick.cpp

behaviour_goalkeeper.o: entity/player/behaviour/basics/behaviour_goalkeeper.cpp entity/player/behaviour/basics/behaviour_goalkeeper.h \
		entity/player/behaviour/behaviour.h \
		entity/player/baseplayer.h \
		entity/player/playeraccess.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/skill.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h \
		utils/knn/knn.hh \
		entity/contromodule/basecoach.h \
		entity/player/playerbus.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o behaviour_goalkeeper.o entity/player/behaviour/basics/behaviour_goalkeeper.cpp

skill_goalkeeper.o: entity/player/skills/basics/skill_goalkeeper.cpp entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/skills/skill.h \
		entity/player/baseplayer.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o skill_goalkeeper.o entity/player/skills/basics/skill_goalkeeper.cpp

skill_gkick.o: entity/player/skills/basics/skill_gkick.cpp entity/player/skills/basics/skill_gkick.h \
		entity/player/skills/skill.h \
		entity/player/baseplayer.h \
		entity/player/skills/skills_include.h \
		entity/player/skills/basics/skill_donothing.h \
		entity/player/skills/basics/skill_goto.h \
		entity/player/skills/basics/skill_rotateto.h \
		entity/player/skills/basics/skill_gotolookto.h \
		entity/player/skills/basics/skill_aroundtheball.h \
		entity/player/skills/basics/skill_kick.h \
		entity/player/skills/basics/skill_dribble.h \
		entity/player/skills/basics/skill_goalkeeper.h \
		entity/player/player.h \
		entity/entity.h \
		entity/world/world.h \
		entity/baseentity.h \
		utils/fields/fields.hh \
		entity/world/worldmapupdater.h \
		entity/referee/SSLReferee/sslreferee.h \
		include/3rd_party/referee.pb.h \
		include/3rd_party/game_event.pb.h \
		entity/referee/SSLReferee/sslgameinfo.h \
		utils/basics/color.hh \
		entity/referee/referee.h \
		utils/utils.hh \
		entity/locations.h \
		utils/fieldside/fieldside.hh \
		utils/basics/side.hh \
		utils/basics/wall.hh \
		entity/contromodule/mrcteam.h \
		entity/player/control/pid.h \
		utils/mrctimer/mrctimer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o skill_gkick.o entity/player/skills/basics/skill_gkick.cpp

moc_playbook.o: moc_playbook.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_playbook.o moc_playbook.cpp

moc_behaviour.o: moc_behaviour.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_behaviour.o moc_behaviour.cpp

####### Install

install_target: first FORCE
	@test -d $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin || mkdir -p $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin
	-$(QINSTALL_PROGRAM) $(QMAKE_TARGET) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/$(QMAKE_TARGET)
	-$(STRIP) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/$(QMAKE_TARGET)

uninstall_target: FORCE
	-$(DEL_FILE) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/$(QMAKE_TARGET)
	-$(DEL_DIR) $(INSTALL_ROOT)/opt/Armorial-Suassuna/bin/ 


install: install_target  FORCE

uninstall: uninstall_target  FORCE

FORCE:

