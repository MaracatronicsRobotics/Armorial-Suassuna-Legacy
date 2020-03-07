#ifndef WORLD_HH
#define WORLD_HH

#include <QReadWriteLock>
#include <iostream>
#include <entity/baseentity.h>
#include <entity/player/baseplayer.h>
#include <entity/entity.h>
#include <utils/fields/fields.hh>
#include <entity/world/worldmapupdater.h>
#include <entity/referee/SSLReferee/sslreferee.h>

#include <entity/coachview/coachview.h>

class World : public Entity {
public:
    World(Controller *ctr, Fields::Field *defaultField, CoachView *ourGUI);
    ~World();
    QString name();

    // Management
    void setTeams(MRCTeam *ourTeam, MRCTeam *theirTeam);
    void setControlModule(ControlModule *ctrModule);

    // Entities management
    void addEntity(Entity *e, int priority);
    void removeEntity(int id);

    // WorldMap read-write synchronization
    void wmLockRead();
    void wmUnlock();

    // Team pointer returns
    MRCTeam* ourTeam() { return _ourTeam; }
    MRCTeam* theirTeam() { return _theirTeam; }

    // Controller/WorldMap access
    Controller* getController() const { return _ctr; }
    WorldMap* getWorldMap() const { return _wm; }
private:
    // Entity implementation
    void initialization();
    void loop();
    void finalization();

    // Controller access
    Controller *_ctr;

    // Teams access
    MRCTeam *_ourTeam;
    MRCTeam *_theirTeam;

    // WorldMap
    WorldMap *_wm;
    WorldMapUpdater *_wmUpdater;

    // Control module
    ControlModule *_ctrModule;
    QMutex _ctrModuleMutex;

    // WorldMap read-write synchronization
    QReadWriteLock _wmLock;
    void wmLockWrite();

    // Entities by priority
    QMap<int,QHash<int,Entity*>*> _priorityLevels; // Map<priority, Hash<id,Entity> >

    // Internal
    void setupWorldMap();
    void stopAndDeleteEntities();

    // GUI
    CoachView *_ourGUI;
};

#endif // WORLD_HH
