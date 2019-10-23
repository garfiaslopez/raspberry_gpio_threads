/*
 *
 *  Created on: Viernes 6 de septiembre, 2019
 *      Author: JOSE GARFIAS LOPEZ
 *
 *  Descripcion:
 */
//Uso de union de hilos para activar un led por 5 seg despues de activar 3 
//veces dos botones monitoreados por un hilo cada uno

//Asignacion de pines

/*
Entradas: 3,5
Salida: 7
*/

#include <pthread.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <iostream>
#define N 2 //Define el numero de hilos hijo	

using namespace std;

// Arreglo de puertos de entrada 
uint8_t entrada[] = {
		RPI_BPLUS_GPIO_J8_03,
		RPI_BPLUS_GPIO_J8_05
};

uint8_t salida= RPI_BPLUS_GPIO_J8_07; //pin de salida

void *leer(void *t);

int main(int argc, char **argv) {

   pthread_t threads[N];
   int rc;			// code return by posix-thread function
   long t;			// thread id and variable for repetition control
   //void *status; 	// contain value returned by a thread

	// If you call this, it will not actually access the GPIO
	// Use for testing
	// bcm2835_set_debug(1);
	if (!bcm2835_init())
		return 1;

    bcm2835_gpio_fsel(entrada[0], BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(entrada[1], BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(entrada[0], BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_set_pud(entrada[1], BCM2835_GPIO_PUD_UP);

    bcm2835_gpio_fsel(salida, BCM2835_GPIO_FSEL_OUTP);

    while(1)
    {
        //Creaciòn de hilos 
        rc = pthread_create(&threads[0], NULL, leer, (void *)0);
        rc = pthread_create(&threads[1], NULL, leer, (void *)1);
        cout<<"Esperando la activaciòn de los botones"<<endl;
        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        // Se espera la terminación de los hilos hijo para continuar
        cout<<"Se activaron los botones"<<endl;
        bcm2835_gpio_write(salida, HIGH);
        bcm2835_delay(5000);
        bcm2835_gpio_write(salida, LOW);
        
    }//Fin del while
}//Fin del main

void *leer(void *t)
{
    long t_local=(long)t;
    int contador=0;
    for(contador=0; contador<3; contador++) {
        // Espera hasta que el botón sea activado (lectura de un estado bajo) 
        while(bcm2835_gpio_lev(entrada[t_local]));
        bcm2835_delay(500);
    }//Fin del ciclo for

    pthread_exit(NULL);

}// Fin de funciòn de leer
