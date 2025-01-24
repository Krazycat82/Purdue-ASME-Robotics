#include "FullArmControl.h"
#include "Arduino.h"
#include "SegmentControlStepper.h"
#include "SegmentControlMotor.h"

FullArmControl::FullArmControl(SegmentControlStepper *wristPitchControl, SegmentControlStepper *foreArmPitchControl, SegmentControlMotor *upperArmPitchControl, SegmentControlStepper *shoulderYawControl, float upperArmLength, float foreArmLength, float wristLength)
{
    this->wristPitchControl = wristPitchControl;
    this->foreArmPitchControl = foreArmPitchControl;
    this->upperArmPitchControl = upperArmPitchControl;
    this->shoulderYawControl = shoulderYawControl;

    this->upperArmLength = upperArmLength;
    this->foreArmLength = foreArmLength;
    this->wristLength = wristLength;
}

FullArmControl::~FullArmControl()
{
    delete wristPitchControl;
    delete foreArmPitchControl;
    delete upperArmPitchControl;
    delete shoulderYawControl;
}

bool FullArmControl::calculateMovement(int speed)
{
    float foreArmInnerAngle; // Angle between the upper arm and the fore arm
    float hypotenuse;        // hypotenuse of height radius triangle
    
    float prevWristAngle = wristPitchControl->getAngle();
    float prevFAAngle = foreArmPitchControl->getAngle();
    float prevUAAngle = upperArmPitchControl->getAngle();
    float prevShoulderAngle = shoulderYawControl->getAngle();

    hypotenuse = hypot(height, radius);
    foreArmInnerAngle = degrees(acos((pow(upperArmLength, 2) + pow(foreArmLength, 2) - pow(hypotenuse, 2)) / float(2 * upperArmLength * foreArmLength)));

    foreArmPitch = foreArmInnerAngle - 180;
    upperArmPitch = degrees(asin((foreArmLength / hypotenuse) * sin(radians(foreArmInnerAngle))) + asin(height / hypotenuse));

    returnableNum = foreArmInnerAngle;

    wristAngle = wristAngle - upperArmPitch - foreArmPitch;

    float motorSpeed = 0.85 * speed;

    if (prevUAAngle > upperArmPitch) {
        motorSpeed = 0.1 * speed;
    }

    bool wrist = wristPitchControl->setUpMove(wristAngle, speed);
    bool foreArm = foreArmPitchControl->setUpMove(foreArmPitch, 2 * speed);
    bool upperArm = upperArmPitchControl->setUpMove(upperArmPitch,  motorSpeed);
    bool shoulderYaw = shoulderYawControl->setUpMove(yaw, speed);

    /*/
    if (!(wrist && foreArm && upperArm && shoulderYaw)) {
        wrist = wristPitchControl->setUpMove(prevWristAngle, speed);
        foreArm = foreArmPitchControl->setUpMove(prevFAAngle, 2 * speed);
        upperArm = upperArmPitchControl->setUpMove(prevUAAngle, speed);
        shoulderYaw = shoulderYawControl->setUpMove(prevShoulderAngle, speed);
    }
    */

    return wrist && foreArm && upperArm && shoulderYaw;
}

void FullArmControl::zero()
{
    wristPitchControl->zero();

    delay(500);

    foreArmPitchControl->zero();

    delay(500);

    upperArmPitchControl->zero();
}

void FullArmControl::run()
{
    wristPitchControl->runMove();
    foreArmPitchControl->runMove();
    upperArmPitchControl->runMove();
    shoulderYawControl->runMove();
}

void FullArmControl::enable()
{
    wristPitchControl->enable();
    foreArmPitchControl->enable();
    upperArmPitchControl->enable();
    shoulderYawControl->enable();
}

void FullArmControl::stepperEnable()
{
    wristPitchControl->enable();
    foreArmPitchControl->enable();
    shoulderYawControl->enable();
}

void FullArmControl::motorEnable()
{
    upperArmPitchControl->enable();
}

void FullArmControl::disable()
{
    stop();

    wristPitchControl->disable();
    foreArmPitchControl->disable();
    upperArmPitchControl->disable();
    shoulderYawControl->disable();
}

void FullArmControl::stop()
{
    wristPitchControl->stop();
    foreArmPitchControl->stop();
    upperArmPitchControl->stop();
    shoulderYawControl->stop();
}

void FullArmControl::setDesiredPosition(float yaw, float radius, float height, float wristAngle)
{
    this->yaw = yaw;
    this->radius = radius - wristLength * cos(radians(wristAngle));
    this->height = height - wristLength * sin(radians(wristAngle));
    this->wristAngle = wristAngle;
}

float FullArmControl::getDesiredYaw()
{
    return yaw;
}

float FullArmControl::getDesiredRadius()
{
    return radius;
}

float FullArmControl::getDesiredHeight()
{
    return height;
}

float FullArmControl::getCurrentYaw()
{
    return shoulderYawControl->getAngle();
}

float FullArmControl::getCurrentRadius()
{
    float upperArmPitch = upperArmPitchControl->getAngle();
    float foreArmPitch = foreArmPitchControl->getAngle();
    float wristPitch = wristPitchControl->getAngle();

    return upperArmLength * cos(radians(upperArmPitch)) + foreArmLength * cos(radians(upperArmPitch + foreArmPitch));
}

float FullArmControl::getCurrentHeight()
{
    float upperArmPitch = upperArmPitchControl->getAngle();
    float foreArmPitch = foreArmPitchControl->getAngle();
    float wristPitch = wristPitchControl->getAngle();

    return upperArmLength * sin(radians(upperArmPitch)) + foreArmLength * sin(radians(upperArmPitch + foreArmPitch));
}

float FullArmControl::getWristPitch()
{
    return wristAngle;
}

float FullArmControl::getForeArmPich()
{
    return foreArmPitch;
}

float FullArmControl::getUpperArmPitch()
{
    return upperArmPitch;
}

float FullArmControl::getNum()
{
    return returnableNum;
}

int FullArmControl::getWristDesiredPos()
{
    return wristPitchControl->getDesiredPos();
}

int FullArmControl::getForeArmDesiredPos()
{
    return foreArmPitchControl->getDesiredPos();
}

float FullArmControl::getWristSpeed()
{
    return wristPitchControl->getSpeed();
}

float FullArmControl::getRadiusError()
{
    return getCurrentRadius() - getDesiredRadius();
}

float FullArmControl::getHeightError()
{
    return getCurrentHeight() - getDesiredHeight();
}

float FullArmControl::getWristError()
{
    return 0;
}
