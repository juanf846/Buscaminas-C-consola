#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


const char* nombreArchivoTitulo = "titulo ASCII.txt";
int main()
{
    char entrada;
    do{
        system("cls");
        mostrarMenu();
        entrada = getch();
        switch(entrada){
            case '1': //abrir nueva partida
            case '2': break;//abrir estadisticas
        }

    }while(entrada!='3');
    return 0;
}

void mostrarMenu(){
    FILE *fptr;
    fptr = fopen(nombreArchivoTitulo, "r");
    if (fptr == NULL)
    {
        printf("Error, no se pudo abrir el archivo \"titulo ASCII.txt\"\n");
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
    for(int i=0;i<50;i++){
        printf(" ");
    }
    printf("1. Jugar\n\n");

    for(int i=0;i<46;i++){
        printf(" ");
    }
    printf("2. Estadisticas\n\n");

    for(int i=0;i<50;i++){
        printf(" ");
    }
    printf("3. Salir\n\n");
    printf("\n\n\n");
    for(int i=0;i<45;i++){
        printf(" ");
    }
    printf("Elija una opcion: ");
}
