#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define N_ESCRITORES 5
#define N_LEITORES 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//int reader = 0;
int writer = 0;

void *Leitor(void *arg){
	int id = (int) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		if(writer > 0)
			pthread_cond_wait(&cond, &mutex);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		printf("Leitor %d esta lendo....\n", id);	
	}
	pthread_exit(NULL);
}

void *Escritor(void *arg){
	int id = (int) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		if(writer > 0)
			pthread_cond_wait(&cond, &mutex);
		else{
			writer++;
			printf("Escritor %d esta escrevendo...\n", id);			
		}
		pthread_mutex_unlock(&mutex);

		pthread_mutex_lock(&mutex);
		printf("Escritor %d terminou de escrever.\n", id);
		writer--;
		pthread_cond_signal(&cond);	
		pthread_mutex_unlock(&mutex);
		
	}
	pthread_exit(NULL);
}

int main(void){
	pthread_t leitor[N_LEITORES], escritor[N_ESCRITORES];
	int i, rc;

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

