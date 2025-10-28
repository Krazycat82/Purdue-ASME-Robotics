#include "Arduino.h"
#include "ArmMotorControl.h"
#include "SegmentControlMotor.h"

#define MOVEMENT_TOLERANCE 0.05

SegmentControlMotor::SegmentControlMotor(int numSteppers, ArmMotorControl *motorList, int zeroPort, int zeroDirection, int passiveAngle)
{
    this->numSteppers = numSteppers;
    this->motorList = motorList;
    this->zeroPort = zeroPort;
    this->zeroDirection = zeroDirection;
    this->passiveAngle = passiveAngle;

    pinMode(zeroPort, INPUT_PULLUP);

    angle = 0;
}

SegmentControlMotor::~SegmentControlMotor()
{
    for (int i = 0; i < numSteppers; i++)
    {
        delete (motorList + i);
    }

    free(motorList);
}

void SegmentControlMotor::zero()
{
    // Zeroing Code
    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->setPosition(0);
    }

    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->setUpMoveOveride(360 * zeroDirection, 20);
    }

    while (digitalRead(zeroPort) == false)
    {
        runMove();
    }

    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->setUpMove(0, 60);
    }

    while (digitalRead(zeroPort) == true)
    {
        runMove();
    }

    stop();

    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->zero();
    }

    // Move to passive position code
    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->setUpMove(passiveAngle, 100);
    }

    while (abs(getAngle() - passiveAngle) > MOVEMENT_TOLERANCE)
    {
        runMove();
    }

    stop();
}

bool SegmentControlMotor::setUpMove(float angle, int speed)
{
    int i = 0;           // Counter variable for while loop
    bool capable = true; // Tracks if the system is capable of making the movement w/o moving out of its limitations

    while (i < numSteppers && capable == true)
    {
        capable = (motorList + i)->setUpMove(angle, speed);
        i++;
    }

    return capable;
}

void SegmentControlMotor::runMove()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->runMove();
    }
}

void SegmentControlMotor::stop()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->stop();
    }
}

void SegmentControlMotor::enable()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->enable();
    }
}

void SegmentControlMotor::disable()
{
    for (int i = 0; i < numSteppers; i++)
    {
        (motorList + i)->disable();
    }
}

float SegmentControlMotor::getAngle()
{
    return motorList->getPosition();
}

bool SegmentControlMotor::getSensor()
{
    return digitalRead(zeroPort);
}
