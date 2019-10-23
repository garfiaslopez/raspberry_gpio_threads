/*
 * wnoise_t.cpp
 *
 *
 *
*/

#include <pthread.h>
#include <bcm2835.h>
#include <stdlib.h>   /* Contiene a random */

#define INPUT RPI_BPLUS_GPIO_J8_05

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

typedef struct data {
  unsigned char *samples;
  unsigned int size;
  snd_pcm_t *handle;
  snd_pcm_frames frames;
}data;

int main(int argc, char **argv) {

  data datos = {(char *)malloc(char*255),255};

  /* creacion de hilos */
  pthread_create(&white_noise_t,NULL,white_noise,(void *)data);
  pthread_create(&play_noise_t,NULL,play_noise,(void *)data);
  pthread_create(&read_button_t,NULL,read_button,(void *)data);

  printf("Reproduciendo rudio blanco\n ");

  /* union de hilo */
  pthread_join(white_noise_t,NULL);
  pthread_join(play_noise_t,NULL);
  pthread_join(read_button_t,NULL);

  pthread_exit(NULL);

} // fin main

/* Funcion que genera numeros aleatorios y los
 * amacena en memoria */
void *white_noise(void *datos) {
  data datos = (data)datos; //recuperando datos 
  int i;

  while(1) {
    // segmento de programa crítico
    pthread_mutex_lock(&mutex1);
    for(i=0;i<datos.size;i++) {
      //num. aleatorios 0-255
      datos.samples[i] = random() & 0xFF;
    } // fin for
    pthread_mutex_unlock(&mutex1);
    // fin de segmento crítico
  } // fin while

} //fin de white_noise
/* ---------------------------------------------- */


/* Funcion que reproduce ruido blanco en la tarjeta
 * de sonido */
void *play_noise(void *datos) {

  data datos = (data)datos; //recuperando datos

  // reproducción de muestras
  while(1) {
    // segmento de programa crítico
    pthread_mutex_lock(&mutex1);
      //reproducir muestras
      snd_pcm_writei(datos.handles,datos.data,datos.size);
    pthread_mutex_unlock(&mutex1);
    // fin de segmento crítico
  } // fin while
 
} // fin play noise

/* Funcion que lee el estado del boton
 * y pausa la reproducción */
void *read_button(void *id) {
   /* 1. Inicializar chip bcm2835 */
   bcm2835_init();

   /* 2. Definir puerto como entrada para iniciar reproduccion */
   bcm2835_gpio_fsel(INPUT,BCM2835_GPIO_FSEL_INPT);
   /* 3. Habilita resistecia de elevacion */
   bcm2835_gpio_set_pud(INPUT, BCM2835_GPIO_PUD_UP);

   printf("Oprima el boton para detener la reproducción\n")

   /* 4. Leer estado boton */
   while(1) {
     if(bcm2835_gpio_lev(INPUT) == LOW) {
       pthread_mutex_lock(&mutex1);
       printf("Reproducción detenida por 5s\n ");
       bcm2835_delay(5000);
       pthread_mutex_unlock(&mutex1);
       printf("Reproducción restaurada\n ");
     } // fin if
   } //fin while
} // fin funcion read_button

