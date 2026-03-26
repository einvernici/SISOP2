/*Compilar junto com o arquivo das estruturas: "gcc GA1.c estruturas.c -o <nome do executavel>"*/
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>
#include <time.h>
#include "estruturas.h"

#define PRODUTORES 3 	//quantidade threads produtores
#define CONSUMIDORES 2	//quantidade threads consumidores
#define RUNNING_TIME 5	//tempo de duracao do programa

//declaracao estruturas
pthread_mutex_t esteira_mutex;
sem_t semaforoProdutor;
sem_t semaforoConsumidor;

//traduzir enum Tipo em string
char* tipo_string[] = {
        "PARAFUSO",
        "PREGO",
        "PORCA"
};

//traduzir enum Qualidade em string
char* qualidade_string[] = {
        "BOM",
        "DEFEITO"
};

//criacao e inicializacao variaveis globais
CircularQueue esteira = { .front = -1, .back = -1 };
int time_to_run = 1;
int produtorRodou[PRODUTORES];
int consumidorRodou[CONSUMIDORES];

// FUNCAO PRODUTOR - INICIO
void* produtorFunction(void* arg){
	int index = (intptr_t)arg;
	while(time_to_run){
		sem_wait(&semaforoProdutor);
		pthread_mutex_lock(&esteira_mutex);
		Produto* p = createProduto(index); 
		int tempo = p->tempo;
		//insere na esteira
		enqueue(&esteira, p);
		//log produtos produzidos
		FILE *file = fopen("produtores.txt", "a");
		fprintf(file,"(P%d): tipo: %s, qualidade: %s, tempo: %d\n", 
		index, tipo_string[p->tipo], qualidade_string[p->qualidade], p->tempo);
		fclose(file);
		//incrementa numero de vezes que executou
		produtorRodou[index]++;
		//printf("Produto inserido na esteira.\n");
		pthread_mutex_unlock(&esteira_mutex);
		sem_post(&semaforoConsumidor);
		sleep(tempo);
	}
}
// FUNCAO PRODUTOR - FIM

// FUNCAO CONSUMIDOR - INICIO
void* consumidorFunction(void* arg){
	int index = (intptr_t)arg;
	while(time_to_run){
		sem_wait(&semaforoConsumidor);
		pthread_mutex_lock(&esteira_mutex);
		int qualidadeProduto = peek(&esteira);
		//checa se pode consumir
		if(index == qualidadeProduto){
			//consome produto
			Produto* p = dequeue(&esteira);
			//log produtos consumidos
			FILE *file = fopen("consumidores.txt", "a");
			fprintf(file, "(C%d): tipo: %s, qualidade: %s\n", index, tipo_string[p->tipo],qualidade_string[p->qualidade]);
			fclose(file);
			//libera memoria alocada para produto
			free(p);
			//incrementa numero de vezes que executou
			consumidorRodou[index]++;
			pthread_mutex_unlock(&esteira_mutex);
			sem_post(&semaforoProdutor);
			sleep(1);
		} else {
			sem_post(&semaforoConsumidor);
			//incrementa numero de vezes que executou
			consumidorRodou[index]++;
			pthread_mutex_unlock(&esteira_mutex);
			sleep(1);
		}
	}
}
// FUNCAO CONSUMIDOR - FIM

//FUNCAO MONITOR - INICIO
void* monitorFunction(void* arg){
	while(time_to_run){
		pthread_mutex_lock(&esteira_mutex);
		display(&esteira);
		pthread_mutex_unlock(&esteira_mutex);
		sleep(1);
	}
}
//FUNCAO MONITOR - FIM

int main(){
	srand(time(NULL));
	pthread_t produtor[PRODUTORES], consumidor[CONSUMIDORES], monitor;
	pthread_mutex_init(&esteira_mutex, NULL);
	sem_init(&semaforoProdutor, 0, 5);
	sem_init(&semaforoConsumidor, 0, 0);

	for(int i = 0; i < PRODUTORES; i++){
		//cast valor de i para ponteiro de int - cada thread tem seu proprio index
		pthread_create(&produtor[i], NULL, &produtorFunction, (void*)(intptr_t)i);
	}
	for(int i = 0; i < CONSUMIDORES; i++){
		pthread_create(&consumidor[i], NULL, &consumidorFunction, (void*)(intptr_t)i);
	}
	pthread_create(&monitor, NULL, &monitorFunction, NULL);

	sleep(RUNNING_TIME);
	time_to_run = 0;
	
	for(int i = 0; i < PRODUTORES; i++){
		pthread_join(produtor[i], NULL);
		printf("Produtor(%d) finalizado.\n", i);
	}
	for(int i = 0; i < CONSUMIDORES; i++){
		pthread_join(consumidor[i], NULL);
		printf("Consumidor(%d) finalizado.\n", i);
	}	
	pthread_join(monitor, NULL);
	printf("Monitor finalizado.\n");

	printf("\nSOBROU NA ESTEIRA\n");
	display(&esteira);
	printf("\nESTATISTICAS PRODUTORES\n");
	for(int i = 0; i < PRODUTORES; i++){
		printf("(P%d) executou %d vezes\n", i, produtorRodou[i]);
	}
	printf("\nESTATISTICAS CONSUMIDORES\n");
	for(int i = 0; i < CONSUMIDORES; i++){
		printf("(C%d) executou %d vezes\n", i, consumidorRodou[i]);
	}
	pthread_mutex_destroy(&esteira_mutex);
	sem_destroy(&semaforoProdutor);
	sem_destroy(&semaforoConsumidor);
	return 0;
}
