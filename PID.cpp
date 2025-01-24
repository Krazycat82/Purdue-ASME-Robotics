#include "PID.h"
#include "Arduino.h"

PID::PID(double kP, double kI, double kD)
{
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
    
    pastTime = millis();
}

PID::~PID()
{
}

double PID::calculate(double currPos)
{
    double error = setPoint - currPos;
    double currTime = millis();

    double P = kP * error;
    double I = kI * error * (currTime - pastTime);
    double D = kD * (error - pastError) / (currTime - pastTime);

    return P + I + D;
}

void PID::changeSetPoint(double setPoint)
{
    this->setPoint = setPoint;
    reset();
}

void PID::reset()
{
    pastError = 0;
}
