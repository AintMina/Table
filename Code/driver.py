
import RPi.GPIO as GPIO
import time


MotorDir = [
    'fw',
    'bw',
]

class drive():
    def __init__(self, dir_pin, step_pin, enable_pin):
        self.dir_pin = dir_pin
        self.step_pin = step_pin        
        self.enable_pin = enable_pin

        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(self.dir_pin, GPIO.OUT)
        GPIO.setup(self.step_pin, GPIO.OUT)
        GPIO.setup(self.enable_pin, GPIO.OUT)

    def digital_write(self, pin, value):
        GPIO.output(pin, value)

    def Stop(self):
        self.digital_write(self.enable_pin, 1)

    def TurnStep(self, Dir, steps, stepdelay=0.005):
        if (Dir == MotorDir[0]):
            self.digital_write(self.enable_pin, 0)
            self.digital_write(self.dir_pin, 0)
        elif (Dir == MotorDir[1]):
            self.digital_write(self.enable_pin, 0)
            self.digital_write(self.dir_pin, 1)
        else:
            self.digital_write(self.enable_pin, 1)
            return

        if (steps == 0):
            return
            
        for i in range(steps):
            self.digital_write(self.step_pin, True)
            time.sleep(stepdelay)
            self.digital_write(self.step_pin, False)
            time.sleep(stepdelay)