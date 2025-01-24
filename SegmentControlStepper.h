#ifndef SegmentControlStepper_h
#define SegmentControlStepper_h
#include "Arduino.h"
#include "ArmStepperControl.h"

class SegmentControlStepper
{
private:
    ArmStepperControl *stepperList; // List of stepper motors on the arm

    int numSteppers;                // The number of stepper motors on the arm segment (the length of the stepper list)
    int zeroPort;                   // The port connected to the zero switch
    int angle;                      // Current angle of the arm
    int passiveAngle;               // Angle of Segment when not in use
    int zeroDirection; // The direction the segment must move to contact the zero switch

public:
    // Constructor and Deconstructor
    SegmentControlStepper(int numSteppers, ArmStepperControl *stepperList, int zeroPort, int zeroDirection, int passiveAngle);
    ~SegmentControlStepper();

    // Control Functions
    void zero();                            // Moves the motor until it hits the zero switch and then zeros the stepper motor(s)
    bool setUpMove(float angle, int speed); // Moves the segment to a specific angle
    void runMove();                         // Makes the movement
    void stop();                            // Stops the motor from running
    void enable();                          // Allows signals to the stepper motor driver
    void disable();                         // Diables signals to the stepper motor driver

    // Setters and Getters
    float getAngle(); // Gets the current angle of the segment
    bool getSensor(); // Gets the value of the zero switch
    int getDesiredPos();
    float getSpeed();
};

#endif