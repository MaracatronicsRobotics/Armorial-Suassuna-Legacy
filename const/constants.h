#ifndef CONSTANTS_H
#define CONSTANTS_H

class MRCConstants
{
private:
    static int _threadFrequency;
public:
    MRCConstants();

    static int threadFrequency() { return _threadFrequency; }

    /* Fast Path Planning */
    static float _FPPBallThreshHold;
    static float _FPPRobotThreshHold;
    static float _FPPBreakDistance;
    static float _FPPSmoothPathResolution;
};

#endif // CONSTANTS_H
