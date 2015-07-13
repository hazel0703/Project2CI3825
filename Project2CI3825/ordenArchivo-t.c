/******************************************************************************
* 																			  *
* Autores: Adriana Devera 09-11286					       					  *
*          Maria de Lourdes Garcia 10-10264                                   *
* 																			  *
* Archivo: ordenArchivo-t.c                                                   *
* Version: Hilos           						       						  *
* Descripción: Contiene a los hijos hoja,lee los argumentos y mide los tiempos*
* 			   en milisegundos.										          *
*																			  *
******************************************************************************/

#include  "funciones.h"

/*El procedimiento hiloIntermedio recibe como paramentro un arreglo que 
contiene a partir de param[2] los elementos a ordenar, en param[0] tiene la 
cantidad de elementos a ordenar, y en param[1] tiene la cantidad de hojas que 
el hilo intermedio debe generar, se crean dos listas, repartiendo los 
elementos obtenidos como argumento entre las dos listas segun el enunciado. 
estas listas se pasan como argumento a los dos nuevos hilos intermedios/hojas 
a ser creados, y se repite el proceso, cuando ambos hilos intermedios/hojas 
terminan su ejecucion, devuelven una lista ordenada de enteros sobre la cual 
se realiza merge-sort, y se retorna la lista final.*/

void hiloIntermedio(int * param)
{
	int tiempo1, tiempo2, tiempo3;
		tiempo1 = Tomar_Tiempo();
	int numsPorHoja = param[0]/param[1];
	int longitud1 = numsPorHoja*(param[1]/2);
	int * nuevaLista = malloc((longitud1+2)*sizeof(int));
	int * nuevaLista2 = malloc((param[0]-longitud1+2)*sizeof(int));
	
	nuevaLista[0] = longitud1;
	nuevaLista[1] = param[1]/2;
	
	int i = 0;
	
	for(i=0;i<longitud1;i++)
	{
		nuevaLista[i+2]=param[i+2];
	}
	
	nuevaLista2[0] = param[0]-longitud1;
	nuevaLista2[1] = param[1]/2;
	
	int j = 0;
	
	for(j=0;j<(param[0]-longitud1);j++)
	{
		nuevaLista2[j+2]=param[j+2+longitud1];
	}
	
	int *listaOrd;
	int *listaOrd2;
	
	if (param[1]>2)
	{
		pthread_t nuevoNodo1; 
		pthread_t nuevoNodo2; 
		pthread_create(&nuevoNodo1,NULL,hiloIntermedio,nuevaLista); 
		pthread_create(&nuevoNodo2,NULL,hiloIntermedio,nuevaLista2);   
		pthread_join(nuevoNodo1,(void**)&listaOrd);  
		pthread_join(nuevoNodo2,(void**)&listaOrd2);
		free(nuevaLista);
		free(nuevaLista2);
	} 
	else 
	{
		pthread_t nuevaHoja;
		pthread_create(&nuevaHoja,NULL,hiloHoja,nuevaLista);
		pthread_t nuevaHoja2;
		pthread_create(&nuevaHoja2,NULL,hiloHoja,nuevaLista2);  
		pthread_join(nuevaHoja,(void**)&listaOrd);
		pthread_join(nuevaHoja2,(void**)&listaOrd2);
	}
  
	int k = 0;
	int * listaFinal = malloc(param[0]*(sizeof(int)));
	int a = 0;
	int b = 0;

	for(k=0;k<param[0];k++)
	{
		if ((a<longitud1) && (b<(param[0]-longitud1)))
		{                     
			if(listaOrd[a]<listaOrd2[b])
			{
				listaFinal[k] = listaOrd[a];
				a++;                      
			} 
			else 
			{
				listaFinal[k] = listaOrd2[b];  
				b++;      
			}
		} 
		else 
		{
			if(a<longitud1)
			{
				listaFinal[k] = listaOrd[a];
				a++;                 
			}  
			else 
			{
				listaFinal[k] = listaOrd2[b];  
				b++;       
			} 
		}                            
	}
	 
	tiempo2 = Tomar_Tiempo();
	tiempo2 = tiempo2-tiempo1;
   
	printf("Tiempo de ejecucion de hilo intermedio: %f  milisegundos\n", tiempo2*0.000001); 

	pthread_exit(listaFinal);
}

/******************************************************************************
*                                                                             *
*                        PROGRAMA PRINCIPAL HILOS                             *
*                                                                             *
******************************************************************************/

/*El metodo main lee los argumentos pasados en la linea de comando al momento 
de compilar, verifica que sean validos, luego construye una lista con todos 
los elementos a ordenar "listaInicial" que ademas contiene en listaInicial[0]
la cantidad de enteros a ordenar, y en listaInicial[1] la cantidad de hojas a 
generar, se crea el hilo raiz y se pasa esta lista como parametro, al
finalizar su ejecucion se calcula su tiempo y se escribe la lista ordenada
resultante en el nuevo archivo de salida recibido como parametro.*/


int main (int argc, char * argv[]) 
{
	int tiempo1, tiempo2, tiempo3;
	tiempo1 = Tomar_Tiempo();
	pthread_t id;	
	
	//Se verifican los argumentos de entrada en la consola
	if (argc != 5) 
	{
	printf("Escribir: ordenArchivo-p NumEnteros NumNiveles ArchivoEnterosDesordenado ArchivoEnterosOrdenado\n");
	  exit(0);
	}

	char *archivoSalida = argv[4];
	char *archivoEntrada = argv[3];int numHojas = pow(2,atoi(argv[2])-1);
	int numElems = atoi(argv[1]);
	int ni = atoi(argv[1]);
	int numniveles = atoi(argv[2]);
	FILE *abin, *atex;
	
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

	/*************************************************************************/


	int  y[numElems];
	FILE *fh = fopen (archivoEntrada, "rb");

	if (fh != NULL) 
	{
		fread (y, sizeof (int), numElems, fh);
		fclose (fh);
	} 
	else 
	{
		   perror("Escribir: ./ordenArchivo-t NumEnteros NumNiveles ArchivoEnterosDesordenado ArchivoEnterosOrdenado\n");

		   exit(0);
	}

	int listaInicial[numElems+2];
	
	listaInicial[0] = numElems;
	listaInicial[1] = numHojas;
	
	int n;
	
	for(n=0;n<numElems;n++)
	{
		   listaInicial[n+2] = y[n];  
	}     

	//Toma el tiempo de ejecución del hilo Raiz 
	tiempo3 = Tomar_Tiempo();
	tiempo3 = tiempo3-tiempo1;
	/*************************************************************************/

	int *resp;   
	pthread_create(&id,NULL,hiloIntermedio,listaInicial);
	pthread_join(id,(void**)&resp);
	printf("Tiempo de ejecución del hilo Raíz: %f milisegundos\n", tiempo3*0.000001);

	FILE* archivo;
	archivo = fopen(archivoSalida, "w");
	
	if(archivo)
	{
		int j = 0;
		
		for(j=0;j<numElems;j++)
			fprintf(archivo, "%d\n", resp[j]);
		
		fclose(archivo);
	}
	else
	{
		perror("**Archivo de la Raiz fopen:");
		exit(0);
	}
	
	return 0;
}