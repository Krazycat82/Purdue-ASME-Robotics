#include "ArmStepperControl.h"
#include "Arduino.h"
#include <AccelStepper.h>
#include <MultiStepper.h>

#define DEGREES_PER_REVOLUTION 360

// Constructor and Deconstructor
ArmStepperControl::ArmStepperControl(int dirPin, int pulPin, char sideOfArm, int stepPerRev, float gearRatio, int maxPos, int minPos, int zeroPos)
{
    if (sideOfArm == 'L')
    {
        direction = 1;
    }
    else if (sideOfArm == 'R')
    {
        direction = -1;
    }
    else
    {
        direction = 0;
    }
    
    this->dirPin = dirPin;
    this->pulPin = pulPin;
    this->stepPerRev = stepPerRev;
    this->gearRatio = gearRatio;
    this->maxPos = maxPos;
    this->minPos = minPos;
    this->zeroPos = direction * zeroPos;

    

    stepperMotor = new AccelStepper(1, pulPin, dirPin);
    stepperMotor->setAcceleration(1000);
}

ArmStepperControl::~ArmStepperControl()
{
    delete stepperMotor;
}

// Control Functions
void ArmStepperControl::zero()
{
    disable();
    stepperMotor->setCurrentPosition(zeroPos / gearRatio * stepPerRev / DEGREES_PER_REVOLUTION);
    enable();
}

void ArmStepperControl::setPosition(float angle)
{
    disable();
    stepperMotor->setCurrentPosition(angle / gearRatio * stepPerRev / DEGREES_PER_REVOLUTION);
    enable();
}

bool ArmStepperControl::setUpMove(float angle, int speed)
{
    bool capable = false;

    stepperMotor->setMaxSpeed(speed / gearRatio * stepPerRev / DEGREES_PER_REVOLUTION);
    if (angle <= maxPos && angle >= minPos)
    {
        stepperMotor->moveTo(0);
        
        desiredPos = direction * angle / gearRatio * stepPerRev / DEGREES_PER_REVOLUTION;
        stepperMotor->moveTo(desiredPos);
        capable = true;
    }

    return capable;
}

void ArmStepperControl::setUpMoveOveride(float angle, int speed)
{
    stepperMotor->setMaxSpeed(speed / gearRatio * stepPerRev / DEGREES_PER_REVOLUTION);
    stepperMotor->moveTo(direction * angle / gearRatio * stepPerRev / DEGREES_PER_REVOLUTION);
}

void ArmStepperControl::runMove()
{
    stepperMotor->run();
}

void ArmStepperControl::enable()
{
    stepperMotor->enableOutputs();
}

void ArmStepperControl::stop()
{
    stepperMotor->stop();
}

void ArmStepperControl::disable()
{
    stepperMotor->disableOutputs();
}

// Setters and Getters
float ArmStepperControl::getPosition()
{
    return (float(stepperMotor->currentPosition()) * gearRatio / stepPerRev * DEGREES_PER_REVOLUTION);
}

float ArmStepperControl::getSpeed()
{
    return stepperMotor->maxSpeed();
}

float ArmStepperControl::getMaxPos()
{
    return maxPos;
}

float ArmStepperControl::getDesiredPos()
{
    return desiredPos;
}
