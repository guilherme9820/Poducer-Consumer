#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define TAM 10

int buffer[TAM] = {0};
int qtd = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void printaVetor(){
	int i;
	printf("buffer[] = ");
	for(i = 0; i < TAM; i++)
		printf("%d ", buffer[i]);
	printf("\n");
}

void *Produz(){
	int valor;
	int iterator = 0;
	while(1){
		pthread_mutex_lock(&mutex);
		if(qtd == 10)
			pthread_cond_wait(&cond, &mutex);
		else{
			srand(time(NULL));
			valor = rand() % 51;
			buffer[iterator] = valor;
			printf("Valor inserido no buffer[%d] = %d\n", iterator, buffer[iterator]);
			iterator = (iterator + 1) % TAM;
			qtd++;	
			printaVetor();	
		}
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void *Consome(){
	int iterator = 0;
	while(1){
		pthread_mutex_lock(&mutex);
		if(qtd == 0)
			pthread_cond_wait(&cond, &mutex);
		else{
			printf("Valor consumido de buffer[%d] = %d\n", iterator, buffer[iterator]);
			buffer[iterator] = 0;
			iterator = (iterator + 1) % TAM;
			qtd--;
			printaVetor();
		}
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main(void){
	pthread_t produtor, consumidor;
	pthread_create(&produtor, NULL, Produz, NULL);
	pthread_create(&consumidor, NULL, Consome, NULL);	

	pthread_join(produtor, NULL);
	pthread_join(consumidor, NULL);
}

