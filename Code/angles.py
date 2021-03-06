
import math


# Getting the angles from polar
def polar_get_angles(theta, r_old, r, L1, L2, arm_down):

    # Checking if denominator is zero
    if (L1+L2*math.cos(math.acos((math.pow(r, 2)-math.pow(L1, 2)-math.pow(L2, 2))/(2*L1*L2)))) == 0:
        return

    # Math
    alpha = math.atan((L2*math.sin(math.acos((math.pow(r, 2)-math.pow(L1, 2)-math.pow(L2, 2))/(2*L1*L2)))) / (L1+L2*math.cos(math.acos((math.pow(r, 2)-math.pow(L1, 2)-math.pow(L2, 2))/(2*L1*L2)))))
    q0 = theta - alpha
    q1 = theta + alpha
    q2 = math.acos((math.pow(r, 2)-math.pow(L1, 2)-math.pow(L2, 2))/(2*L1*L2))

    # Change arm position calculation when going through origin
    if r < 0 and r_old >= 0 or r >= 0 and r_old < 0:
        arm_down = not arm_down

    # Getting X and Y coordinates from polar
    x = r * math.cos(theta)
    y = r * math.sin(theta)

    # If arm bends up
    if arm_down:
        # Checking which sector the coordinates are in
        if x >= 0 and y >= 0 and q0 >= 0:
            theta1 = q0
        elif x < 0:
            theta1 = q0 + math.pi
        elif x >= 0 and y < 0:
            theta1 = q0 + (2*math.pi)
        elif x >= 0 and y >= 0 and q0 < 0:
            theta1 = q0 + (2*math.pi)
        else:
            print("Error in theta1 first IF")
    # If arm bends down
    elif not arm_down:
        # Checking which sector the coordinates are in
        if x >= 0 and y >= 0 and q1 >= 0:
            theta1 = q1
        elif x < 0:
            theta1 = q1 + math.pi
        elif x >= 0 and y < 0:
            theta1 = q1 + (2*math.pi)
        elif x >= 0 and y >= 0 and q1 < 0:
            theta1 = q1 + (2*math.pi)
        else:
            print("Error in theta1 second IF")

    # Checking which way the second arm is bent
    if arm_down:
        theta2 = q2
    elif not arm_down:
        theta2 = (2 * math.pi) - q2
    else:
        print("Error in theta2")

    # Returning angles as list
    return [theta1, theta2, x, y]

# Getting the change of angles
def delta_angles(beta1, beta2, theta1, theta2):

    dtheta1 = theta1 - beta1
    dtheta2 = theta2 - beta2

    if dtheta1 >= math.pi:
        dtheta1 = dtheta1 - 2*math.pi
    elif dtheta1 <= -math.pi:
        dtheta1 = dtheta1 + 2*math.pi

    if dtheta2 >= math.pi:
        dtheta2 = dtheta2 - 2*math.pi
    elif dtheta2 <= -math.pi:
        dtheta2 = dtheta2 + 2*math.pi

    return dtheta1, dtheta2

# Calculating steps from angles
def steps(theta1, theta2, microstepping):

    spr = 200 * 3 * microstepping
    step1 = (spr/(2*math.pi)) * theta1
    step2 = (spr/(2*math.pi)) * theta2

    return [int(step1), int(step2)]