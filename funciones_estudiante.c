/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Vuga
    Nombre: Franco
    DNI: 42724342
    Entrega: 8/5/2024
    -----------------
    Apellido: Quinteros
    Nombre: Enzo
    DNI: 43863082
    Entrega: 8/5/2024
    -----------------
    (Sólo para grupos de tres integrantes)
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

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
        t_funcion* funcionActual;
        for(j = 1; j < argc; j++)
        {
            funcionActual = buscarFuncion(argv[j]);
            if(funcionActual->funcion) // Encontre la funcion
            {
                if(funcionActual->invocado == false) // No se aplico el efecto todavia
                {
                    strcat(nombreNuevoArchivo , &funcionActual->nombre[2]);
                    strcat(nombreNuevoArchivo, ".bmp\0");
                    fseek(pfOriginal, 0, SEEK_SET);
                    if(funcionActual->funcion(pfOriginal, nombreNuevoArchivo)) // LLamo a la funcion pasando el path del archivo y en base a lo que devuelva imprimo en pantalla.
                    {
                        printf("No se pudo hacer la operacion '%s' sobre la imagen.\n", funcionActual->nombre);
                    }
                    else
                    {
                        printf("Se aplico el efecto '%s' a la imagen.\n", &funcionActual->nombre[2]);
                    }
                    funcionActual->invocado = true;
                    nombreNuevoArchivo[11] = '\0'; // "estudiante_\0"
                }
                else // funcionActual->invocado == false ( No aplico denuevo el efecto)
                {
                    printf("Ya se aplico el efecto '%s' sobre la imagen.\n", &funcionActual->nombre[2]);
                }

            }
            else if(j != indiceArchivoBMP)
            {
                printf("La funcionalidad '%s' no es valida.\n", argv[j]+2);
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

t_funcion* buscarFuncion(const char* nombreFuncion)
{
    static t_funcion tablaFunciones[] ={{"--negativo", negativo, false},
                                        {"--escala-de-grises", escalaDeGrises, false},
                                        {"--aumentar-contraste", aumentarContraste, false},
                                        {"--reducir-contraste", reducirContraste, false},
                                        {"--tonalidad-azul", tonalidadAzul, false},
                                        {"--tonalidad-verde", tonalidadVerde, false},
                                        {"--tonalidad-roja", tonalidadRoja, false},
                                        {"--recortar", recortar, false},
                                        {"--rotar-derecha", rotarDerecha, false},
                                        {"--rotar-izquierda", rotarIzquierda, false},
                                        {"--comodin", comodin, false},
                                        {NULL, NULL}};


    t_funcion* f;
    for(f = tablaFunciones; f->nombre != NULL; f++)
    {
        if(strcmp(nombreFuncion, f->nombre) == 0)
        {
            return f;
        }
    }
    f->funcion = NULL;
    return f;
}

void** crearMatriz(int fil, int col, int tamElem)
{
    void** matriz = malloc(sizeof(void*)*fil);

    for(void* fila = *matriz; fila < *matriz+fil-1; fila++)
    {
        fila = malloc(col*tamElem);
        if(fila == NULL) // No se pudo reservar memoria
        {
            for(void *filE = fila; filE > *matriz; filE--)
            {
                free(filE);
            }
        }
    }

    return matriz;
}

void copiarEncabezadoEnArchivo(FILE* pfOriginal, FILE* pfNuevo)
{
// Busco tamaño del encabezado:
    unsigned int tamEncabezado;
    fseek(pfOriginal, 14, SEEK_SET);
    fread(&tamEncabezado, sizeof(tamEncabezado), 1, pfOriginal);

// Leo y almaceno el encabezado del archivo original para escribirlo en el archivo nuevo:
    unsigned char encabezadoBMP[tamEncabezado];
    fseek(pfOriginal, 0, SEEK_SET);
    fseek(pfNuevo, 0, SEEK_SET);
    fread(encabezadoBMP, sizeof(encabezadoBMP), 1, pfOriginal);
    fwrite(encabezadoBMP, sizeof(encabezadoBMP), 1, pfNuevo);
}

void cargarEncabezadoBMP(FILE* pfArchivo, t_metadata* infoEncabezado)
{
    fseek(pfArchivo, 2, SEEK_SET);
    fread(&infoEncabezado->tamArchivo, 4, 1, pfArchivo);
    fseek(pfArchivo, 10, SEEK_SET);
    fread(&infoEncabezado->comienzoImagen, 4, 1, pfArchivo);
    fseek(pfArchivo, 14, SEEK_SET);
    fread(&infoEncabezado->tamEncabezado, 4, 1, pfArchivo);
    fseek(pfArchivo, 18, SEEK_SET);
    fread(&infoEncabezado->ancho, 4, 1, pfArchivo);
    fseek(pfArchivo, 22, SEEK_SET);
    fread(&infoEncabezado->alto, 4, 1, pfArchivo);
}

int negativo(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);

// Ahora muevo ambos punteros de archivo al comienzo de la imagen:
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    unsigned char byte; // Cada byte es la componente R,G o B de un pixel. En este caso me es indistinto, todos van a ser modificados de la misma forma
    while(fread(&byte, 1, 1, pfOriginal))
    {
        byte = ~byte;
        fwrite(&byte, 1,1, pfNuevo);
    }

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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);

// Ahora muevo ambos punteros de archivo al comienzo de la imagen:
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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);

