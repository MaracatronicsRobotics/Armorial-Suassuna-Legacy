#ifndef CONTROLMODULE_H
#define CONTROLMODULE_H

#include <QString>
#include <QReadWriteLock>

class ControlModule {
private:
    virtual void run() = 0;
public:
    ControlModule();
    virtual ~ControlModule();
    virtual QString name() = 0;
    // Called in World loop
    void runControlModule();
};

#endif // CONTROLMODULE_H
