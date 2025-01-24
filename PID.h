#ifndef PID_h
#define PID_h
#include "Arduino.h"

class PID
{

private:
    double kP;
    double kI;
    double kD;
    double pastTime;
    double pastError = 0; // Temporary definition to prevent null pointer exception
    double setPoint = 0;  // Temporary definition to prevent null pointer exception

public:
    // Constructor and Deconstructor
    PID(double kP, double kI, double kD);
    ~PID();

    // Functions
    double calculate(double currPos);     // Calculates the motor control value
    void changeSetPoint(double setPoint); // Changes the set point
    void reset();                         // Resets the PID to have 0 past error
};
#endif