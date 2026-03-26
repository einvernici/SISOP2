#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"

//FUNCOES PRODUTO - IMPLEMENTACAO
Produto* createProduto(int index){
	Produto* p = malloc(sizeof(Produto));
	p->tipo = index;
	p->qualidade = rand() % 2;
	p->tempo = rand() % 3 + 1;
	return p;
}
//FUNCOES PRODUTO - FIM

//FUNCOES FILA - IMPLEMENTACAO
bool isFull(CircularQueue* queue){
	if((queue->back + 1) % QUEUE_SIZE == queue->front){
		return true;
	} else {
		return false;
	}
}

bool isEmpty(CircularQueue* queue){
	if(queue->front == -1){
		return true;
	} else {
		return false;
	}
}

void enqueue(CircularQueue* queue, Produto* produto){
	if(isFull(queue)){
		printf("Fila cheia. Insercao negada.");
		return;
	}
	if(isEmpty(queue)){
		queue->front = 0;
	}
	queue->back = (queue->back + 1) % QUEUE_SIZE;
	queue->esteira[queue->back] = produto;
}

Produto* dequeue(CircularQueue* queue){
	if(isEmpty(queue)){
		printf("Fila vazia. Remocao negada.");
	}
	Produto* p = queue->esteira[queue->front];
	if(queue->front == queue->back){
		queue->front = queue->back = -1;
	} else {
		queue->front = (queue->front + 1) % QUEUE_SIZE;
	}
	return p;
}

void display(CircularQueue* queue){
	if(isEmpty(queue)){
		printf("Esteira vazia.\n");
		return;
	}
	int i = queue->front;
	printf("Esteira:\n");
	if(queue->front <= queue->back){
		while(i <= queue->back){
			printf("tipo: %d, qualidade: %d\n", 
			queue->esteira[i]->tipo, queue->esteira[i]->qualidade);
			i++;
		}
	} else {
		while(i < QUEUE_SIZE){
			printf("tipo: %d, qualidade: %d\n", 
			queue->esteira[i]->tipo, queue->esteira[i]->qualidade);
			i++;
		}
		i = 0;
		while(i <= queue->back){
			printf("tipo: %d, qualidade: %d\n", 
			queue->esteira[i]->tipo, queue->esteira[i]->qualidade);
			i++;
		}
	}
}

int peek(CircularQueue* queue){
	Produto* p = queue->esteira[queue->front];
	return p->qualidade;
}
//FUNCOES FILA - FIM
