#ifndef ArmStepperControl_h
#define ArmStepperControl_h
#include "Arduino.h"
#include <AccelStepper.h>
#include <MultiStepper.h>

class ArmStepperControl
{

private:
    int dirPin;      // DIR- pin connection
    int pulPin;      // PUL- pin connection
    int direction;   // Normal direction of rotation (away from zero)
    int stepPerRev;  // Number of steps per revolution
    float gearRatio; // Gear ratio
    int minPos;      // Mimimum position
    int maxPos;      // Maximum position
    int zeroPos;     // Position that the stepper zeros to

    AccelStepper *stepperMotor; // Pointer to stepper motor

    int currPos = -1;   // Temporary definition to prevent null pointer exception
    int desiredPos = 0; // Temporary definition to prevent null pointer

public:
    // Constructor and Deconstructor
    ArmStepperControl(int dirPin, int pulPin, char sideOfArm, int stepPerRev, float gearRatio, int maxPos, int minPos, int zeroPos);
    ~ArmStepperControl();

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
    float getPosition();   // Gets the current position from the stepper motor
    float getSpeed();      // Gets the current max speed
    float getMaxPos();     // Gets the max position possible
    float getDesiredPos(); // Gets the desired position
};
#endif