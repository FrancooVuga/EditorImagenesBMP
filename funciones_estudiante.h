#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED
#include "constantes.h"
#include "estructuras.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#define ARCHIVO_NO_ESPECIFICADO -1


typedef int (*punteroFuncion)(FILE* pfOriginal, const char* nombreNuevoArchivo);

typedef struct
{
    const char* nombre;
    punteroFuncion funcion;
}t_funcion;

t_funcion buscarFuncion(const char* nombreFuncion);

int solucion(int argc, char* argv[]);

int negativo(FILE* pfOriginal, const char* nombreNuevoArchivo);
int escalaDeGrises(FILE* pfOriginal, const char* nombreNuevoArchivo);
int aumentarContraste(FILE* pfOriginal, const char* nombreNuevoArchivo);
int reducirContraste(FILE* pfOriginal, const char* nombreNuevoArchivo);
int tonalidadAzul(FILE* pfOriginal, const char* nombreNuevoArchivo);
int tonalidadVerde(FILE* pfOriginal, const char* nombreNuevoArchivo);
int tonalidadRoja(FILE* pfOriginal, const char* nombreNuevoArchivo);
int recortar(FILE* pfOriginal, const char* nombreNuevoArchivo);
int rotarDerecha(FILE* pfOriginal, const char* nombreNuevoArchivo);
int rotarIzquierda(FILE* pfOriginal, const char* nombreNuevoArchivo);
int espejar(FILE* pfOriginal, const char* nombreNuevoArchivo);



#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
