/*
 *  Created on: martes 23 octubre, 2019
 *      Author: JOSE GARFIAS LOPEZ
 *
 *  Descripcion:
 */

#include <pthread.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <iostream>

#define INPUTS_COUNT 2
#define OUTPUTS_COUNT 20
#define OUTPUTS_PER_INPUT 10

uint8_t INPUTS[] = {
    RPI_BPLUS_GPIO_J8_03,
    RPI_BPLUS_GPIO_J8_05
};

uint8_t OUTPUTS[] = {
	RPI_BPLUS_GPIO_J8_07,
	RPI_BPLUS_GPIO_J8_11,
    RPI_BPLUS_GPIO_J8_13,
	RPI_BPLUS_GPIO_J8_15,
	RPI_BPLUS_GPIO_J8_21,
	RPI_BPLUS_GPIO_J8_29,
    RPI_BPLUS_GPIO_J8_31,
	RPI_BPLUS_GPIO_J8_33,
    RPI_BPLUS_GPIO_J8_35,
    RPI_BPLUS_GPIO_J8_37,

    RPI_BPLUS_GPIO_J8_12,
	RPI_BPLUS_GPIO_J8_16,
    RPI_BPLUS_GPIO_J8_18,
	RPI_BPLUS_GPIO_J8_22,
    RPI_BPLUS_GPIO_J8_24,
	RPI_BPLUS_GPIO_J8_26,
    RPI_BPLUS_GPIO_J8_32,
	RPI_BPLUS_GPIO_J8_36,
    RPI_BPLUS_GPIO_J8_38,
	RPI_BPLUS_GPIO_J8_40
};

using namespace std;

uint8_t threads_created;
pthread_mutex_t mutex_flag = PTHREAD_MUTEX_INITIALIZER;

void *pushed_button(void *button_id_ref);

int main(int argc, char **argv) {
    pthread_t threads[INPUTS_COUNT];
    int rc;			// code return by posix-thread function
    long t;			// thread id and variable for repetition control
    void *status; 	// contain value returned by a thread
    threads_created = 0x00;

    if (!bcm2835_init()) {
        return 1;
    }

    for (int i=0; i<INPUTS_COUNT; i++) {
        bcm2835_gpio_fsel(INPUTS[i], BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(INPUTS[i], BCM2835_GPIO_PUD_UP);
    }

    for (int i=0; i<OUTPUTS_COUNT; i++) {
        bcm2835_gpio_fsel(OUTPUTS[i], BCM2835_GPIO_FSEL_OUTP);
    }

    while(1) {
        if (threads_created <= 0) {
            for (int i=0; i<INPUTS_COUNT; i++) {
                t = (long)i;
                rc = pthread_create(&threads[i], NULL, pushed_button, (void *)t);
                if (rc) {
                    return 0;
                }
                threads_created++;
            }
        }
    }
}

void *pushed_button(void *button_id_ref) {
    long button_id = (long)button_id_ref;
    int start_output_index = button_id * OUTPUTS_PER_INPUT;
    int end_output_index = start_output_index + OUTPUTS_PER_INPUT;
    int turned_on_output_index = 0;
    int timer = 0;

    while(bcm2835_gpio_lev(INPUTS[button_id]));
    pthread_mutex_lock(&mutex_flag);
    while (timer <= 100) {
        if(!bcm2835_gpio_lev(INPUTS[button_id])) {
            int output_to_turn_on = start_output_index + turned_on_output_index;
            bcm2835_gpio_write(OUTPUTS[output_to_turn_on], 1);
            turned_on_output_index += 1;
            if (turned_on_output_index >= OUTPUTS_PER_INPUT) {
                turned_on_output_index = 0;
                for (int i=start_output_index; i<end_output_index; i++) {
                    bcm2835_gpio_write(OUTPUTS[i],0);
                }
            }
        }
        bcm2835_delay(100);
        timer += 1;
    }
    pthread_mutex_unlock(&mutex_flag);
    threads_created -= 1;
    pthread_exit(NULL);
}
