#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//Constante que guarda a quantia de filosofos
#define FILOSOFOS 5
//Constante que guarda a quantia de garfos
#define GARFOS 5

//Status possiveis:
#define COMENDO	0
#define PENSANDO 1

//Declaração dos semáforos
sem_t mutex;
sem_t garfos[GARFOS];

//Declaração do array com o status de cada filosofo
int statusFilosofo[FILOSOFOS];

//Função que trata da tentativa de um filosofo comer
void comer(int fil){
	//Checa se o filosofo não está comendo - e se há garfos disponiveis (nenhum de seus 'vizinhos está comendo')
	if(statusFilosofo[fil] != COMENDO && statusFilosofo[ (fil + 4) % FILOSOFOS ] != COMENDO && statusFilosofo[ (fil + 1) % FILOSOFOS ] != COMENDO){
		//Atualiza o status do filosofo
		statusFilosofo[fil] = COMENDO;
		//Acessa os garfos
		sem_wait(&mutex);
		sem_wait(&garfos[fil]);
		sem_wait(&garfos[(fil + 1) % 5]);

		printf("\nFilosofo %d está comendo",fil + 1);
		sleep(2);

		printf("\nFilosofo %d terminou de comer",fil + 1);
		//Libera acesso aos garfos
		sem_post(&garfos[(fil + 1) % 5]);
		sem_post(&garfos[fil]);
		sem_post(&mutex);
		//Atualiza o status de cada filosofo
		statusFilosofo[fil] = PENSANDO;
	}
}

//Funçãp que será chamada na thread que lida com os 'movimentos' de cada filosofo
void * filosofo (void * num){
	while(1){
		int* fil = num;
		sleep(1);
		comer(*fil);
		sleep(0);
	}
}

int main(){
	int i;
	pthread_t threads[FILOSOFOS];
	int filosofos[FILOSOFOS];

	//Inicia semaforo mutex
	sem_init(&mutex,0,4);
	//Laço que inicia os semaforos de cada garfo e seleciona o status inicial de cada filosofo
	for(i=0; i < GARFOS; i++){
		sem_init(&garfos[i],0,1);
		statusFilosofo[i] = PENSANDO;
	}
	//Inicia as threads de cada processo que lida com cada filosofo
	for(i = 0; i < FILOSOFOS; i++){
		filosofos[i] = i;
		pthread_create(&threads[i],NULL,filosofo,(void *)&filosofos[i]);
		printf("\nFilosofo %d está pensando", i+1);
	}
	//Adiciona treads no pool
	for(i=0; i < FILOSOFOS; i++){
		pthread_join(threads[i],NULL);
	}
	//FIM
}
