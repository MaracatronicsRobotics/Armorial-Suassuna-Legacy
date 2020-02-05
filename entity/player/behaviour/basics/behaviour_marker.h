#ifndef BEHAVIOUR_MARKER_H
#define BEHAVIOUR_MARKER_H

#include <entity/player/behaviour/behaviour.h>
#include <entity/player/skills/skills_include.h>

class Behaviour_Marker : public Behaviour {
private:
	void configure();
	void run();
	int _state;
	Skill_GoToLookTo *_sk_GoToLookTo;

public:
	Behaviour_Marker();
	QString name();
};

#endif // BEHAVIOUR_MARKER_H
