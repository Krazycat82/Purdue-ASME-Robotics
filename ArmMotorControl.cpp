#include "ArmMotorControl.h"
#include "Arduino.h"
#include <CytronMotorDriver.h>
#include <Encoder.h>
#include "PID.h"

#define DEGREES_PER_REVOLUTION 360
#define ENCODER_RESOLUTION_FORMULA ((1.0+(46.0/17) )* (1.0+(46.0/17)) * (1.0+(46.0/17) )* (1.0+(46.0/17)) * 28.0)

// Constructor and Deconstructor
ArmMotorControl::ArmMotorControl(int dirPin, int pwmPin, int chAPin, int chBPin, float gearRatio, int maxPos, int minPos, int zeroPos, float kP, float kI, float kD)
{
    this->gearRatio = gearRatio;
    this->maxPos = maxPos;
    this->minPos = minPos;
    this->zeroPos = zeroPos;

    motor = new CytronMD(PWM_DIR, pwmPin, dirPin);
    encoder = new Encoder(chAPin, chBPin);
    motorPID = new PID(kP, kI, kD);
}

ArmMotorControl::~ArmMotorControl()
{
    delete motor;
    delete encoder;
}

// Control Functions
void ArmMotorControl::zero()
{
    encoder->write(-1 * zeroPos / gearRatio * ENCODER_RESOLUTION_FORMULA / DEGREES_PER_REVOLUTION);
}

void ArmMotorControl::setPosition(float angle)
{
    encoder->write(-1 * angle / gearRatio * ENCODER_RESOLUTION_FORMULA / DEGREES_PER_REVOLUTION);
}

bool ArmMotorControl::setUpMove(float angle, int speed)
{
    bool capable = false;

    maxSpeed = speed;
    if (angle <= maxPos && angle >= minPos)
    {
        motorPID->changeSetPoint(angle);
        capable = true;
    }

    return capable;
}

void ArmMotorControl::setUpMoveOveride(float angle, int speed)
{
    maxSpeed = speed;
    motorPID->changeSetPoint(angle);
}

void ArmMotorControl::runMove()
{
    PIDValue = motorPID->calculate(getPosition());
    PIDValue = abs(PIDValue) - maxSpeed < 0 ? PIDValue : maxSpeed * (PIDValue / abs(PIDValue));
    motor->setSpeed(PIDValue);
}

void ArmMotorControl::enable()
{
    maxSpeed = prevMaxSpeed;
}

void ArmMotorControl::stop()
{
    motor->setSpeed(20);
}

void ArmMotorControl::disable()
{
    prevMaxSpeed = maxSpeed;
    maxSpeed = 0;
}

// Setters and Getters
float ArmMotorControl::getPosition()
{
    return (-1 * double(encoder->read()) * gearRatio / ENCODER_RESOLUTION_FORMULA * DEGREES_PER_REVOLUTION);
}

float ArmMotorControl::getSpeed()
{
    return maxSpeed;
}

float ArmMotorControl::getMaxPos()
{
    return maxPos;
}

double ArmMotorControl::getPIDValue()
{
    return PIDValue;
}