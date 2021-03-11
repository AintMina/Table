# angles module has:
# polar_get_angles(alpha1, alpha2, r, theta, L1, L2)
# delta_angles(beta1, beta2, theta1, theta2)
# steps(theta1, theta2, spr)
#
# driver module has:
# Stop()
# TurnStep(Dir, steps)

import angles
from driver import drive
import math


coords = [0, 0]
L1, L2 = 140.5, 140.5
r_old = L1 + L2
arm_down = True
motor1 = drive(dir_pin=13, step_pin=19, enable_pin=12)
motor2 = drive(dir_pin=24, step_pin=18, enable_pin=4)
microsteps = 16

with open('sandify.thr') as f:

    for line in f:

        if '#' not in line:
            line = line.strip('\n')

            if line:
                line = line.split()

                # Multiplied by the sum of the arms
                r = (L1 + L2) * float(line[1])

                #if '.thr' in f.name:
                l = angles.polar_get_angles(float(line[0]), r_old, r, L1, L2, arm_down)

                if l is not None:
                    d = angles.delta_angles(coords[0], coords[1], l[0], l[1])
                    r_old = l[0]
                    coords = l

                    step = angles.steps(d[0], d[1], microsteps)
                    step[1] = step[1]/2

                    # Setting motor direction
                    if step[0] < 0:
                        dir1 = 'bw'
                        step[0] = -step[0]
                    else:
                        dir1 = 'fw'

                    if step[1] < 0:
                        dir2 = 'fw'
                        step[1] = -step[1]
                    else:
                        dir2 = 'bw'
                    print(step)

                    # Moves the motors step by step
                    if step[0] == step[1]:
                        for i in range(step[0]):
                            motor1.TurnStep(dir1, 1, stepdelay=0.001)
                            motor2.TurnStep(dir2, 1, stepdelay=0.001)
                    
                    if step[0] > step[1]:
                        

motor1.Stop()
motor2.Stop()