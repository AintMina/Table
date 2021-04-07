/**
 * 
 * Author: AintMina
 * Date: 18.3.2021
 * Version 0.1
 * 
 * Sketch to drive 2-joint robotic arm.
 * 
 */

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "angles.h"
#include "driver.h"

const int chipSelect = 53;
float L1 = 140.3, L2 = 140.3;
double r_old = double(L1 + L2);
bool arm_down = true;
double theta1_old = 0, theta2_old = 0; 
int microsteps = 1;
motor motor1(5, 2, 8);
motor motor2(6, 3, 8);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  } 

  Serial.print("Initializing SD card...");
  delay(5000);

  // If card not found
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while(1);
  }

  Serial.println("Card initialized.");

  // Opening the file
  File File = SD.open("filler.thr");

  // If file exists writes the line
  if (File) {
    while (File.available()){

      String line_temp = File.readStringUntil('\n');
      char line[line_temp.length()];
      line_temp.toCharArray(line, sizeof(line));

      // If line is not commented
      if (!(strstr(line, "#"))) {
        if (strlen(line) > 2) {
          char* angle = strtok(line, " ");      // Splitting the string
          char* r = strtok(0, " ");         // Splitting the string
          double angle_float = atof(angle);      // Transforms from char to float
          double r_float = (atof(r) * (L1 + L2));          // Transforms from char to float

          // Getting new angles for the arms
          double theta2 = polar_get_theta2(angle_float, r_float, L1, L2, arm_down);
          double theta1 = polar_get_theta1(abs(theta2), angle_float, r_float, L1, L2, arm_down);

          // Checking if the arm bends over itself
          bool arm_down = arm_down_check(r_float, r_old, arm_down);
          
          double delta_theta1 = delta_angles(theta1, theta1_old);     // Getting change of arm angle in radians
          double delta_theta2 = delta_angles(theta2, theta2_old);     // Getting change of arm angle in radians
          
          int step1 = steps(delta_theta1, microsteps);      // Getting steps from delta angles
          int step2 = steps(delta_theta2, microsteps);      // Getting steps from delta angles
          step2 = step2 + step1;

          // Setting motor direction
          motor1.set_direction(step1);
          motor2.set_direction(-step2);

          // Moving the actual motors
          if ((abs(step1) > abs(step2)) && (step2 != 0)) {
            dual_steps(abs(step1), motor1, abs(step2), motor2);
          }
          else if ((abs(step1) < abs(step2)) && (step1 != 0)) {
            dual_steps(abs(step2), motor2, abs(step1), motor1);
          }
          else if (abs(step1) == abs(step2)) {
            equal_steps(abs(step1), motor2, motor1);
          }
          else if (step2 == 0) {
            motor1.Step(step1);
          }
          else if (step1 == 0) {
            motor2.Step(step2);
          }
          else {}

          theta1_old = theta1_old + theta_from_steps(step1, microsteps, theta1, theta1_old);      // Saves q1 from steps
          theta2_old = theta2_old + theta_from_steps(step2 - step1, microsteps, theta2, theta2_old);      // Saves q2 from steps
          r_old = r_from_steps(L1, L2, theta1_old, theta2_old);      // Saves r from steps

          /*Serial.print(angle_float);
          Serial.print("\t\t\t");
          Serial.print(step1);
          Serial.print("\t\t\t");
          Serial.print(r_float);
          Serial.print("\t\t\t");
          Serial.println(step2);

          Serial.print(theta1);
          Serial.print("\t\t\t");
          Serial.print(delta_theta1);
          Serial.print("\t\t\t");
          Serial.print(theta2);
          Serial.print("\t\t\t");
          Serial.println(delta_theta2);*/
        }
      }
    }
    motor1.Stop();
    motor2.Stop();
    File.close();     // Closes the file
  }
  else {
    Serial.println("error onpening the file");
    motor1.set_direction(25);
    motor2.set_direction(-25);
    motor1.Step(600*8);
    delay(100);
    motor2.Step((600)*8);
    delay(100);
    motor1.Stop();
    motor2.Stop();
  }
}

void loop() {

}
