#include "Arduino.h"
#include "ArmStepperControl.h"
#include "SegmentControlStepper.h"

#define MOVEMENT_TOLERANCE 0.05

SegmentControlStepper::SegmentControlStepper(int numSteppers, ArmStepperControl *stepperList, int zeroPort, int zeroDirection, int passiveAngle)
{
    this->numSteppers = numSteppers;
    this->stepperList = stepperList;
    this->zeroPort = zeroPort;
    this->zeroDirection = zeroDirection;
    this->passiveAngle = passiveAngle;

    pinMode(zeroPort, INPUT_PULLUP);

    angle = 0;
}

SegmentControlStepper::~SegmentControlStepper()
{
    for (int i = 0; i < numSteppers; i++)
    {
        delete (stepperList + i);
    }

    free(stepperList);
}

void SegmentControlStepper::zero()
{
    // Zeroing Code
    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->setPosition(0);
    }

    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->setUpMoveOveride(360 * zeroDirection, 20);
    }

    while (digitalRead(zeroPort) == false)
    {
        runMove();
    }

    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->setUpMove(0, 5);
    }

    while (digitalRead(zeroPort) == true)
    {
        runMove();
    }

    stop();

    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->zero();
    }

    // Move to passive position code
    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->setUpMove(passiveAngle, 20);
    }

    while (abs(getAngle() - passiveAngle) > MOVEMENT_TOLERANCE)
    {
        runMove();
    }
}

bool SegmentControlStepper::setUpMove(float angle, int speed)
{
    int i = 0;           // Counter variable for while loop
    bool capable = true; // Tracks if the system is capable of making the movement w/o moving out of its limitations

    while (i < numSteppers && capable == true)
    {
        capable = (stepperList + i)->setUpMove(angle, speed);
        i++;
    }

    return capable;
}

void SegmentControlStepper::runMove()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->runMove();
    }
}

void SegmentControlStepper::stop()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->stop();
    }
}

void SegmentControlStepper::enable()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->enable();
    }
}

void SegmentControlStepper::disable()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (stepperList + i)->disable();
    }
}

float SegmentControlStepper::getAngle()
{
    return stepperList->getPosition();
}

bool SegmentControlStepper::getSensor()
{
    return digitalRead(zeroPort);
}

int SegmentControlStepper::getDesiredPos()
{
    return stepperList->getDesiredPos();
}

float SegmentControlStepper::getSpeed()
{
    return stepperList->getSpeed();
}
