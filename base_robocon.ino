#include "cytron_robocon.h"
#include "encoder_robocon.h"
#include "mpu6050_robocon.h"
#include "ps3_robocon.h"
#include "pid_robocon.h"


void setup() {

  Serial.begin(9600);
  mpu_setup();
  encoder_1_init();
  encoder_2_init();
  ps3_setup();

}

void loop() 
{
  mpu_loop();
  ps3_loop();

}
