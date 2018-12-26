#include "pca9685.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int file;
int adapter_nr = 1;
char filename[20];

MODE1_REG_DATA m1rd;
MODE2_REG_DATA m2rd;

void setPWMFreq(int freq);
void pca9685_reset(void);
void pca9685_setPWM(__u8 led, __s32 on_value, __s32 off_value);
void setPWM(__u8 led, __u32 val);
void setAngle(__u8 led,float angle);

int main(int argc, char *argv[])
{
  snprintf(filename,12,"/dev/i2c-%d",adapter_nr);
  file = open(filename,O_RDWR);
  if(file<0)
  {
    exit(1);
  }
  
  int addr = 0x40;
  if(ioctl(file,I2C_SLAVE,addr))
    {
      exit(1);
    }
  
  __u8 reg = 0x10;
  __s32 resw;
  __u8 resb;

  pca9685_reset();
  
  m1rd.raw = i2c_smbus_read_byte_data(file,PCA9685_MODE1);

  printf("%#2x \n\r",m1rd.raw);
  printf("MODE1:\n\r");
  printf("-----\n\r");
  printf("restart_bit: %x\n\r", (m1rd.raw & MODE1_RESTART) >> 7);
  printf("extclk_bit:  %x\n\r", (m1rd.raw & MODE1_EXTCLK) >> 6);
  printf("ai_bit:      %x\n\r", (m1rd.raw & MODE1_AI) >> 5);
  printf("sleep_bit:   %x\n\r", (m1rd.raw & MODE1_SLEEP) >>4);
  printf("sub1_bit:    %x\n\r", (m1rd.raw & MODE1_SUB1) >> 3);
  printf("sub2_bit:    %x\n\r", (m1rd.raw & MODE1_SUB2) >> 2);
  printf("sub3_bit:    %x\n\r", (m1rd.raw & MODE1_SUB3) >> 1);
  printf("allcall_bit: %x\n\r", (m1rd.raw & MODE1_ALLCALL) );

  setPWMFreq(60);
  __u8 led = atoi(argv[1]);
  __u32 angle = atoi(argv[2]);
  printf("\n\r");
  printf("led: %u, angle: %u \n\r",led,angle);
  setAngle(led,angle);
}

void setAngle(__u8 led, float angle)
{
  float prop = angle / 180.0;
  __u32 dutyCycle = (565.0 - 172.0)*prop + 172.0;
  setPWM(led,dutyCycle);
}

void setPWM(__u8 led, __u32 val)
{
  pca9685_setPWM(led,0,val);
}

void pca9685_reset(void)
{
  i2c_smbus_write_byte_data(file,PCA9685_MODE1,0x00);
  i2c_smbus_write_byte_data(file,PCA9685_MODE2,0x04);
}

void setPWMFreq(int freq)
{
  __u8 prescale_val = (PCA9685_OSC_CLOCK_MHZ/4096/freq) - 1;
  i2c_smbus_write_byte_data(file,PCA9685_MODE1,0x10);
  i2c_smbus_write_byte_data(file,PCA9685_PRE_SCALE,prescale_val);
  i2c_smbus_write_byte_data(file,PCA9685_MODE1,0x80);
  i2c_smbus_write_byte_data(file,PCA9685_MODE2,0x04);
}

void pca9685_setPWM(__u8 led, __s32 on_value, __s32 off_value)
{
  i2c_smbus_write_byte_data(file, PCA9685_LEDn_ON_L(led), on_value & 0xFF);
  i2c_smbus_write_byte_data(file, PCA9685_LEDn_ON_H(led), on_value >> 8 );
  i2c_smbus_write_byte_data(file, PCA9685_LEDn_OFF_L(led), off_value & 0xFF);
  i2c_smbus_write_byte_data(file, PCA9685_LEDn_OFF_H(led), off_value >> 8);
}

