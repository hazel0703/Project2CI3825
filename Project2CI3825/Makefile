############################################
# Adriana Devera 09-11286		   #
# Maria de Lourdes Garcia 10-10264 	   #
# Archivo: Makefile			   #
# Descripción: ejecución de los procesos e #
# hilos. 				   #
############################################

all: ordenArchivo-p ordenArchivo-t hoja

ordenArchivo-p: funciones.o ordenArchivo-p.o
	gcc  funciones.o ordenArchivo-p.o -o ordenArchivo-p -lm

hoja: funciones.o hoja.o
	gcc funciones.o hoja.o -o hoja

ordenArchivo-t: funciones.o ordenArchivo-t.o
	gcc funciones.o ordenArchivo-t.o -pthread -o ordenArchivo-t -lm
	
.SUFFIXES: .o .c
.c.o:
	gcc -c -g $<

clean:
	rm -f *.o ordenArchivo-p hoja ordenArchivo-t
