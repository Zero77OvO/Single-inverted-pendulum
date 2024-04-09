 #ifndef _PID_H
 #define _PID_H

 #include "main.h"

int vertical_PID_value(float measure, float calcu);
 int velocity_PID_value(int velocity);
 int location_PID_value(float measure, float calcu);
 float banlance_pid(void);
 void pid_speed_cal(void);
 void pid_angle_cal(void);
 void pid_set_tarspeed(uint32_t tar);
 #endif
