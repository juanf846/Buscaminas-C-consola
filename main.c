#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include "partida.h"
#include "utils.h"
#include "datos.h"
#include "estadisticas.h"

void mostrarMenu();

const char* NOMBRE_ARCHIVO_TITULO = "titulo ASCII.txt";

int main()
{
    char entrada;
    cargarEstadisticas();
    do{
        system("cls");
        mostrarMenu();
        entrada = getch();
        switch(entrada){
            case '1': //abrir nueva partida
                partida_main(false);
                break;
            case '2':
                partida_main(true);
                break;
            case '3': //abrir estadisticas
                estadisticas_main();
                break;
        }

    }while(entrada!='q');
    return 0;
}

void mostrarMenu(){
    FILE *fptr;
    fptr = fopen(NOMBRE_ARCHIVO_TITULO, "r");
    if (fptr == NULL)
    {
        printf("Error, no se pudo abrir el archivo \"titulo ASCII.txt\"\n");
        system("pause");
        exit(-404);
    }
    printf("\n\n\n\n\n");

    char ch = fgetc(fptr);
    int nuevaLinea = true;
    while (ch != EOF)
    {
        if(nuevaLinea==true){
            //imprime margen a la izquierda
            for(int i=0;i<10;i++){
                printf(" ");
            }
            nuevaLinea=false;
        }
        if(ch == '\n'){
            nuevaLinea=true;
        }
        printf ("%c", ch);
        ch = fgetc(fptr);
    }
    fclose(fptr);

    for(int i=0;i<5;i++){
        printf("\n");
    }
    center_printf("1. Jugar\n\n");

    center_printf("2. Cargar partida\n\n");

    center_printf("3. Estadisticas\n\n");

    center_printf("Q. Salir\n\n");
}
