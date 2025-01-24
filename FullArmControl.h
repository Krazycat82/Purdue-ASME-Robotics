#ifndef FullArmControl_h
#define FullArmControl_h
#include "Arduino.h"
#include "SegmentControlStepper.h"
#include "SegmentControlMotor.h"

class FullArmControl
{

private:
    SegmentControlStepper *wristPitchControl;
    SegmentControlStepper *foreArmPitchControl;
    SegmentControlMotor *upperArmPitchControl;
    SegmentControlStepper *shoulderYawControl;

    // All units are inches or degrees

    float yaw;               // Yaw rotation of shoulder
    float radius;            // Radius of control point from center of rotation
    float height;            // Height above shoulder pitch rotation axis
    float wristAngle;        // Angle of the wrist from the horizontal plane
    float foreArmPitch = 0;  // Angle of the fore arm from the horizontal plane
    float upperArmPitch = 0; // Angle of the upper arm from the horizontal plane
    float returnableNum = 0; // Whatever number i need to return

    float upperArmLength; // Length of the upper arm
    float foreArmLength;  // Length of the fore arm
    float wristLength;    // Length of the wrist

public:
    // Constructor and Deconstructor
    FullArmControl(SegmentControlStepper *wristPitchControl, SegmentControlStepper *foreArmPitchControl, SegmentControlMotor *upperArmPitchControl, SegmentControlStepper *shoulderYawControl, float upperArmLength, float foreArmLength, float wristLength);
    ~FullArmControl();

    // Control Functions
    bool calculateMovement(int speed);
    void zero();
    void run();
    void enable();
    void stepperEnable();
    void motorEnable();
    void disable();
    void stop();

    // Setters and Getters
    void setDesiredPosition(float yaw, float radius, float height, float wristAngle);
    float getDesiredYaw();
    float getDesiredRadius();
    float getDesiredHeight();
    float getCurrentYaw();
    float getCurrentRadius();
    float getCurrentHeight();
    float getWristPitch();
    float getForeArmPich();
    float getUpperArmPitch();
    float getNum();
    int getWristDesiredPos();
    int getForeArmDesiredPos();
    float getWristSpeed();
    float getRadiusError();
    float getHeightError();
    float getWristError();
};

#endif