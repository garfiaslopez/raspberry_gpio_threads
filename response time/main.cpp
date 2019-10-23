/*
 * p02_blink.cpp
 *
 *  Created on: Viernes 6 de septiembre, 2019
 *      Author: JOSE GARFIAS LOPEZ
 *
 *  Descripcion:
 */
#include <bcm2835.h>
// Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PINOUT RPI_BPLUS_GPIO_J8_11
#define PININ RPI_BPLUS_GPIO_J8_03
int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
    // Use for testing
//    bcm2835_set_debug(1);
    if (!bcm2835_init())
        return 1;
    // Set the pin to be an output
    bcm2835_gpio_fsel(PINOUT, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PININ, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PININ, BCM2835_GPIO_PUD_UP);
    // Blink
    while (1)
    {
        // Turn it on
        bcm2835_gpio_write(PINOUT, !bcm2835_gpio_lev(PININ));

    }
    bcm2835_close();
    return 0;
}
