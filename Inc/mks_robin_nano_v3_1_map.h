/*
  skr_pro_v1_1_map.h - driver code for STM32F407 ARM processors

  Part of grblHAL

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#if N_ABC_MOTORS > 2 || N_AUTO_SQUARED > 1
#error "Axis configuration is not supported!"
#endif

#if !defined(STM32F407xx) || HSE_VALUE != 8000000
#error "This board has STM32F407 processor with a 8MHz crystal, select a corresponding build!"
#endif


#define BOARD_NAME "MKS Robin Nano V3.1"
#define BOARD_URL "https://github.com/makerbase-mks/MKS-Robin-Nano-V3.X"


#if USB_SERIAL_CDC == 0
#define SERIAL_MOD 3
#endif

#define I2C_PORT 1
#define I2C1_ALT_PINMAP // GPIOB, SCL_PIN = 6, SDA_PIN = 7

#undef EEPROM_ENABLE
#define EEPROM_ENABLE 3

#if TRINAMIC_ENABLE
#define HAS_BOARD_INIT
#endif

// Define step pulse output pins.
#define X_STEP_PORT                 GPIOE
#define X_STEP_PIN                  3                   // X
#define Y_STEP_PORT                 GPIOE
#define Y_STEP_PIN                  0                  // Y
#define Z_STEP_PORT                 GPIOB
#define Z_STEP_PIN                  5                  // Z
#define STEP_OUTMODE                GPIO_BITBAND
//#define STEP_PINMODE                PINMODE_OD // Uncomment for open drain outputs

// Define step direction output pins.
#define X_DIRECTION_PORT            GPIOE
#define X_DIRECTION_PIN             2
#define Y_DIRECTION_PORT            GPIOB
#define Y_DIRECTION_PIN             9
#define Z_DIRECTION_PORT            GPIOB
#define Z_DIRECTION_PIN             4
#define DIRECTION_OUTMODE           GPIO_BITBAND
//#define DIRECTION_PINMODE           PINMODE_OD // Uncomment for open drain outputs

// Define stepper driver enable/disable output pin.
#define X_ENABLE_PORT               GPIOE
#define X_ENABLE_PIN                4
#define Y_ENABLE_PORT               GPIOD
#define Y_ENABLE_PIN                7
#define Z_ENABLE_PORT               GPIOD
#define Z_ENABLE_PIN                4
//#define STEPPERS_ENABLE_PINMODE   PINMODE_OD // Uncomment for open drain outputs

// Define homing/hard limit switch input pins.
#define X_LIMIT_PORT                GPIOA
#define X_LIMIT_PIN                 15                          // X- Limit
#define Y_LIMIT_PORT                GPIOD
#define Y_LIMIT_PIN                 2                           // Y- Limit
#define Z_LIMIT_PORT                GPIOC
#define Z_LIMIT_PIN                 8                           // Z- Limit
#define LIMIT_INMODE                GPIO_BITBAND

// Define ganged axis or A axis step pulse and step direction output pins.
#if N_ABC_MOTORS > 0
#define M3_AVAILABLE                // E0
#define M3_STEP_PORT                GPIOD
#define M3_STEP_PIN                 6
#define M3_DIRECTION_PORT           GPIOD
#define M3_DIRECTION_PIN            3
#define M3_LIMIT_PORT               GPIOC
#define M3_LIMIT_PIN                4
#define M3_ENABLE_PORT              GPIOB
#define M3_ENABLE_PIN               3
#endif

// Define ganged axis or B axis step pulse and step direction output pins.
#if N_ABC_MOTORS > 1
#define M4_AVAILABLE                // E1
#define M4_STEP_PORT                GPIOD
#define M4_STEP_PIN                 15
#define M4_DIRECTION_PORT           GPIOA
#define M4_DIRECTION_PIN            1
#define M4_LIMIT_PORT               GPIOE // Conflict with X limit switch IRQ
#define M4_LIMIT_PIN                7
#define M4_ENABLE_PORT              GPIOA
#define M4_ENABLE_PIN               3
#endif

// Define ganged axis or C axis step pulse and step direction output pins.
#if N_ABC_MOTORS == 3
#error "This board does not support 6 motors"
#endif

  // Define spindle enable and spindle direction output pins.
#define SPINDLE_ENABLE_PORT         GPIOC
#define SPINDLE_ENABLE_PIN          14                          // FAN1
#define SPINDLE_DIRECTION_PORT      GPIOB
#define SPINDLE_DIRECTION_PIN       1                           // FAN2

// Define spindle PWM output pin.
#define SPINDLE_PWM_PORT_BASE       GPIOA_BASE
#define SPINDLE_PWM_PIN             0                           // H-BED - PA0

// Define flood and mist coolant enable output pins.
#define COOLANT_FLOOD_PORT          GPIOE
#define COOLANT_FLOOD_PIN           5                           // HEAT0
#define COOLANT_MIST_PORT           GPIOB
#define COOLANT_MIST_PIN            0                           // HEAT1

// Define user-control controls (cycle start, reset, feed hold) input pins.

#define RESET_PORT                  GPIOA
#define RESET_PIN                   13                           // PW_DET
#define FEED_HOLD_PORT              GPIOE
#define FEED_HOLD_PIN               6                           // MT_DET2
#define CYCLE_START_PORT            GPIOA
#define CYCLE_START_PIN             4                           // MT_DET1

#if SAFETY_DOOR_ENABLE
#define SAFETY_DOOR_PORT            GPIOB
#define SAFETY_DOOR_PIN             2                           // PW_OFF
#endif
#define CONTROL_INMODE GPIO_BITBAND

// Define probe switch input pin.
#define PROBE_PORT                  GPIOA
#define PROBE_PIN                   8                           // BLTouch PA8

#define SDCARD_ENABLE 1
#if SDCARD_ENABLE
#define SD_CS_PORT                  GPIOC
#define SD_CS_PIN                   9
#define SPI_PORT                    3 // GPIOC, SCK_PIN = 10, MISO_PIN = 11, MOSI_PIN = 12
#endif

#if TRINAMIC_UART_ENABLE

#undef TRINAMIC_UART_ENABLE
#define TRINAMIC_UART_ENABLE        2

#define MOTOR_UARTX_PORT            GPIOD
#define MOTOR_UARTX_PIN             5
#define MOTOR_UARTY_PORT            GPIOD
#define MOTOR_UARTY_PIN             7
#define MOTOR_UARTZ_PORT            GPIOD
#define MOTOR_UARTZ_PIN             4

#ifdef  M3_AVAILABLE
#define MOTOR_UARTM3_PORT           GPIOD
#define MOTOR_UARTM3_PIN            9
#endif

#ifdef  M4_AVAILABLE
#define MOTOR_UARTM4_PORT           GPIOD
#define MOTOR_UARTM4_PIN            8
#endif

#endif

#if TRINAMIC_SPI_ENABLE

// SPI1 is used: GPIOC pin 0, 1 and 14

#define MOTOR_CSX_PORT              GPIOD
#define MOTOR_CSX_PIN               5
#define MOTOR_CSY_PORT              GPIOD
#define MOTOR_CSY_PIN               7
#define MOTOR_CSZ_PORT              GPIOD
#define MOTOR_CSZ_PIN               4

#ifdef  M3_AVAILABLE
#define MOTOR_CSM3_PORT             GPIOD
#define MOTOR_CSM3_PIN              9
#endif

#ifdef  M4_AVAILABLE
#define MOTOR_CSM4_PORT             GPIOD
#define MOTOR_CSM4_PIN              8
#endif

#endif

//Wifi interface
#define MPG_ENABLE                  1
#define MPG_MODE_PORT               GPIOE
#define MPG_MODE_PIN                9                   //WIFI_CTRL
#define MPG_STREAM          1
