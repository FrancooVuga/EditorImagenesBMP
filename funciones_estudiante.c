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
// Primero busco el indice del archivo .bmp, por si no es el ultimo parametro:
    int i;
    int indiceArchivoBMP = ARCHIVO_NO_ESPECIFICADO;

    for(i = 1; i < argc; i++)
    {
        if(strstr(argv[i], ".bmp"))
        {
            indiceArchivoBMP = i;
        }
    }

// Ahora proceso todos los argumentos si se encontró el .bmp:
    if(indiceArchivoBMP != ARCHIVO_NO_ESPECIFICADO)
    {
        FILE* pfOriginal = fopen(argv[indiceArchivoBMP], "rb");
        if(!pfOriginal)
        {
            printf("No se pudo abrir el archivo '%s'.\n", argv[indiceArchivoBMP]);
            return ARCHIVO_NO_ENCONTRADO;
        }
       // Ahora por cada argumento a main, llamo a la respectiva funcion
        int j;
        char nombreNuevoArchivo[40] = {"estudiante_"};
        t_funcion funcionActual;
        for(j = 1; j < argc; j++)
        {
            funcionActual = buscarFuncion(argv[j]);
            if(funcionActual.funcion) // Encontre la funcion
            {
                strcat(nombreNuevoArchivo , &funcionActual.nombre[2]);
                strcat(nombreNuevoArchivo, ".bmp\0");
                fseek(pfOriginal, 0, SEEK_SET);
                if(funcionActual.funcion(pfOriginal, nombreNuevoArchivo)) // LLamo a la funcion pasando el path del archivo y en base a lo que devuelva imprimo en pantalla.
                {
                    printf("No se pudo hacer la operacion '%s' sobre la imagen.\n", argv[j]);

                }
                nombreNuevoArchivo[11] = '\0'; // "estudiante_\0"
            }
            else if(j != indiceArchivoBMP)
            {
                printf("La funcion '%s' no es valida.\n", argv[j]);
            }
        }
        fclose(pfOriginal);
    }
    else
    {
        printf("No se especifico un archivo '.bmp'.\n");
        return ARCHIVO_NO_ENCONTRADO;
    }

    return TODO_OK;
}

t_funcion buscarFuncion(const char* nombreFuncion)
{
    // Donde deberia estar declarado?
    static t_funcion tablaFunciones[] ={{"--negativo", negativo},
                                        {"--escala-de-grises", escalaDeGrises},
                                        {"--aumentar-contraste", aumentarContraste},
                                        {"--reducir-contraste", reducirContraste},
                                        {"--tonalidad-azul", tonalidadAzul},
                                        {"--tonalidad-verde", tonalidadVerde},
                                        {"--tonalidad-roja", tonalidadRoja},
                                        {"--recortar", recortar},
                                        {"--rotar-derecha", rotarDerecha},
                                        {"--rotar-izquierda", rotarIzquierda},
                                        {"--espejar", espejar},
                                        {NULL, NULL}};


    t_funcion* f;
    for(f = tablaFunciones; f->nombre != NULL; f++)
    {
        if(strcmp(nombreFuncion, f->nombre) == 0)
        {
            return *f;
        }
    }
    f->funcion = NULL;
    return *f;
}

int negativo(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el nuevo archivo
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);


// Ahora muevo ambos punteros de archivo al comienzo de la imagen
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);


    unsigned char byte; // Cada byte es la componente R,G o B de un pixel. En este caso me es indistinto, todos van a ser modificados de la misma forma
    while(fread(&byte, 1, 1, pfOriginal))
    {
        byte = ~byte;
        fwrite(&byte, 1,1, pfNuevo);
    }

    printf("Se invoco la funcion 'negativo'.\n");

    fclose(pfNuevo);

    return TODO_OK;
}

int escalaDeGrises(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el nuevo archivo
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);


// Ahora muevo ambos punteros de archivo al comienzo de la imagen
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    float promedio;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
        promedio = (pixel[0] + pixel[1] + pixel[2]) / 3;
        pixel[0] = pixel[1] = pixel[2] = promedio;
        fwrite(pixel, sizeof(pixel),1, pfNuevo);
    }

    printf("Se invoco la funcion 'escalaDeGrises'.\n");

    fclose(pfNuevo);

    return TODO_OK;
}

int aumentarContraste(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el nuevo archivo
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);


// Ahora muevo ambos punteros de archivo al comienzo de la imagen
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    float promedio;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
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

    fclose(pfNuevo);

    return TODO_OK;
}

int reducirContraste(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el nuevo archivo
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);


// Ahora muevo ambos punteros de archivo al comienzo de la imagen
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    float promedio;
    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
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

    fclose(pfNuevo);

    return TODO_OK;
}

int tonalidadAzul(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el nuevo archivo
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);


// Ahora muevo ambos punteros de archivo al comienzo de la imagen
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
        pixel[0] = pixel[0] * 1.50 > 255 ? 255 : pixel[0] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'tonalidadAzul'.\n");

    fclose(pfNuevo);

    return TODO_OK;
}

int tonalidadVerde(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el nuevo archivo
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);


// Ahora muevo ambos punteros de archivo al comienzo de la imagen
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
        pixel[1] = pixel[1] * 1.50 > 255 ? 255 : pixel[1] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'tonalidadVerde'.\n");

    fclose(pfNuevo);

    return TODO_OK;
}

int tonalidadRoja(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el nuevo archivo
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);


