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
            if(funcionActual) // Encontre la funcion
            {
                if(funcionActual(argv[indiceArchivoBMP])) // LLamo a la funcion pasando el path del archivo y en base a lo que devuelva imprimo en pantalla.
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
    unsigned char byte; // Cada byte es la componente R,G o B de un pixel. En este caso me es indistinto, todos van a ser modificados de la misma forma
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
    float promedio;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < tamImagen; i+=3)
    {
        fread(pixel, 3,1, pfOrigen);
        promedio = (pixel[0] + pixel[1] + pixel[2]) / 3;
        if(promedio > 127)
        {
            pixel[0] = pixel[0] * 1.25 > 255 ? 255 : pixel[0] * 1.25;
            pixel[1] = pixel[1] * 1.25 > 255 ? 255 : pixel[1] * 1.25;
            pixel[2] = pixel[2] * 1.25 > 255 ? 255 : pixel[2] * 1.25;
        }
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'aumentarContraste'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int reducirContraste(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_reducir_contraste.bmp", "wb");

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
        if(promedio < 127)
        {
            pixel[0] *= 0.75;
            pixel[1] *= 0.75;
            pixel[2] *= 0.75;
        }
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'reducirContraste'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int tonalidadAzul(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_tonalidad_azul.bmp", "wb");

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
//    int nuevoAzul;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < tamImagen; i+=3)
    {
        fread(pixel, 3,1, pfOrigen);
//        nuevoAzul = 255/2;
//        if(nuevoAzul > 255)
//        {
//            pixel[0] = 255;
//        }
//        else
//        {
//            pixel[0] = nuevoAzul;
//        }
//        pixel[0] = nuevoAzul;
//        pixel[1] *= 0.7;
//        pixel[2] *= 0.7;

// OTRA FORMA
        pixel[0] = pixel[0] * 1.50 > 255 ? 255 : pixel[0] * 1.50;

        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'tonalidadAzul'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int tonalidadVerde(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_tonalidad_verde.bmp", "wb");

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
//    int nuevoVerde;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < tamImagen; i+=3)
    {
        fread(pixel, 3,1, pfOrigen);
////        nuevoVerde *= 1.5;
////        if(pixel[1] >= 255)
////        {
////            pixel[1] = 255;
////        }
//        pixel[0] *= 0.7;
////        pixel[1] *= 1.5;
//        pixel[2] *= 0.7;

// OTRA FORMA
        pixel[1] = pixel[1] * 1.50 > 255 ? 255 : pixel[1] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'tonalidadVerde'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int tonalidadRoja(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_tonalidad_roja.bmp", "wb");

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
//    int nuevoRojo;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < tamImagen; i+=3)
    {
        fread(pixel, 3,1, pfOrigen);
////        nuevoRojo *= 1.5;
////        if(pixel[2] >= 255)
////        {
////            pixel[2] = 255;
////        }
//        pixel[0] *= 0.7;
//        pixel[1] *= 0.7;
////        pixel[2] *= 0.7;
// OTRA FORMA
        pixel[2] = pixel[2] * 1.50 > 255 ? 255 : pixel[2] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'tonalidadRoja'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int recortar(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_recortar.bmp", "wb");

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

    int i, j;
    unsigned char byte;
    unsigned int nuevoAlto = infoEncabezado.alto * 0.5 ;
    unsigned int nuevoAncho = infoEncabezado.ancho * 0.5;
//    int bytesEscritos = 0;
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < nuevoAlto*3; i++)
    {
        fseek(pfOrigen, infoEncabezado.comienzoImagen + i*infoEncabezado.ancho*3, SEEK_SET);
        for(j = 0; j < nuevoAncho*3; j++)
        {
            fread(&byte, 1,1, pfOrigen);
            fwrite(&byte, 1,1, pfNuevo);
//            bytesEscritos++;
        }
    }

// Modifico los datos del encabezado
    infoEncabezado.tamArchivo = nuevoAlto*nuevoAncho*3 + infoEncabezado.comienzoImagen;
    infoEncabezado.ancho = nuevoAncho;
    infoEncabezado.alto = nuevoAlto;
    tamImagen = nuevoAlto*nuevoAncho*3;
// Ahora escribo en el archivo
    fseek(pfNuevo, 2, SEEK_SET); // Tam archivo
    fwrite(&infoEncabezado.tamArchivo, 4, 1, pfNuevo);
    fseek(pfNuevo, 18, SEEK_SET); // Ancho
    fwrite(&infoEncabezado.ancho, 4, 1, pfNuevo);
    fseek(pfNuevo, 22, SEEK_SET); // Alto
    fwrite(&infoEncabezado.alto, 4, 1, pfNuevo);
    fseek(pfNuevo, 34, SEEK_SET); // Tamaño imagen
    fwrite(&infoEncabezado.ancho, 4, 1, pfNuevo);


    printf("Se invoco la funcion 'recortar'.\n");
//    printf("Pixeles escritos en la imagen recortada: %d\n", bytesEscritos);

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int rotarDerecha(const char* nombreArchivo)
{
    FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_rotar_derecha.bmp", "wb");

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

    int i, j;
    unsigned int nuevoAlto = infoEncabezado.ancho;
    unsigned int nuevoAncho = infoEncabezado.alto;
    t_pixel pixel;
    t_pixel mapaDePixeles[infoEncabezado.alto][infoEncabezado.ancho];
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.alto; i++)
    {
        for(j = 0; j < infoEncabezado.ancho; j++)
        {
           fread(pixel.pixel, 3, 1, pfOrigen);
           mapaDePixeles[i][j] = pixel;
        }
    }

    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.ancho; i++) // Columna
    {
        for(j = 0; j < infoEncabezado.alto; j++) // Fila
        {
           pixel = mapaDePixeles[j][infoEncabezado.ancho - 1 - i];
           fwrite(pixel.pixel, 3, 1, pfNuevo);
        }
    }

// Modifico los datos del encabezado
    infoEncabezado.ancho = nuevoAncho;
    infoEncabezado.alto = nuevoAlto;
    fseek(pfNuevo, 18, SEEK_SET); // Ancho
    fwrite(&infoEncabezado.ancho, 4, 1, pfNuevo);
    fseek(pfNuevo, 22, SEEK_SET); // Alto
    fwrite(&infoEncabezado.alto, 4, 1, pfNuevo);

    printf("Se invoco la funcion 'rotarDerecha'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int rotarIzquierda(const char* nombreArchivo)
{
        FILE* pfOrigen = fopen(nombreArchivo, "rb");
    FILE* pfNuevo = fopen("estudiante_rotar_izquierda.bmp", "wb");

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

    int i, j;
    unsigned int nuevoAlto = infoEncabezado.ancho;
    unsigned int nuevoAncho = infoEncabezado.alto;
    t_pixel pixel;
    t_pixel mapaDePixeles[infoEncabezado.alto][infoEncabezado.ancho];
    fseek(pfOrigen, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.alto; i++)
    {
        for(j = 0; j < infoEncabezado.ancho; j++)
        {
           fread(pixel.pixel, 3, 1, pfOrigen);
           mapaDePixeles[i][j] = pixel;
        }
    }

    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.ancho; i++)
    {
        for(j = 0; j < infoEncabezado.alto; j++)
        {
           pixel = mapaDePixeles[infoEncabezado.alto - j - 1][i];
           fwrite(pixel.pixel, 3, 1, pfNuevo);
        }
    }

// Modifico los datos del encabezado
    infoEncabezado.ancho = nuevoAncho;
    infoEncabezado.alto = nuevoAlto;
    fseek(pfNuevo, 18, SEEK_SET); // Ancho
    fwrite(&infoEncabezado.ancho, 4, 1, pfNuevo);
    fseek(pfNuevo, 22, SEEK_SET); // Alto
    fwrite(&infoEncabezado.alto, 4, 1, pfNuevo);

    printf("Se invoco la funcion 'rotarIzquierda'.\n");

    fclose(pfOrigen);
    fclose(pfNuevo);

    return TODO_OK;
}

int comodin(const char* nombreArchivo) // REEMPLAZAR
{
    printf("Se invoco la funcion 'comodin'.\n");
    return TODO_OK;
}
