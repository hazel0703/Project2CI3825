/******************************************************************************
* 																			  *
* Autores: Adriana Devera 09-11286					 					      *
*          Maria de Lourdes Garcia 10-10264                                   *
* 																			  *
* Archivo: ordenArchivo-p.c  						       					  *
* Versión: Procesos	                                                          *
* Descripcion: Crea arbol de procesos, el cual será ordenado por el algoritmo *
* 			   mergsort pedido, en donde cada nodo es un proceso hijo de la   *
*			   raíz del arbol. 												  *
*																			  *
******************************************************************************/

#include "funciones.h"

/******************************************************************************
*                                                                             *
*                        PROGRAMA PRINCIPAL PROCESOS                          *
*                                                                             *
******************************************************************************/

int main(int argc, char *args[]) 
{
	int tiempo1, tiempo2;
	tiempo1 = Tomar_Tiempo();
	
/*Se verifican los argumentos*/
	if (argc != 5) 
	{
		printf("Escribir: ordenArchivo-p NumEnteros NumNiveles ArchivoEnterosDesordenado ArchivoEnterosOrdenado\n");
		exit(0);
	}
	
	int ni = atoi(args[1]);
	int numniveles = atoi(args[2]);
	FILE *archivoBin, *atex;
	
	if (ni <= 0) 
	{
		printf("El número de enteros a ordenar debe ser entero positivo\n");
		exit(0);
	}

	if (numniveles <=0 ) 
	{
		printf("El número de niveles a ordenar debe ser entero positivo\n");
		exit(0);
	}
  
	if (ni < pow(2,numniveles-1)) 
	{
		printf("El número de enteros debe ser mayor o igual al número de hojas\n");
		exit(0);
	}

	/*Procedemos a crear el arbol de procesos*/

	int inicio, impar, nivel;
	pid_t hizq, hder;
	inicio = 0;

	for (nivel=1; nivel<numniveles; nivel++) 
	{
		impar = ni % 2;
		ni = ni/2;
		
		/*Se crea el hijo izquierdo*/
		if ((hizq = fork()) < 0) 
		{
			perror("Hijo izquierdo **fork:");
			exit(1);
		}
		
		/*Trozo de código del proceso padre*/
		if (hizq > 0) 
		{
			inicio = inicio+ni;
			ni = ni + impar;

			/*creación del nodo (hijo) derecho*/
			if ((hder = fork()) < 0) 
			{
				perror("Hijo derecho **fork:");
				exit(1);
			}
		}
		
		/*Si no es hijo, se sale del ciclo en el que estaba*/
		if ((hizq > 0) && (hder > 0)) 
			break;
	}
	
	/*************************************************************************/
	
	int total = (ni-impar)+ni;
	
	if (numniveles == 1)
		total = ni;

	int destino[total];
	
	if (numniveles > 1) 
	{
		int *status = NULL; 
		char inicioString[33];
		char niString[33];
	
		/*Estas funciones pasan de entero a string, para poder pasarle los 
		argumentos al execl*/
		sprintf(inicioString,"%d",inicio);
		sprintf(niString,"%d",ni);
		
		// Si es hoja, que ejecute otro programa
		if (nivel == numniveles)
			execl("./hoja","./hoja",inicioString,niString,args[3],NULL);
		
		// Espera por los dos hijos que creó
		wait(status);
		if (WIFSIGNALED(status)){
			perror("Proceso hijo no terminó correctamente:");
			exit(2);
		}
		wait(status);

		if (WIFSIGNALED(status)){
			perror("Proceso hijo no terminó correctamente:");
			exit(2);
		}
	   
		/**************Lectura del archivo PID.txt de los hijos***************/
		
		int arreglo1[ni-impar];
		leer_archivo_hijo(hizq,arreglo1,ni-impar);
		
		int arreglo2[ni];
		leer_archivo_hijo(hder,arreglo2,ni);
		
		/*********************************************************************/
			
		//Creación del archivo del padre

		mezclar(arreglo1,arreglo2,ni-impar,ni,destino);
	}
	
	/* Código que ejecutarán las intermedios no raíz (crear un archivo con sus 
	PID*/
	if (nivel > 1) 
	{	
		char PIDString[33];
		char *txt = ".txt";
	
		sprintf(PIDString,"%d",getpid());
		strcat(PIDString,txt);
	   
		if (!(atex = fopen(PIDString, "w"))) 
		{
			perror("**fopen:");
			exit(0);
		}

		txt= malloc(sizeof(char));
		/* Código que ejecuta la raíz (crear un archivo con el nombre pasado como parámetro)*/
	} 
	else 
	{
		if (!(atex = fopen(args[4], "w"))) 
		{
			perror("Archivo de proceso raiz **fopen:");
			exit(0);
		}
	}
	
	if (numniveles == 1) 
	{
		if (!(archivoBin = fopen(args[3], "r"))) 
		{
			perror("**fopen:"); /*Archivo de entrada en main*/
			exit(0);
		}
		
		extraer_arreglo(0,ni,archivoBin,destino);
		
		if (fclose(archivoBin)) 
		{
			perror("**fclose:"); /*Archivo de entrada en main*/
			exit(0);
		}
	
		quicksort(destino,0,ni-1);
	}
	
	int i;
	for (i=0; i<total; i++)
		fprintf(atex,"%d\n",destino[i]);
	
	if (fclose(atex)) {
		perror("Archivo de proceso raiz/intermedio **fclose:");
		exit(0);
	}

	/*************************************************************************/
  
	tiempo2 = Tomar_Tiempo();
	tiempo2 = tiempo2-tiempo1;
	if (nivel == 1)
		printf("Tiempo de ejecución del proceso Raíz: %f milisegundos\n", tiempo2*0.000001);
	else
		printf("Tiempo de ejecución del proceso Intermedio: %f milisegundos\n", tiempo2*0.000001);
	return 0;
}
