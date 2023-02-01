/*
  btt_skr_1.1.c - driver code for STM32F407xx ARM processors

  Part of grblHAL

  Copyright (c) 2021-2022 Terje Io

  Some software serial code is ported from Arduino.  Credit belongs to the many
  authors that contributed to that project.

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

#include "driver.h"

#if defined(BOARD_MKS_ROBIN_NANO_3_1)

#if TRINAMIC_SPI_ENABLE

#include "trinamic/common.h"

static struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} cs[TMC_N_MOTORS_MAX];

// XXXXX replace with something better...
inline static void delay (void)
{
    volatile uint32_t dly = 10;

    while(--dly)
        __ASM volatile ("nop");
}

static uint8_t spi_get_byte (void)
{
    /*
    spi_port.Instance->DR = 0xFF; // Writing dummy data into Data register

    while(!__HAL_SPI_GET_FLAG(&spi_port, SPI_FLAG_RXNE));

    return (uint8_t)spi_port.Instance->DR;
    */
    return spi_put_byte(0xff);
}

static uint8_t spi_put_byte (uint8_t byte)
{
/*    spi_port.Instance->DR = byte;

    while(!__HAL_SPI_GET_FLAG(&spi_port, SPI_FLAG_TXE));
    while(!__HAL_SPI_GET_FLAG(&spi_port, SPI_FLAG_RXNE));

    __HAL_SPI_CLEAR_OVRFLAG(&spi_port);

    return (uint8_t)spi_port.Instance->DR;
    */
    uint8_t out=byte;
    uint8_t in=0;
    for(i=0;i<8;i++)
    {
        DIGITAL_OUT(sck.port,sck.pin,0);
        DIGITAL_OUT(sdo.port,sdo.pin,byte>>8);
        in=in<<1 + DIGITAL_IN(sdi.port,sdi.pin);
        DIGITAL_OUT(sck.port,sck.pin,1);
        byte=byte<<1;
    }
    return in;
}

TMC_spi_status_t tmc_spi_read (trinamic_motor_t driver, TMC_spi_datagram_t *datagram)
{
    TMC_spi_status_t status;

    DIGITAL_OUT(cs[driver.id].port, cs[driver.id].pin, 0);

    datagram->payload.value = 0;

    datagram->addr.write = 0;
    spi_put_byte(datagram->addr.value);
    spi_put_byte(0);
    spi_put_byte(0);
    spi_put_byte(0);
    spi_put_byte(0);

    DIGITAL_OUT(cs[driver.id].port, cs[driver.id].pin, 1);
    delay();
    DIGITAL_OUT(cs[driver.id].port, cs[driver.id].pin, 0);

    status = spi_put_byte(datagram->addr.value);
    datagram->payload.data[3] = spi_get_byte();
    datagram->payload.data[2] = spi_get_byte();
    datagram->payload.data[1] = spi_get_byte();
    datagram->payload.data[0] = spi_get_byte();

    DIGITAL_OUT(cs[driver.id].port, cs[driver.id].pin, 1);

    return status;
}

TMC_spi_status_t tmc_spi_write (trinamic_motor_t driver, TMC_spi_datagram_t *datagram)
{
    TMC_spi_status_t status;

    DIGITAL_OUT(cs[driver.id].port, cs[driver.id].pin, 0);

    datagram->addr.write = 1;
    status = spi_put_byte(datagram->addr.value);
    spi_put_byte(datagram->payload.data[3]);
    spi_put_byte(datagram->payload.data[2]);
    spi_put_byte(datagram->payload.data[1]);
    spi_put_byte(datagram->payload.data[0]);

    DIGITAL_OUT(cs[driver.id].port, cs[driver.id].pin, 1);

    return status;
}

static void add_cs_pin (xbar_t *gpio, void *data)
{
    if (gpio->group == PinGroup_MotorChipSelect)
      switch (gpio->function) {

        case Output_MotorChipSelectX:
            cs[X_AXIS].port = (GPIO_TypeDef *)gpio->port;
            cs[X_AXIS].pin = gpio->pin;
            break;

        case Output_MotorChipSelectY:
            cs[Y_AXIS].port = (GPIO_TypeDef *)gpio->port;
            cs[Y_AXIS].pin = gpio->pin;
            break;

        case Output_MotorChipSelectZ:
            cs[Z_AXIS].port = (GPIO_TypeDef *)gpio->port;
            cs[Z_AXIS].pin = gpio->pin;
            break;

        case Output_MotorChipSelectM3:
            cs[3].port = (GPIO_TypeDef *)gpio->port;
            cs[3].pin = gpio->pin;
            break;

        case Output_MotorChipSelectM4:
            cs[4].port = (GPIO_TypeDef *)gpio->port;
            cs[4].pin = gpio->pin;
            break;

        case Output_MotorChipSelectM5:
            cs[5].port = (GPIO_TypeDef *)gpio->port;
            cs[5].pin = gpio->pin;
            break;

        default:
            break;
    }
}

static void if_init (uint8_t motors, axes_signals_t enabled)
{
    static bool init_ok = false;

    UNUSED(motors);

    if (!init_ok) {

        static const periph_pin_t sck = {
            .function = Output_SCK,
            .group = PinGroup_SPI,
            .port = GPIOD,
            .pin = 0,
            .mode = { .mask = PINMODE_OUTPUT }
        };
        HAL_GPIO_Init(GPIOC, &sck);

        static const periph_pin_t sdo = {
            .function = Output_MOSI,
            .group = PinGroup_SPI,
            .port = GPIOD,
            .pin = 14,
            .mode = { .mask = PINMODE_OUTPUT }
        };
        HAL_GPIO_Init(GPIOC, &sdo);

        static const periph_pin_t sdi = {
            .function = Input_MISO,
            .group = PinGroup_SPI,
            .port = GPIOD,
            .pin = 1,
            .mode = { .mask = PINMODE_NONE }
        };
        HAL_GPIO_Init(GPIOC, &sdi);


        hal.periph_port.register_pin(&sck);
        hal.periph_port.register_pin(&sdo);
        hal.periph_port.register_pin(&sdi);
        hal.enumerate_pins(true, add_cs_pin, NULL);
    }
}

void board_init (void)
{
    static trinamic_driver_if_t driver_if = {.on_drivers_init = if_init};

    trinamic_if_init(&driver_if);
}

#endif // TRINAMIC_SPI_ENABLE

#if TRINAMIC_UART_ENABLE

void board_init (void)
{
    extern void tmc_uart_init (void);
    tmc_uart_init();
}

#endif // TRINAMIC_UART_ENABLE

#endif // BOARD_BTT_SKR_PRO_1_1
