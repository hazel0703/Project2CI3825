/******************************************************************************
* 																			  *
* Autores: Adriana Devera 09-11286					       					  *
*          Maria de Lourdes Garcia 10-10264                                   *
*																			  *
* Archivo: funciones.c                                                        *
*                                                                             *
******************************************************************************/

#include "funciones.h"

/******************************************************************************
*                                                                             *
*                   Funciones para los Procesos e Hilos                       *
*                                                                             *
******************************************************************************/

/*Función que se utiliza para medir el tiempo de ejecución, tomado del ejemplo 
dado en el enunciado*/

int Tomar_Tiempo() 
{  
	struct timeval t; /* usado para tomar los tiempos */
	int dt;
	gettimeofday ( &t, (struct timezone*)0 );
	dt = (t.tv_sec)*1000000 + t.tv_usec;
	return dt;
}

/**********************Función quicksort para las hojas***********************/

int pivot(int *lista, int izq, int der)
{
	int i,pivote,valor_pivote,aux;

	pivote = izq;
	valor_pivote = lista[pivote];

	for (i=izq+1; i<=der; i++)
	{
		if (lista[i] < valor_pivote)
		{
			pivote++;
			aux=lista[i];
			lista[i]=lista[pivote];
			lista[pivote]=aux;
		}
	}

	aux=lista[izq];
	lista[izq]=lista[pivote];
	lista[pivote]=aux;

	return pivote;
}

void  quicksort(int * listaFinal,int izq,int der)
{
	int pivote;

	if(izq < der)
	{
		pivote=pivot(listaFinal, izq, der);
		quicksort(listaFinal, izq, pivote-1);
		quicksort(listaFinal, pivote+1, der);
	}
}

/******************************************************************************
*                                                                             *
*                       Funciones para los procesos		     	              *
*                                                                             *
******************************************************************************/

/*Función que se utiliza para buscar la posición inicial y final en el archivo
binario, y guarda la cantidad de enteros pedida por consola en otro arreglo*/

void extraer_arreglo(int inicio, int ni, FILE *file, int *arreglo) 
{
	fseek(file, inicio*4, SEEK_SET);
	
	if (!(fread(arreglo, sizeof(int), ni, file))) 
	{
		perror("Archivo de entrada en hoja **fread:");
		exit(0);
	}
}

/*Función que es utilizada para mezclar los arreglos creados, y crear solo uno 
ordenado, aquí se mezclan las secuencias ordenadas de sus procesos.*/

void mezclar(int *a1, int *a2, int n1, int n2, int *destino) 
{	
	int i, j, k;

	i = 0;
	j = 0;
	k = 0;
	
	while ((i < n1) && (j < n2)) 
	{
		if (a1[i] < a2[j]) 
		{
			destino[k] = a1[i];
			i++;
		} 
		else 
		{
			destino[k] = a2[j];
			j++;
		}
		k++;
	}

	while (i < n1) 
	{
		destino[k] = a1[i];
		i++;
		k++;
	}

	while (j < n2) 
	{
		destino[k] = a2[j];
		j++;
		k++;
	}
}

/*Función que se utiliza para leer los números de un archivo con el PID del 
hijo del proceso, es un arreglo que contiene todos los números enteros leídos*/

void leer_archivo_hijo(int PIDhijo, int *arreglo, int tam) 
{		
	FILE *fin;
	char PIDString[33];
	char *txt = ".txt";
	
	sprintf(PIDString,"%d",PIDhijo);
	strcat(PIDString,txt);
	/*Abre el archivo intermedio*/

	if (!(fin = fopen(PIDString, "r"))) 
	{
		perror("Archivo con PID del hijo **fopen:");
		exit(0);
	}
	
	int i;

	for (i=0; i<tam; i++) 
	{
		fscanf(fin,"%d", &arreglo[i]);
	}

	/*cierra los archivos intermedios creados*/
	if (fclose(fin)) 
	{
		perror("Archivo con PID del hijo **fclose:");
		exit(0);
	}
	
	/*Se borran los archivos intermedios creados para mezclar*/
	if (remove(PIDString)) 
	{
		perror("Archivo con PID del hijo **remove:");
		exit(0);
	}
}


/******************************************************************************
*                                                                             *
*                       Funciones para los hilos                              *
*                                                                             *
******************************************************************************/

/*El procedimiento hiloHoja recibe como paramentro un arreglo que contiene a 
partir de param[2] los elementos a ordenar, se copian estos elementos a una 
nueva lista y se ejecuta quicksort sobre esta, finalmente se libera la memoria 
de ¨param¨ y devuelve la nueva lista ordenada*/

void  hiloHoja(int * param) 
{
	int * listaFinal2 = malloc(param[0]*sizeof(int));
	int k =0;

	for(k=0;k<param[0];k++)
	{
		listaFinal2[k] = param[k+2];
	}

	quicksort(listaFinal2,0,param[0]-1);
	free(param);
	pthread_exit(listaFinal2);
	free(listaFinal2);
}