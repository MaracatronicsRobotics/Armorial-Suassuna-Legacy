#ifndef CONSTANTS_H
#define CONSTANTS_H

class MRCConstants
{
private:
    static int _threadFrequency;
public:
    MRCConstants();

    static int threadFrequency() { return _threadFrequency; }
};

#endif // CONSTANTS_H
