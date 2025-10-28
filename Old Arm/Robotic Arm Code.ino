#include "ArmStepperControl.h"
#include "ArmMotorControl.h"
#include "SegmentControlStepper.h"
#include "SegmentControlMotor.h"
#include "FullArmControl.h"

#define FORE_ARM_MOTOR_NUM 1
#define UPPER_ARM_MOTOR_NUM 2
#define SHOULDER_YAW_MOTOR_NUM 1
#define SHOULDER_PITCH_MOTOR_NUM 1
#define MOVEMENT_TOLERANCE 0.25

char receivedCommand;
float receivedYaw = 0;
float receivedRadius = 0;
float receivedHeight = 0;
float receivedWristAngle = 0;

bool worked = false;

ArmStepperControl foreArmMotor(5, 22, 'L', 800, (float(16) / 60 * 16 / 60), 91, -90, 91);
ArmStepperControl shoulderYawMotor(6, 24, 'L', 800, float(1) / 3, 90, -90, 0);
ArmStepperControl rightUpperMotor(7, 26, 'R', 800, (float(16) / 60 * 16 / 100), 0, -135, -135);
ArmStepperControl leftUpperMotor(8, 28, 'L', 800, (float(16) / 60 * 16 / 100), 0, -135, -135);
ArmMotorControl shoulderPitchMotor(10, 9, 2, 3, (1.0 / 2), 90, -10, -10, 7, 0.1, 0.5);

ArmStepperControl foreArm[FORE_ARM_MOTOR_NUM] = {foreArmMotor};
ArmStepperControl upperArm[UPPER_ARM_MOTOR_NUM] = {leftUpperMotor, rightUpperMotor};
ArmStepperControl shoulderYaw[SHOULDER_YAW_MOTOR_NUM] = {shoulderYawMotor};
ArmMotorControl shoulderPitch[SHOULDER_PITCH_MOTOR_NUM] = {shoulderPitchMotor};

SegmentControlStepper wristPitchControl(FORE_ARM_MOTOR_NUM, foreArm, 52, 1, 0);
SegmentControlStepper foreArmPitchControl(UPPER_ARM_MOTOR_NUM, upperArm, 50, -1, 0);
SegmentControlStepper shoulderYawControl(SHOULDER_YAW_MOTOR_NUM, shoulderYaw, 46, 1, 0);
SegmentControlMotor shoulderPitchControl(SHOULDER_PITCH_MOTOR_NUM, shoulderPitch, 48, -1, 0);

FullArmControl arm(&wristPitchControl, &foreArmPitchControl, &shoulderPitchControl, &shoulderYawControl, float(16), float(16), float(5));

