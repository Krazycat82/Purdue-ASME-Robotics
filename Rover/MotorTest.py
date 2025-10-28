import board
from adafruit_pca9685 import PCA9685

class MotorTest:
    def __init__(self, channel):
        # Initialize I2C bus and PCA9685 module.
        i2c = board.I2C()
        self.pca = PCA9685(i2c)
        self.pca.frequency = 200 # Set frequency to 200Hz for motor control.
        self.channel = channel
        self.period = 1000000 / self.pca.frequency # Period in microseconds.

    def set_motor_speed(self, speed):
        """
        Set the speed of the motor on the specified channel.
        :param speed: The speed value (0-100) to set for the motor.
        """
        if 0 <= self.channel <= 15:
            if -100 <= speed <= 100:
                pulse_width = 1500 + ((speed / 100) * 500) # Map speed to pulse width (1000-2000us)
                duty = pulse_width / self.period * 0xFFFF
                self.pca.channels[self.channel].duty_cycle = duty
            else:
                raise ValueError("Speed must be between -100 and 100.")
        else:
            raise ValueError("Channel must be between 0 and 15.")

    def stop_motor(self):
        """
        Stop the motor on the specified channel.
        """
        self.set_motor_speed(0)

# Testing:

motor = MotorTest(channel=0)
running = True

while running:
    try:
        speed = int(input("Input motor speed (-100 to 100), or Ctrl+C to stop: "))
        motor.set_motor_speed(speed)

    except ValueError:
        print("Invalid input. Please enter a number between -100 and 100.")
        
    except KeyboardInterrupt:
        motor.stop_motor()
        print("Motor stopped due to keyboard interrupt.")
        running = False