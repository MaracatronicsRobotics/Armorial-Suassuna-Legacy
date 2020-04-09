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

#ifndef ENTITY_HH
#define ENTITY_HH
#include <QThread>
#include <QMutex>
class Entity : public QThread {
public:
    enum EntityType {ENT_WORLD, ENT_PLAYER, ENT_CONTROLMODULE, ENT_SENSOR, ENT_REFEREE, ENT_GUI, ENT_GRSIMULATOR};
    Entity(EntityType type);
    int entityId() const { return _id; }
    EntityType entityType() const { return _type; }
    void setEntityPriority(int p);
    int entityPriority();
    void setLoopTime(float ms);
    void setLoopFrequency(int hz);
    float loopTime();
    void enable(bool value);
    bool isEnabled();
    void stopEntity();
    // Entity name (class name)
    virtual QString name() = 0;
private:
    void run();
    bool isRunning();
    // Virtual methods to be implemented
    virtual void initialization() = 0;
    virtual void loop() = 0;
    virtual void finalization() = 0;
    volatile int _id;
    volatile EntityType _type;
    static int _idcont;
    bool _running;
    bool _enabled;
    int _priority;
    float _loopTime;
    QMutex _mutexRunning;
    QMutex _mutexEnabled;
    QMutex _mutexPriority;
    QMutex _mutexLoopTime;
};
#endif // ENTITY_HH
