#ifndef MRCSTRATEGY_H
#define MRCSTRATEGY_H

#include <entity/contromodule/strategy/basics/sslstrategy.h>

class MRCStrategy : public SSLStrategy {
private:
    void configure();
public:
    MRCStrategy();
    QString name();
};

#endif // MRCSTRATEGY_H
