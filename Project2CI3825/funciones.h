/******************************************************************************
* 																			  *
* Autores: Adriana Devera 09-11286					       					  *
*          Maria de Lourdes Garcia 10-10264                                   *
*																			  *
* Archivo: funciones.h                                                        *
*                                                                             *
******************************************************************************/

#ifndef funciones
	#define std
	#include <stdio.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <string.h>
	#include <sys/types.h>
	#include <sys/time.h>
	#include <unistd.h>
	#include <wait.h>
	#include <math.h>
	#include <time.h>
	//#include <pthread.h>	
#endif

/*Prototipos de las funciones del archivo funciones.c*/

int Tomar_Tiempo();

int pivot(int *lista, int izq, int der);

void quicksort(int * listaFinal,int izq,int der);

void extraer_arreglo(int inicio, int ni, FILE *file, int *arreglo);
  
void mezclar(int *a1, int *a2, int n1, int n2, int *destino);
  
void leer_archivo_hijo(int PIDhijo, int *arreglo, int tam);
 
void hiloHoja(int * param);

//void hiloIntermedio(int * param);



