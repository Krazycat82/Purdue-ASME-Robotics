#ifndef ArmMotorControl_h
#define ArmMotorControl_h
#include "Arduino.h"
#include <CytronMotorDriver.h>
#include <Encoder.h>
#include "PID.h"

class ArmMotorControl
{

private:
    float gearRatio; // Gear ratio
    int minPos;      // Mimimum position
    int maxPos;      // Maximum position
    int zeroPos;     // Position that the stepper zeros to
    double PIDValue; // Valye of the PID

    CytronMD *motor;  // Pointer to motor
    Encoder *encoder; // Pointer to motor encoder
    PID *motorPID;    // Pointer to motor PID

    int currPos = 0;      // Temporary definition to prevent null pointer exception
    int maxSpeed = 0;     // Temporary definition to prevent null pointer exception
    int prevMaxSpeed = 0; // Temporary definition to prevent null pointer exception

public:
    // Constructor and Deconstructor
    ArmMotorControl(int dirPin, int pwmPin, int chAPin, int chBPin, float gearRatio, int maxPos, int minPos, int zeroPos, float kP, float kI, float kD);
    ~ArmMotorControl();

    // Control Functions
    void zero();                                   // Zeros the stepper motor
    void setPosition(float angle);                 // Set the current angle of the stepper motor
    bool setUpMove(float angle, int speed);        // Designates the desired angle for the stepper motor
    void setUpMoveOveride(float angle, int speed); // Designates the desired angle for the stepper mover and overides limit controls
    void runMove();                                // Moves the stepper motor
    void enable();                                 // Enables signals to the stepper motor
    void stop();                                   // Stops the stepper motor from moving
    void disable();                                // Disables signals from the stepper motor

    // Setters and Getters
    float getPosition();  // Gets the current position from the stepper motor
    float getSpeed();     // Gets the current max speed
    float getMaxPos();    // Gets the max position possible
    double getPIDValue(); // Gets the PIDValue
};

#endif