// Ahora muevo ambos punteros de archivo al comienzo de la imagen
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
        pixel[2] = pixel[2] * 1.50 > 255 ? 255 : pixel[2] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

    printf("Se invoco la funcion 'tonalidadRoja'.\n");

    fclose(pfNuevo);

    return TODO_OK;
}

int recortar(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Leo el tamaño del encabezadoBMP:
    char tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, 4, 1, pfOriginal);

// Declaro una variable para almacenar el encabezado y luego escribirlo en el nuevo archivo
    char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(&encabezadoBMP, tamEncabezado, 1, pfOriginal);
    fwrite(encabezadoBMP, tamEncabezado, 1, pfNuevo);


// Guardo los datos en la estructura 'infoEncabezado':
    t_metadata infoEncabezado;

    memcpy(&infoEncabezado.tamArchivo, &encabezadoBMP[2], 4);
    memcpy(&infoEncabezado.tamEncabezado, &encabezadoBMP[14], 4);
    memcpy(&infoEncabezado.comienzoImagen, &encabezadoBMP[10], 4);
    memcpy(&infoEncabezado.ancho, &encabezadoBMP[18], 4);
    memcpy(&infoEncabezado.alto, &encabezadoBMP[22], 4);
    memcpy(&infoEncabezado.profundidad, &encabezadoBMP[2], 2);
    unsigned int tamImagen;
    memcpy(&tamImagen, &encabezadoBMP[34], 4);

    int i, j;
    unsigned char byte;
    unsigned int nuevoAlto = infoEncabezado.alto * 0.5 ;
    unsigned int nuevoAncho = infoEncabezado.ancho * 0.5;
    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    fseek(pfOriginal, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < nuevoAlto*3; i++)
    {
        fseek(pfOriginal, infoEncabezado.comienzoImagen + i*infoEncabezado.ancho*3, SEEK_SET);
        for(j = 0; j < nuevoAncho*3; j++)
        {
            fread(&byte, 1,1, pfOriginal);
            fwrite(&byte, 1,1, pfNuevo);
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

// OTRA FORMA
//    unsigned char filaCompleta[infoEncabezado.ancho*3];
//    unsigned char pixelVacio[3] = {0,0,0};
//    unsigned char pixel[3];
//    for(i = infoEncabezado.alto; i > nuevoAlto; i--)
//    {
//        fread(filaCompleta, sizeof(filaCompleta), 1, pfOriginal);
//        fwrite(filaCompleta, nuevoAncho*3, 1, pfNuevo);
//        for(int byte = 0; byte < nuevoAncho*3; byte++)
//        {
//            fwrite(&pixelVacio[0], 1, 1, pfNuevo);
//        }
//    }
//
//    while(fread(pixel, 3, 1, pfOriginal))
//    {
//        fwrite(pixelVacio, sizeof(pixelVacio), 1, pfNuevo);
//    }

    printf("Se invoco la funcion 'recortar'.\n");

    fclose(pfNuevo);

    return TODO_OK;
}

int rotarDerecha(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }
// Leo el tamaño del encabezadoBMP:
    char tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, 4, 1, pfOriginal);

// Declaro una variable para almacenar el encabezado y luego escribirlo en el nuevo archivo
    char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(&encabezadoBMP, tamEncabezado, 1, pfOriginal);
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
    fseek(pfOriginal, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.alto; i++)
    {
        for(j = 0; j < infoEncabezado.ancho; j++)
        {
           fread(pixel.pixel, 3, 1, pfOriginal);
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

    fclose(pfNuevo);

    return TODO_OK;
}

int rotarIzquierda(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Leo el tamaño del encabezadoBMP:
    char tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, 4, 1, pfOriginal);

// Declaro una variable para almacenar el encabezado y luego escribirlo en el nuevo archivo
    char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(&encabezadoBMP, tamEncabezado, 1, pfOriginal);
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
    fseek(pfOriginal, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.alto; i++)
    {
        for(j = 0; j < infoEncabezado.ancho; j++)
        {
           fread(pixel.pixel, 3, 1, pfOriginal);
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

    fclose(pfNuevo);

    return TODO_OK;
}

int espejar(FILE* pfOriginal, const char* nombreNuevoArchivo) // REEMPLAZAR
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Leo el tamaño del encabezadoBMP:
    char tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, 4, 1, pfOriginal);

// Declaro una variable para almacenar el encabezado y luego escribirlo en el nuevo archivo
    char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fread(&encabezadoBMP, tamEncabezado, 1, pfOriginal);
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
    t_pixel pixel;
    t_pixel mapaDePixeles[infoEncabezado.alto][infoEncabezado.ancho];
    fseek(pfOriginal, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.alto; i++)
    {
        for(j = 0; j < infoEncabezado.ancho; j++)
        {
           fread(pixel.pixel, 3, 1, pfOriginal);
           mapaDePixeles[i][j] = pixel;
        }
    }

    fseek(pfNuevo, infoEncabezado.comienzoImagen, SEEK_SET);
    for(i = 0; i < infoEncabezado.alto; i++)
    {
        for(j = infoEncabezado.ancho - 1; j >= 0; j--)
        {
           pixel = mapaDePixeles[i][j];
           fwrite(pixel.pixel, 3, 1, pfNuevo);
        }
    }

    printf("Se invoco la funcion 'espejar'.\n");

    fclose(pfNuevo);
    return TODO_OK;
}
