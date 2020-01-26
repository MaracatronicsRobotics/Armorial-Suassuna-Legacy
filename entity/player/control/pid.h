#ifndef PID_H
#define PID_H

#include <utils/mrctimer/mrctimer.h>

class PID
{
public:
    PID();
    PID(double kp, double ki, double kd, double max, double min);
    ~PID();
    void setPIDParameters(double kp, double kd, double ki, double max, double min);
    double calculate(double desired, double actual);
private:
    MRCTimer *_timer; // timer to get _dt
    double _kp, _ki, _kd, _dt; // pid terms
    double _max, _min; // max and min values
    double _integral, _pre_error; // save last error and integral sum
};

#endif // PID_H
