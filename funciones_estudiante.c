/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    (Sólo para grupos de tres integrantes)
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

*/

/* Parametros originales:
--docente  /home/ezequiel/unlam.bmp --negativo --escala-de-grises


*/

#include "constantes.h"
#include "funciones_estudiante.h"

int solucion(int argc, char* argv[])
{
/*
    Aquí deben hacer el código que solucione lo solicitado.
    Todas las funciones utilizadas deben estar declaradas en este archivo, y en su respectivo .h
*/
// Primero busco el indice del archivo .bmp si no es el ultimo parametro:
    int i;
    int indiceArchivoBMP = ARCHIVO_NO_ENCONTRADO;

    for(i = 1; i < argc; i++)
    {
        if(strstr(argv[i], ".bmp"))
        {
            indiceArchivoBMP = i;
        }
    }

// Ahora proceso todos los argumentos si se encontró el .bmp:
    //int resultadoFuncion;
    if(indiceArchivoBMP != ARCHIVO_NO_ENCONTRADO)
    {
       // Ahora por cada argumento a main, llamo a la respectiva funcion
        int j;
        for(j = 1; j < argc; j++)
        {
            punteroFuncion funcionActual = buscarFuncion(argv[j]);
            if(funcionActual)
            {
                if(funcionActual(argv[indiceArchivoBMP]))
                {
                    printf("No se pudo hacer la operacion '%s' sobre la imagen.\n", argv[j]);

                }

            }
        }
    }
    else
    {
        printf("No se especifico un archivo '.bmp'.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    return TODO_OK;
}

punteroFuncion buscarFuncion(const char* nombreFuncion)
{
    // Donde deberia estar declarado?
    static t_funcion tablaFunciones[] = {{"--negativo", negativo},
                              {"--escala-de-grises", escalaDeGrises},
                              {"--aumentar-contraste", aumentarContraste},
                              {"--reducir-contraste", reducirContraste},
                              {"--tonalidad-azul", tonalidadAzul},
                              {"--tonalidad-verde", tonalidadVerde},
                              {"--tonalidad-roja", tonalidadRoja},
                              {"--recortar", recortar},
                              {"--rotar-derecha", rotarDerecha},
                              {"--rotar-izquierda", rotarIzquierda},
                              {"--comodin", comodin},
                              {NULL, NULL}
                             };


    for(t_funcion* f = tablaFunciones; f->nombre != NULL; f++)
    {
        if(strcmp(nombreFuncion, f->nombre) == 0)
        {
            return f->funcion;
        }
    }
    printf("La funcion '%s' no es valida.\n", nombreFuncion);
    return NULL;
}

int negativo(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_negativo.bmp", "wb");

    if( !pfOrigen || !pfNuevo)
    {
        fclose(pfOrigen);
        fclose(pfNuevo);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Leo el tamaño del encabezadoBMP:
    char tamEncabezado;
    fseek(pfOrigen, 14, SEEK_SET);
    fread(&tamEncabezado, 4, 1, pfOrigen);

// Declaro una variable para almacenar el encabezado y luego escribirlo en el nuevo archivo
    char encabezadoBMP[tamEncabezado];
    fseek(pfOrigen, 0, SEEK_SET);
    fread(&encabezadoBMP, tamEncabezado, 1, pfOrigen);
    fwrite(encabezadoBMP, tamEncabezado, 1, pfNuevo);

// Guardo los datos en la estructura 'infoEncabezado':
    t_metadata infoEncabezado;

    memcpy(&infoEncabezado.tamArchivo, &encabezadoBMP[2], 4);
    memcpy(&infoEncabezado.tamEncabezado, &encabezadoBMP[14], 4);
    memcpy(&infoEncabezado.comienzoImagen, &encabezadoBMP[10], 4);
    memcpy(&infoEncabezado.ancho, &encabezadoBMP[18], 4);
    memcpy(&infoEncabezado.alto, &encabezadoBMP[22], 4);
    memcpy(&infoEncabezado.profundidad, &encabezadoBMP[2], 2);
// Puedo usar la variable 'tamImagen' o hacer directamente 'tamArchivo' - 'comienzoImagen'
    unsigned int tamImagen;
    memcpy(&tamImagen, &encabezadoBMP[34], 4);

    int i;
    unsigned char byte;
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < tamImagen; i++)
    {
        fread(&byte, 1,1, pfOrigen);
        byte = ~byte;
        fwrite(&byte, 1,1, pfNuevo);
    }




    printf("Se invoco la funcion 'negativo'.\n");
    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int escalaDeGrises(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_escala_de_grises.bmp", "wb");

    if( !pfOrigen || !pfNuevo)
    {
        fclose(pfOrigen);
        fclose(pfNuevo);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Leo el tamaño del encabezadoBMP:
    char tamEncabezado;
    fseek(pfOrigen, 14, SEEK_SET);
    fread(&tamEncabezado, 4, 1, pfOrigen);

// Declaro una variable para almacenar el encabezado y luego escribirlo en el nuevo archivo
    char encabezadoBMP[tamEncabezado];
    fseek(pfOrigen, 0, SEEK_SET);
    fread(&encabezadoBMP, tamEncabezado, 1, pfOrigen);
    fwrite(encabezadoBMP, tamEncabezado, 1, pfNuevo);

// Guardo los datos en la estructura 'infoEncabezado':
    t_metadata infoEncabezado;

    memcpy(&infoEncabezado.tamArchivo, &encabezadoBMP[2], 4);
    memcpy(&infoEncabezado.tamEncabezado, &encabezadoBMP[14], 4);
    memcpy(&infoEncabezado.comienzoImagen, &encabezadoBMP[10], 4);
    memcpy(&infoEncabezado.ancho, &encabezadoBMP[18], 4);
    memcpy(&infoEncabezado.alto, &encabezadoBMP[22], 4);
    memcpy(&infoEncabezado.profundidad, &encabezadoBMP[2], 2);
// Puedo usar la variable 'tamImagen' o hacer directamente 'tamArchivo' - 'comienzoImagen'
    unsigned int tamImagen;
    memcpy(&tamImagen, &encabezadoBMP[34], 4);

    int i;
    float promedio;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < tamImagen; i+=3)
    {
        fread(pixel, 3,1, pfOrigen);
        promedio = (pixel[0] + pixel[1] + pixel[2]) / 3;
        pixel[0] = pixel[1] = pixel[2] = promedio;
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'escalaDeGrises'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);
    return TODO_OK;
}

int aumentarContraste(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_aumentar_contraste.bmp", "wb");

    if( !pfOrigen || !pfNuevo)
    {
        fclose(pfOrigen);
        fclose(pfNuevo);
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Leo el tamaño del encabezadoBMP:
    char tamEncabezado;
    fseek(pfOrigen, 14, SEEK_SET);
    fread(&tamEncabezado, 4, 1, pfOrigen);

// Declaro una variable para almacenar el encabezado y luego escribirlo en el nuevo archivo
    char encabezadoBMP[tamEncabezado];
    fseek(pfOrigen, 0, SEEK_SET);
    fread(&encabezadoBMP, tamEncabezado, 1, pfOrigen);
    fwrite(encabezadoBMP, tamEncabezado, 1, pfNuevo);

// Guardo los datos en la estructura 'infoEncabezado':
    t_metadata infoEncabezado;

    memcpy(&infoEncabezado.tamArchivo, &encabezadoBMP[2], 4);
    memcpy(&infoEncabezado.tamEncabezado, &encabezadoBMP[14], 4);
    memcpy(&infoEncabezado.comienzoImagen, &encabezadoBMP[10], 4);
    memcpy(&infoEncabezado.ancho, &encabezadoBMP[18], 4);
    memcpy(&infoEncabezado.alto, &encabezadoBMP[22], 4);
    memcpy(&infoEncabezado.profundidad, &encabezadoBMP[2], 2);
// Puedo usar la variable 'tamImagen' o hacer directamente 'tamArchivo' - 'comienzoImagen'
    unsigned int tamImagen;
    memcpy(&tamImagen, &encabezadoBMP[34], 4);

    int i;
    unsigned char byte; // unsigned byte = 1 byte -> 0-255
    unsigned char nuevoByte;
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < tamImagen; i++)
    {
        fread(&byte, 1,1, pfOrigen);
        nuevoByte = byte;
        nuevoByte = 1.25 * (nuevoByte - 128) + 128;
        fwrite(&nuevoByte, 1,1, pfNuevo);
        //byte = byte > 255 ? 255 : byte < 0 ? 0 : byte;


//        if(nuevoByte > 255)
//        {
//            nuevoByte = 255;
//        }
//        if(nuevoByte < 0)
//        {
//            nuevoByte = 0;
//        }
        //fwrite(&nuevoByte, 1,1, pfNuevo);
    }

    printf("Se invoco la funcion 'aumentarContraste'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);
    return TODO_OK;
}

int reducirContraste(const char* nombreArchivo)
{
    printf("Se invoco la funcion 'reducirContraste'.\n");
    return TODO_OK;
}

int tonalidadAzul(const char* nombreArchivo)
{
    printf("Se invoco la funcion 'tonalidadAzul'.\n");
    return TODO_OK;
}

int tonalidadVerde(const char* nombreArchivo)
{
    printf("Se invoco la funcion 'tonalidadVerde'.\n");
    return TODO_OK;
}

int tonalidadRoja(const char* nombreArchivo)
{
    printf("Se invoco la funcion 'tonalidadRoja'.\n");
    return TODO_OK;
}

int recortar(const char* nombreArchivo)
{
    printf("Se invoco la funcion 'recortar'.\n");
    return TODO_OK;
}

int rotarDerecha(const char* nombreArchivo)
{
    printf("Se invoco la funcion 'rotarDerecha'.\n");
    return TODO_OK;
}

int rotarIzquierda(const char* nombreArchivo)
{
    printf("Se invoco la funcion 'rotarIzquierda'.\n");
    return TODO_OK;
}

int comodin(const char* nombreArchivo) // REEMPLAZAR
{
    printf("Se invoco la funcion 'comodin'.\n");
    return TODO_OK;
}
