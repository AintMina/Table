/**
 * 
 Author: AintMina
 Date: 27.3.2021
 Version 0.1

 A driver for driving stepper motors for arduino with CNC-shield.

*/


class motor {
    public:
    char dir, step, enable;

    // Constructor for the pins
    motor(char dir_pin, char step_pin, char enable_pin) {
        dir = dir_pin;
        enable = enable_pin;
        step = step_pin;

        // Initiating the pins
        pinMode(dir, OUTPUT);
        pinMode(enable, OUTPUT);
        pinMode(step, OUTPUT);
    }

    // Disables the motor
    void Stop() {
        digitalWrite(enable, HIGH);
    }

    // Sets the direction pin and enables the motor
    void set_direction(int steps) {
        if (steps >= 0) {
            digitalWrite(enable, LOW);
            digitalWrite(dir, HIGH);
        }
        else if (steps < 0) {
            digitalWrite(enable, LOW);
            digitalWrite(dir, LOW);
        }
    }

    void Step(int steps, float stepdelay = 1) {
        for (int i = 0; i <= steps; i++) {
            digitalWrite(step, HIGH);
            delay(stepdelay);
            digitalWrite(step, LOW);
            delay(stepdelay);
            //delay(1);
        }
    }
};

// Function to divide the steps
// motor1 needs to have more steps!
void dual_steps(int step1, motor motor1, int step2, motor motor2) {
    int v = step1 / step2;      // Variable to figure out step ratio
    int counter = 0;
    int step2_counter = 0;

    for (int i = 0; i < step1; i++) {
        motor1.Step(1);
        counter++;
        if ((counter = v) && (step2_counter < step2)) {
            motor2.Step(1);
            step2_counter++;
            counter = 0;
        }
    }

    // Checking if step2 did all the steps
    while (step2_counter < step2) {
        motor2.Step(1);
        step2_counter++;
    }
}

void equal_steps(int step, motor motor1, motor motor2) {
    for (int i = 0; i < step; i++) {
        motor1.Step(1);
        motor2.Step(1);
    }
}