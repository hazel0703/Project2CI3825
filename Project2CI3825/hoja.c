/******************************************************************************
* 																			  *
* Autores: Adriana Devera 09-11286					       					  *
*          Maria de Lourdes Garcia 10-10264                                   *
*																			  *
* Archivo: hoja.char                                                          *
*                                                                             *
******************************************************************************/

#include "funciones.h"

/******************************************************************************
*                                                                             *
*                          PROGRAMA PRINCIPAL DE HOJA                         *
*                                                                             *
******************************************************************************/


int main(int argc, char *args[]) 
{    
	int tiempo1, tiempo2;
	/*Se toma el tiempo de ejecución de los procesos*/
	tiempo1 = Tomar_Tiempo(); 
	
	if (argc != 4) 
	{
		exit(0);
	}
	
	int inicio, ni;

	inicio = atoi(args[1]);
	ni = atoi(args[2]);
	FILE *archivoBin, *fout;
	
	/****************Esto lee el archivo y ordena el arreglo******************/
	
	if (!(archivoBin = fopen(args[3], "r"))) 
	{
		perror("Archivo de entrada en hoja **fopen:");
		exit(0);
	}
	
	int arreglo[ni]; 
	extraer_arreglo(inicio,ni,archivoBin,arreglo);
	quicksort(arreglo,0,ni-1);

	/*Esto crea los archivos con nombre de PID*/

	char PIDString[33];
	char *txt = ".txt"; /*Extensión para crear los archivos creados con el 
						  PID, lo piden en formato .txt*/
	
	sprintf(PIDString,"%d",getpid());
	strcat(PIDString,txt); /*Utilizado para concatenar el nombre del archivo + 
						    .txt */ 
	
	if (!(fout = fopen(PIDString, "w"))) 
	{
		perror("Archivo de salida en hoja **fopen:");
		exit(0);
	}

	/*Se imprime el arreglo en el archivo*/

	int i;

	for (i=0; i<ni; i++)
		fprintf(fout,"%d\n",arreglo[i]);
 
	/*Cierra el archivo binario*/   
	if (fclose(archivoBin)) 
	{
		perror("Archivo de entrada en hoja **fclose:");
		exit(0);
	}
	if (fclose(fout)) 
	{
		perror("Archivo de salida en hoja **fclose:");
		exit(0);
	}
	
	/*************************************************************************/
	
	/*Se toma el tiempo y se imprime en pantalla una vez que haya terminado el 
	programa*/

	tiempo2 = Tomar_Tiempo();
	tiempo2 = tiempo2-tiempo1;
	printf("Tiempo de ejecución de la Hoja: %f milisegundos\n",tiempo2*0.000001);
	return 0;
}
