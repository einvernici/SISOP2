#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdbool.h>
#define QUEUE_SIZE 5

// ESTRUTURA DOS PRODUTOS - INICIO
enum Tipo {
	PARAFUSO,
	PREGO,
	PORCA
};

enum Qualidade {
	BOM,
	DEFEITO
};

typedef struct Produto{
	enum Tipo tipo;
	enum Qualidade qualidade;
	int tempo;
} Produto;

//FUNCOES
Produto* createProduto(int index);
// ESTRUTURA DOS PRODUTOS - FIM

//ESTRUTURA DA FILA - INICIO
typedef struct CircularQueue {
	Produto* esteira[QUEUE_SIZE];
	int front;
	int back;
} CircularQueue;

//FUNCOES
bool isFull(CircularQueue* queue);
bool isEmpty(CircularQueue* queue);
void enqueue(CircularQueue* queue, Produto* produto);
Produto* dequeue(CircularQueue* queue);
void display(CircularQueue* queue);
int peek(CircularQueue* queue);
//ESTRUTURA DA FILA - FIM

#endif
