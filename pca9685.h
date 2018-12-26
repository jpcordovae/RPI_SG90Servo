#ifndef PCA9685_H
#define PCA9685_H

#include <linux/i2c-dev.h>
#include <stdio.h>

#define PCA9685_MODE1       0x00
#define PCA9685_MODE2       0x01
#define PCA9685_SUBADDR1    0x02
#define PCA9685_SUBADDR2    0x03
#define PCA9685_SUBADDR3    0x04
#define PCA9685_ALLCALLADDR 0x05

#define PCA9685_LEDn_ON_L_BASE  0x06
#define PCA9685_LEDn_ON_H_BASE  0x07
#define PCA9685_LEDn_OFF_L_BASE 0x08
#define PCA9685_LEDn_OFF_H_BASE 0x09

#define PCA9685_LEDn_ON_L(N)  (PCA9685_LEDn_ON_L_BASE + (4 * (N)))
#define PCA9685_LEDn_ON_H(N)  (PCA9685_LEDn_ON_H_BASE + (4 * (N)))
#define PCA9685_LEDn_OFF_L(N) (PCA9685_LEDn_OFF_L_BASE + (4 * (N)))
#define PCA9685_LEDn_OFF_H(N) (PCA9685_LEDn_OFF_H_BASE + (4 * (N)))

#define PCA9685_PRESCALE_MIN  0x03 // max frequency of 1526Hz
#define PCA9685_PRESCALE_MAX  0xFF // min frequency of 24Hz
#define PCA9685_COUNTER_RANGE 4096
#define PCA9685_DEFAUL_PERIOD 5000000 // default period_ns = 1/200 Hz
#define PCA9685_OSC_CLOCK_MHZ 25000000.0 // internal oscillator of 25MHz

#define PCA9685_ALL_LED_ON_L  0xFA
#define PCA9685_ALL_LED_ON_H  0xFB
#define PCA9685_ALL_LED_OFF_L 0xFC
#define PCA9685_ALL_LED_OFF_H 0xFD
#define PCA9685_PRE_SCALE     0xFE
#define PCA9685_TESTMODE      0xFF // DO NOT USE, Datasheet say may cause unpredictable results.

// MODE 1
#define MODE1_RESTART (1 << 7) // w = User writes logic 1 to this bit to clear it to logic 0, A user write of logic0 will have no efect.
#define MODE1_EXTCLK  (1 << 6) // 0 = use internal clock, 1 = use EXTCLK pin clock.s
#define MODE1_AI      (1 << 5) // auto increment
#define MODE1_SLEEP   (1 << 4) // 0 = normal mode, 1 = low power mode. Oscillator off.
#define MODE1_SUB1    (1 << 3) // 0 = PCA9685 does not respond to i2c-bus subaddress 1, 1 = PCA9685 respond to i2c-bus subaddress
#define MODE1_SUB2    (1 << 2) // 0 = PCA9685 does not respond to I2C-bus subaddress 2, 1 PCA9685 responds to I2C-bus subaddress 2.
#define MODE1_SUB3    (1 << 1) // 0 = PCA9685 does not respond to I2C-bus subaddress 3, 1 = PCA9685 responds to I2C-bus subaddress 3.
#define MODE1_ALLCALL 0x01 // 0 = PCA9685 does not respond to LED All Call I2C-bus address, 1 = PCA9685 responds to LED All Call I2C-bus address.

typedef union {
  struct {
    __u8 restart_bit;
    __u8 extclk_bit;
    __u8 ai_bit;
    __u8 sleep_bit;
    __u8 sub1_bit;
    __u8 sub2_bit;
    __u8 sub3_bit;
    __u8 allcall_bit;
  } fields;
  __u8 raw;
} MODE1_REG_DATA;

// MODE 2
#define MODE2_INVRT   (1<<4) // 0 = Output logic state not inverted, 1 =  output logic state inverted.
#define MODE2_OCH     (1<<3) // 0 = Outputs change on STOP command, 1 = Outputs change on ACK.
#define MODE2_OUTDRV  (1<<2) // 0 = The 16 LEDn outputs are configured with an open-drain structure, 1 = The 16 LEDn outputs are configured with a totem pole structure.
#define MODE2_OUTNE0  (1<<1) // When OE = 1 (output drivers not enabled), LEDn = high-impedance.
#define MODE2_OUTNE1  0 // red the datasheet (page 16, Rev. 4)... don't be laisy.

typedef union{
  struct {
    __u8 unused_bits:3;
    __u8 invrt_bit;
    __u8 och_bit;
    __u8 outdrv_bit;
    __u8 outne_bits:2;
  } fields;
  __u8 raw;
} MODE2_REG_DATA;

#endif // PCA9685_H