void setup()
{
    Serial.begin(115200);
    delay(1000);

    wristPitchControl.enable();
    foreArmPitchControl.enable();
    foreArmMotor.zero();
    rightUpperMotor.zero();
    leftUpperMotor.zero();
    shoulderPitchMotor.zero();
    shoulderYawMotor.zero();

    Serial.println("Zeroing");

    arm.zero();

    Serial.println("Setting Up Move 1");

    arm.setDesiredPosition(0, 25, 0, 0);
    Serial.println(arm.calculateMovement(70));

    printInfo();

    Serial.println("Moving");
    Serial.println();

    while (abs(arm.getHeightError()) > MOVEMENT_TOLERANCE || abs(arm.getRadiusError()) > MOVEMENT_TOLERANCE)
    {
        for (int i = 0; i < 100; i++)
        {
            arm.run();
        }
    }

    for (int i = 0; i < 5000; i++)
    {
        arm.run();
    }

    printEndInfo();

    Serial.println("Setting Up Move 2");

    arm.setDesiredPosition(0, 35, 0, 0);
    Serial.println(arm.calculateMovement(70));

    printInfo();

    Serial.println("Moving");
    Serial.println();

    while (abs(arm.getHeightError()) > MOVEMENT_TOLERANCE || abs(arm.getRadiusError()) > MOVEMENT_TOLERANCE)
    {
        for (int i = 0; i < 100; i++)
        {
            arm.run();
        }
    }

    for (int i = 0; i < 5000; i++)
    {
        arm.run();
    }

    printEndInfo();

    Serial.println("Setting Up Move 3");

    arm.setDesiredPosition(0, 25, 20, 0);
    Serial.println(arm.calculateMovement(70));

    printInfo();

    Serial.println("Moving");
    Serial.println();

    while (abs(arm.getHeightError()) > MOVEMENT_TOLERANCE || abs(arm.getRadiusError()) > MOVEMENT_TOLERANCE)
    {
        for (int i = 0; i < 100; i++)
        {
            arm.run();
        }
    }

    for (int i = 0; i < 5000; i++)
    {
        arm.run();
    }

    printEndInfo();

    Serial.println("Setting Up Move 3.5");

    arm.setDesiredPosition(0, 30, 15, 45);
    Serial.println(arm.calculateMovement(70));

    printInfo();

    Serial.println("Moving");
    Serial.println();

    while (abs(arm.getHeightError()) > MOVEMENT_TOLERANCE || abs(arm.getRadiusError()) > MOVEMENT_TOLERANCE)
    {
        for (int i = 0; i < 100; i++)
        {
            arm.run();
        }
    }

    for (int i = 0; i < 5000; i++)
    {
        arm.run();
    }

    printEndInfo();

    Serial.println("Setting Up Move 4");

    arm.setDesiredPosition(0, 30, 0, -90);
    Serial.println(arm.calculateMovement(70));

    printInfo();

    Serial.println("Moving");
    Serial.println();

    while (abs(arm.getHeightError()) > MOVEMENT_TOLERANCE || abs(arm.getRadiusError()) > MOVEMENT_TOLERANCE)
    {
        for (int i = 0; i < 100; i++)
        {
            arm.run();
        }
    }

    for (int i = 0; i < 7000; i++)
    {
        arm.run();
    }

    printEndInfo();

    Serial.println("Setting Up Move 5");

    arm.setDesiredPosition(0, 25, 0, 0);
    Serial.println(arm.calculateMovement(70));

    printInfo();

    Serial.println("Moving");
    Serial.println();

    while (abs(arm.getHeightError()) > MOVEMENT_TOLERANCE || abs(arm.getRadiusError()) > MOVEMENT_TOLERANCE)
    {
        for (int i = 0; i < 100; i++)
        {
            arm.run();
        }
    }

    for (int i = 0; i < 5000; i++)
    {
        arm.run();
    }

    printEndInfo();

    arm.stop();
}

void loop()
{
    /*
    if (Serial.available() > 0)
    {
        //arm.disable();
        arm.stop();
        receivedCommand = Serial.read();

        switch (receivedCommand)
        {
        case 'M': // Must be in format M Yaw Radius Height Wrist Angle
            receivedYaw = Serial.parseFloat();
            receivedRadius = Serial.parseFloat();
            receivedHeight = Serial.parseFloat();
            receivedWristAngle = Serial.parseFloat();

            setNewPosition();

        case 'S':
            arm.disable();

        case 'R':
            arm.enable();
        }
        //arm.enable();
    }
    //arm.stepperEnable();
    arm.run();
    */
}

void printInfo()
{
    Serial.print("Radius: ");
    Serial.println(arm.getDesiredRadius());
    Serial.print("Height: ");
    Serial.println(arm.getDesiredHeight());
    Serial.print("Wrist Pitch: ");
    Serial.println(arm.getWristPitch());
    Serial.print("Fore Arm Pitch: ");
    Serial.println(arm.getForeArmPich());
    Serial.print("Upper Arm Pitch: ");
    Serial.println(arm.getUpperArmPitch());
    Serial.print("Returnable Num: ");
    Serial.println(arm.getNum());
    Serial.print("Wrist Desired Position: ");
    Serial.println(arm.getWristDesiredPos());
    Serial.print("Fore Arm Desired Position: ");
    Serial.println(arm.getForeArmDesiredPos());
    Serial.print("Wrist Speed: ");
    Serial.println(arm.getWristSpeed());
    Serial.println();
}

void printEndInfo()
{
    Serial.print("Radius: ");
    Serial.println(arm.getCurrentRadius());
    Serial.print("Height: ");
    Serial.println(arm.getCurrentHeight());
    Serial.println();
}

void setNewPosition()
{
    arm.setDesiredPosition(0, 30, 0, 0);
    worked = arm.calculateMovement(60);

    if (worked == true)
    {
        Serial.println("Location Possible: Moving");

        printInfo();
    }
    else
    {
        Serial.println("Location Not Possible: Maintaining");
    }
}
