#include <stdio.h>
#include <wiringPi.h>

#define POLL_INTERVAL 		1000	// ms
#define HYSTERESIS		10	// # POLL_INTERVALs

#define PWM_PIN 		1	// WiringPi Pin

#define MIN_PWM			0.40	// %
#define MIN_TEMP		50.0	// deg C

#define MAX_PWM			1.00	// %
#define MAX_TEMP		60.0	// deg C

int hysteresis = 0;

float calc_fan_speed()
{
  float arm_temp;

  FILE* sensor = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  fscanf(sensor, "%f", &arm_temp);
  arm_temp /= 1000;
  fclose(sensor);

  //printf("arm temp: %f\n", arm_temp);

  if (arm_temp < MIN_TEMP)
  {
    return 0.0;
  }
  else if (arm_temp > MAX_TEMP)
  {
    return 1.0;
  }
  else
  {
    return MIN_PWM + ((arm_temp - MIN_TEMP) / (MAX_TEMP - MIN_TEMP)) * (MAX_PWM - MIN_PWM);
  }
}

int main()
{
  //printf("Starting rpisysfan\n");

  if (wiringPiSetup() == -1)
    return 1;

  pinMode(PWM_PIN, PWM_OUTPUT);

  for (;;)
  {
    float fan_speed = calc_fan_speed();

    if (fan_speed > 0)
    {
      hysteresis = HYSTERESIS;
    }
    else if (hysteresis > 0)
    {
      fan_speed = MIN_PWM;
      hysteresis--;
    }

    //printf("fan speed: %f\n", fan_speed);
    pwmWrite(PWM_PIN, (int)(fan_speed * 1023));
    delay(POLL_INTERVAL);
  }

  return 0;
}

