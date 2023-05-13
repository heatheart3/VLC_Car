# How to use Esplink to debug
# GPIO used for JTAG
- GPIO15: TD0
- GPIO12: TD1
- GPIO13: TCK
- GPIO14: TMS

# OpenOCD commands
openocd -f /interface/esp_usb_bridge.cfg -f /target/esp32.cfg


# PD GPIO
- GPIO31: Analong input

# Servo GPIO
GPIO_OUTPUT_PWM_SERVO 13

# Motor GPiO
#define GPIO_OUTPUT_AIN1    14 //AIN1
#define GPIO_OUTPUT_BIN1    12 //BIN1
#define GPIO_OUTPUT_AIN2    19 //AIN2
#define GPIO_OUTPUT_BIN2    17 //BIN2
#define GPIO_OUTPUT_PWMA   18 //PWMA
#define GPIO_OUTPUT_PWMB   16 //PWMB
