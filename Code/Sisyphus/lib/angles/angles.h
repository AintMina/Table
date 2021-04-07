/**
 * 
 * Author: AintMina
 * Date: 25.3.2021
 * Version 0.1
 * 
 * Library to get angles for 2-joint robotic arm.
 * 
 */

#include <math.h>


// Get angles for arm 2 from polar coordinates
double polar_get_theta2(double theta, double r, float L1, float L2, bool arm_down) {

    // Declaring variable
    double theta2;

    // Math (had to break it up to multiple variables)
    double s1 = pow(r, 2)-pow(L1, 2)-pow(L2, 2);
    unsigned int s2 = 2 * L1 * L2;
    double q2 = acos(s1 / s2);

    // Checking which way the second arm is bent
    if (arm_down) {
        theta2 = q2;
    }
    else if (!arm_down) {
        theta2 = -q2;
    }
    else {
        Serial.println("Error in theta2");
    }

    if (theta2 != theta2) {
        return 0.0;
    }

    return theta2;
}

// Get angles for arm 1 from polar coordinates
double polar_get_theta1(float theta2, double theta, double r, float L1, float L2, bool arm_down) {

    // Checking if denominator is zero
    double s2 = L1 + L2 * cos(theta2);
    if (s2 == 0.0) {
        s2 = 0.000001;
    }

    // Math
    double s1 = L2 * sin(theta2);
    double alpha = atan(s1 / s2);
    double q0 = theta - alpha;
    double q1 = theta + alpha;

    // Declaring variables
    double theta1;

    // Getting X and Y coordinates
    double x = r * cos(theta);
    double y = r * sin(theta);

    /*Serial.print(x,5);
          Serial.print("\t\t\t");
          Serial.print(y,5);
          Serial.print("\t\t\t");
          Serial.print(q0,5);*/


    // If arm bends down
    if (arm_down) {
        // Checking which sector the coordinates are in
        if (x >= 0 && y >= 0 && q0 >= 0) {
            theta1 = q0;
        }
        else if (x < 0) {
            theta1 = q0 ;
        }
        else if (x >= 0 && y < 0) {
            theta1 = q0;
        }
        else if (x >= 0 && y >= 0 && q0 < 0) {
            theta1 = q0;
        }
        else {
            Serial.println("Error in theta1 first IF");
        }
    }
    // If arm bends up
    if (!arm_down) {
        // Checking which sector the coordinates are in
        if (x >= 0 && y >= 0 && q1 >= 0) {
            theta1 = q1;
        }
        else if (x < 0) {
            theta1 = q1 + (M_PI / 2);
        }
        else if (x >= 0 && y < 0) {
            theta1 = q1 + M_PI;
        }
        else if (x >= 0 && y >= 0 && q1 < 0) {
            theta1 = q1 + M_PI;
        }
        else {
            Serial.println("Error in theta1 second IF");
        }
    }

    //Serial.print("\t\t\t");
          //Serial.println(theta1,5);

    return theta1;
}

// Checking if coordinates go through origin
bool arm_down_check(double r, double r_old, bool arm_down) {

    if ((r < 0 && r_old >= 0) || (r >= 0 && r_old < 0)) {
        return !(arm_down);
    }
    else {
        return arm_down;
    }
}

// Calculating change of angles
float delta_angles(double theta1, double theta_old) {

    double delta_theta = theta1 - theta_old;

    if (delta_theta >= (M_PI)) {
        delta_theta = delta_theta - (M_PI);
    }
    else if (delta_theta <= -(M_PI)) {
        delta_theta = delta_theta + (M_PI);
    }

    return delta_theta;
}

// Calculating steps from delta angles
int steps(double theta, int microstepping) {

    if (theta == 0.0000) {
        return 0;
    }

    unsigned int steps_per_revolution = 600 * 4 * microstepping;
    double s1 = (steps_per_revolution / (2 * M_PI));
    int step = round(s1 * theta);

    return step;
}

double theta_from_steps(int step, int microstepping, double origina_theta, double theta_old) {

    unsigned int steps_per_revolution = 600 * 4 * microstepping;
    double theta = step / (steps_per_revolution / (2 * M_PI));

    /*while ((int(theta + theta_old) != int(origina_theta)) && (int(theta + theta_old) < int(origina_theta))) {
        theta = theta + (M_PI/2);
    }
    while ((int(theta + theta_old) != int(origina_theta)) && (int(theta + theta_old) > int(origina_theta))) {
        theta = theta - (M_PI/2);
    }*/

    return theta;
}

double r_from_steps(float L1, float L2, double q1, double q2) {

    double x = L1 * cos(q1) + L2 * cos(q1 + q2);
    double y = L1 * sin(q1) + L2 * sin(q1 + q2);
    //double x_sqr = sq(x);
    //double y_sqr = sq(y);
    double r = sqrt(sq(x) + sq(y));

    return r;
}
