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

#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include <src/utils/timer/timer.h>

/**
 * @brief The Entity class is an extension of QThread, which contains all methods necessary to built an threaded class
 */
class Entity : public QThread
{
public:
    Entity(); /*!< Default constructor */
    virtual QString name() = 0; /*!< @return name of the service */

    // Setters
    void setLoopFrequency(int hz); /*!< Set the frequency of loop() @param hz The frequency in hertz */
    void enableEntity(); /*!< Enable the entity */
    void disableLoop(); /*!< Disable the loop() method call */
    void stopEntity(); /*!< Stop the entity, casting finalization() method later */

    // Getters
    int loopFrequency(); /*!< @returns The loop frequency */
    bool isEnabled(); /*!< @returns If the entity is enabled */
    bool isLoopEnabled(); /*!< @returns If the loop() method call is enabled */
    int entityId(); /*!< @returns The unique entity id */

private:
    // Main run method
    void run(); /*!< Implementation of QThread which contains the structure to call the virtual methods */

    // Virtual methods
    virtual void initialization() = 0; /*!< This method is responsible for several initializations which NEED to run in the thread (like QTimer) */
    virtual void loop() = 0; /*!< This method needs to implement what will be called in 'hz' times per second */
    virtual void finalization() = 0; /*!< This method implements all necessary calls which NEED to run in the thread (like QTimer destructions) */

    // Entity info
    int _loopFrequency; /*!< Stores the loop frequency (starts with 60 by default) */
    bool _isEnabled; /*!< Stores the Entity enabled status (starts with false by default) */
    bool _loopEnabled; /*!< Stores the loop() method call status (starts with true by default) */
    static int _entityId; /*!< Stores an unique id for entity */

    // Entity timer
    Timer _entityTimer; /*!< Timer which will be used to execute the loop() method in the determined frequency */
    void startTimer(); /*!< Auxiliar method used to start the Timer */
    void stopTimer(); /*!< Auxiliar method used to stop the Timer */
    long getRemainingTime(); /*!< @returns time passed between the startTimer() and stopTimer() calls */

    // Entity mutexes
    QMutex _mutexEnabled; /*!< Mutex to avoid problems with the Entity status changes */
    QMutex _mutexPriority; /*!< Mutex to avoid problems with the priority changes */
    QMutex _mutexLoop; /*!< Mutex to avoid problems with loop() related changes */
};

#endif // ENTITY_H
