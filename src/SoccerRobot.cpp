#include "mbed.h"
#include "Motor.h"
#include "PS3_SBDBT.h"
#include "Utility.h"
#include "Brain_Board_ver0.h"

// シリアル通信
Serial pc(USBTX, USBRX, 921600);

// モーター
Motor m1((PinName)OUT0_DIR, (PinName)OUT0_PWM);
Motor m2((PinName)OUT1_DIR, (PinName)OUT1_PWM);
Motor m3((PinName)OUT2_DIR, (PinName)OUT2_PWM);

// コントローラー
PS3_SBDBT ps3((PinName)PS3_TX, (PinName)PS3_RX);

void core();
void motor(double, double, double, double);

int main() {
  pc.printf("System starts!\r\n");
  pc.printf("========================\r\n");

  while (1) {
    core();
    wait_ms(1);
  }
}

void core() {
  static double vv, vd, vz, crz;
  static double pvv, pvd, pvz, pcrz;

  ps3.task();

  vv = ps3.lr;
  vd = ps3.lth;
  vz = ps3.z * 0.5;
  // crz = gyrosensor.getAngleZ();
  crz = 0;

  if (vv != pvv || vd != pvd || vz != pvz) {
    motor(vv, vd, vz, deg_to_rad(crz));
  }

  pvv = vv;
  pvd = vd;
  pvz = vz;
  pcrz = crz;
  
  pc.printf("vv = %9.6lf, vd = %9.6lf, vz = %9.6lf\r\n", vv, vd, vz);
}

void motor(double vv, double vd, double vz, double crz) {
  static double _v1, _v2, _v3;

  _v1 = vv * (cos(-vd + crz + deg_to_rad(240))) + vz;
  _v2 = vv * (cos(-vd + crz + deg_to_rad(120))) + vz;
  _v3 = vv * (cos(-vd + crz + deg_to_rad(  0))) + vz;

  m1.setSpeed(_v1);
  m2.setSpeed(_v2);
  m3.setSpeed(_v3);
}
