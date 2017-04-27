#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define N_ESCRITORES 5
#define N_LEITORES 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int writer = 0;

void *Leitor(void *arg){
	long id = (long)arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while(writer > 0)
			pthread_cond_wait(&cond, &mutex);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		srand(time(NULL));

		printf("Leitor %ld esta lendo....\n", id);
		sleep(1 + rand() % 4);	
	}
	pthread_exit(NULL);
}

void *Escritor(void *arg){
	long id = (long)arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while(writer > 0)
			pthread_cond_wait(&cond, &mutex);
		writer++;	
		pthread_mutex_unlock(&mutex);
		srand(time(NULL));

		printf("Escritor %ld esta escrevendo...\n", id);
		//sleep(1 + rand() % 4);

		pthread_mutex_lock(&mutex);
		printf("Escritor %ld terminou de escrever!\n", id);
		writer--;
		pthread_cond_signal(&cond);	
		pthread_mutex_unlock(&mutex);
		
	}
	pthread_exit(NULL);
}

int main(void){
	pthread_t leitor[N_LEITORES], escritor[N_ESCRITORES];
	long i;
	int rc;

	for(i = 0; i < N_LEITORES; i++){
		rc = pthread_create(&leitor[i], NULL, Leitor, (void *)i);
		if(rc){
			printf("Nao foi possivel criar a thread de leitor\n");
			exit(-1);
		}
	}

	for(i = 0; i < N_ESCRITORES; i++){
		rc = pthread_create(&escritor[i], NULL, Escritor, (void *)i);
		if(rc){
			printf("Nao foi possivel criar a thread de escritor\n");
			exit(-1);
		}
	}

	for(i = 0; i < N_LEITORES; i++){
		pthread_join(leitor[i], NULL);
	}
	
	for(i = 0; i < N_ESCRITORES; i++){
		pthread_join(escritor[i], NULL);
	}

}