// Ahora muevo ambos punteros de archivo al comienzo de la imagen:
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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);

// Ahora muevo ambos punteros de archivo al comienzo de la imagen:
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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);

// Ahora muevo ambos punteros de archivo al comienzo de la imagen:
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
        pixel[0] = pixel[0] * 1.50 > 255 ? 255 : pixel[0] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);

// Ahora muevo ambos punteros de archivo al comienzo de la imagen:
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
        pixel[1] = pixel[1] * 1.50 > 255 ? 255 : pixel[1] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Busco el comienzo de la imagen:
    unsigned int comienzoImagen;
    fseek(pfOriginal, 10, SEEK_SET);
    fread(&comienzoImagen, sizeof(comienzoImagen), 1, pfOriginal);

// Ahora muevo ambos punteros de archivo al comienzo de la imagen:
    fseek(pfOriginal, comienzoImagen, SEEK_SET);
    fseek(pfNuevo, comienzoImagen, SEEK_SET);

    unsigned char pixel[3]; // unsigned byte = 1 byte -> 0-255
    while(fread(pixel, sizeof(pixel), 1, pfOriginal))
    {
        pixel[2] = pixel[2] * 1.50 > 255 ? 255 : pixel[2] * 1.50;
        fwrite(pixel, 3,1, pfNuevo);
    }

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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Cargo el encabezado en una estructura:
    t_metadata infoEncabezado;
    cargarEncabezadoBMP(pfOriginal, &infoEncabezado);

//Necesito el tamaño de imagen para modificarlo, el cual no esta en la estructura "t_metadata":
    unsigned int tamImagen;
    fseek(pfOriginal, 34, SEEK_SET);
    fread(&tamImagen, 4, 1, pfOriginal);

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

// Modifico los datos del encabezado:
    infoEncabezado.tamArchivo = nuevoAlto*nuevoAncho*3 + infoEncabezado.comienzoImagen;
    infoEncabezado.ancho = nuevoAncho;
    infoEncabezado.alto = nuevoAlto;
    tamImagen = nuevoAlto*nuevoAncho*3;

// Ahora escribo en el archivo nuevo:
    fseek(pfNuevo, 2, SEEK_SET); // Tam archivo
    fwrite(&infoEncabezado.tamArchivo, 4, 1, pfNuevo);
    fseek(pfNuevo, 18, SEEK_SET); // Ancho
    fwrite(&infoEncabezado.ancho, 4, 1, pfNuevo);
    fseek(pfNuevo, 22, SEEK_SET); // Alto
    fwrite(&infoEncabezado.alto, 4, 1, pfNuevo);
    fseek(pfNuevo, 34, SEEK_SET); // Tamaño imagen
    fwrite(&tamImagen, 4, 1, pfNuevo);

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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Cargo el encabezado en una estructura:
    t_metadata infoEncabezado;
    cargarEncabezadoBMP(pfOriginal, &infoEncabezado);

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

// Modifico y escribo los datos del encabezado:
    infoEncabezado.ancho = nuevoAncho;
    infoEncabezado.alto = nuevoAlto;
    fseek(pfNuevo, 18, SEEK_SET); // Ancho
    fwrite(&infoEncabezado.ancho, 4, 1, pfNuevo);
    fseek(pfNuevo, 22, SEEK_SET); // Alto
    fwrite(&infoEncabezado.alto, 4, 1, pfNuevo);

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

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Cargo el encabezado en una estructura:
    t_metadata infoEncabezado;
    cargarEncabezadoBMP(pfOriginal, &infoEncabezado);

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

// Modifico y escribo los datos del encabezado:
    infoEncabezado.ancho = nuevoAncho;
    infoEncabezado.alto = nuevoAlto;
    fseek(pfNuevo, 18, SEEK_SET); // Ancho
    fwrite(&infoEncabezado.ancho, 4, 1, pfNuevo);
    fseek(pfNuevo, 22, SEEK_SET); // Alto
    fwrite(&infoEncabezado.alto, 4, 1, pfNuevo);

    fclose(pfNuevo);

    return TODO_OK;
}

int comodin(FILE* pfOriginal, const char* nombreNuevoArchivo)
{
    // Esta funcion invierte la imagen haciendo como un efecto "espejo"

    FILE* pfNuevo = fopen(nombreNuevoArchivo, "wb");

    if(!pfNuevo)
    {
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

// Copio el encabezado del arhivo original en el nuevo:
    copiarEncabezadoEnArchivo(pfOriginal, pfNuevo);

// Cargo el encabezado en una estructura:
    t_metadata infoEncabezado;
    cargarEncabezadoBMP(pfOriginal, &infoEncabezado);

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

    fclose(pfNuevo);

    return TODO_OK;
}
