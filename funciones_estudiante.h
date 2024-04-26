#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

#include "constantes.h"
#include "estructuras.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int solucion(int argc, char* argv[]);

typedef int (*punteroFuncion)(const char* nombreArchivo);

typedef struct
{
    const char* nombre;
    punteroFuncion funcion;
}t_funcion;

punteroFuncion buscarFuncion(const char* nombreFucion);

int negativo(const char* nombreArchivo);
int escalaDeGrises(const char* nombreArchivo);
int aumentarContraste(const char* nombreArchivo);
int reducirContraste(const char* nombreArchivo);
int tonalidadAzul(const char* nombreArchivo);
int tonalidadVerde(const char* nombreArchivo);
int tonalidadRoja(const char* nombreArchivo);
int recortar(const char* nombreArchivo);
int rotarDerecha(const char* nombreArchivo);
int rotarIzquierda(const char* nombreArchivo);
int comodin(const char* nombreArchivo); // REEMPLAZAR



#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
