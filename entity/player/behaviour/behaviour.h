/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <entity/player/baseplayer.h>
#include <entity/player/playeraccess.h>
#include <entity/player/playerbus.h>
#include <QObject>

class Behaviour : public QObject {
    Q_OBJECT
public:
    Behaviour();
    virtual ~Behaviour();

    // Called one time before run is first called
    bool isInitialized() { return _initialized; }
    void initialize(Locations *loc);
    void setPlayer(Player *player, PlayerAccess *playerAccess);

    // Called in Player loop
    void runBehaviour();
    virtual QString name() = 0;

protected:
    void usesSkill(Skill *skill);
    void setInitialSkill(Skill *skill);
    void addTransition(int id, Skill *source, Skill *target);
    void enableTransition(int id);

    // Filtered player access on behavior
    PlayerAccess* player();
    Locations *loc();
private:
    class SkillTransition {
    private:
        Skill *_source, *_target;
    public:
        SkillTransition(Skill *source, Skill *target) {
            _source = source;
            _target = target;
        }
        Skill* source() const { return _source; }
        Skill* target() const { return _target; }
    };

    // Implemented by children behaviour
    virtual void configure() = 0;
    virtual void run() = 0;
    bool _configureEnabled;

    // Player access
    Player *_player;
    PlayerAccess *_playerAccess;

    // Game info
    Locations *_loc;

    // State machine encapsulation
    QHash<int,SkillTransition*> _transitionTable;
    QList<Skill*> _skillList;
    Skill *_skill;

    // Behavior initialized
    bool _initialized;
};
#endif // BEHAVIOUR_H
