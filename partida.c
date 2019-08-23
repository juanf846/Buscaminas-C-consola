#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <conio.h>
#include "utils.h"
#include "partida.h"
#include "main.h"

#define ESTADO_ESPERANDO 0
#define ESTADO_POSICION 1
#define ESTADO_MENU 2
#define ESTADO_PERDIO 3
#define ESTADO_GANO 4

#define CARACTER_CUADRADO 254

int charToPosicion(char);
void mostrarUI();
void mostrarMenuPartida();
void generar_mapa();
void destaparCelda(int,int);

int MAXIMO = 5;
int semilla = 0;
int minas = 2;
int destapadas;
char** mapa;
bool** mapaOculta;
bool continuar;
char estado;
char antChar;
/*Estados:
ESPERANDO(0) = Se puede ingresar posicion o ir al menu
POSICION(1) = Se ingreso el primer caracter de la posicion
MENU(2) = Se abrio el menu
TERMINADO(3) = El jugador perdió o ganó
*/
int charToPosicion(char c){
    int a = -1;
    if((c >= '0' && c <= '9')){
        a = c-'0';
    }else if((c >= 'a' && c <= 'k')){
        a = c-'a'+10;
    }
    if(a>=MAXIMO){
        a = -1;
    }
    return a;
}
void partida_main()
{
    estado = ESTADO_ESPERANDO;
    antChar = 0;
    continuar = true;
    destapadas = 0;
    mapa = malloc(sizeof(char*)*MAXIMO);
    mapaOculta = malloc(sizeof(bool*)*MAXIMO);
    for(int i=0;i<MAXIMO;i++){
        mapa[i] = malloc(sizeof(char)*MAXIMO);
        mapaOculta[i] = malloc(sizeof(bool)*MAXIMO);
    }
    struct timeval time;
    gettimeofday(&time,NULL);
    //semilla = time.tv_sec;

    generar_mapa();
    while(continuar){
        char c;
        switch(estado){
            case ESTADO_ESPERANDO:
                mostrarUI();
                center_printf("Ingrese la fila\n");
                center_printf("Aprete ESC para abrir el menu\n");
                printf("%i",destapadas);
                printf("%i",MAXIMO*MAXIMO-minas);

                c = getch();
                if(charToPosicion(c)!=-1){
                    antChar = c;
                    estado = ESTADO_POSICION;
                }else if(c == 27){
                    estado = ESTADO_MENU;
                }else{
                    //system("CLS");
                    center_printf("\nERROR: Valor no valido\n");
                    system("PAUSE");
                }
                break;
            case ESTADO_POSICION:
                mostrarUI();
                center_printf("Ingrese la columna:\n");
                center_printf("Aprete ESC para cancelar");
                c = getch();
                if(charToPosicion(c)!=-1){
                    //Funcion destapar
                    int fila = charToPosicion(antChar);
                    int columna = charToPosicion(c);
                    destaparCelda(fila,columna);
                    antChar = 0;
                    if(mapa[fila][columna]==9){
                        //perdió
                        mapa[fila][columna] = 10;
                        for(int i=0;i<MAXIMO;i++)
                            for(int j=0;j<MAXIMO;j++)
                                if(mapa[i][j]==9)
                                    mapaOculta[i][j]=false;
                        estado = ESTADO_PERDIO;
                        break;
                    }
                    if(destapadas == MAXIMO*MAXIMO-minas){
                        //Ganó
                        printf("gano");
                        estado = ESTADO_GANO;
                        break;
                    }
                    estado = ESTADO_ESPERANDO;
                }else if(c == 27){
                    antChar = 0;
                    estado = ESTADO_ESPERANDO;
                }else{
                    system("CLS");
                    center_printf("\nERROR: Valor no valido\n");
                    system("PAUSE");
                }
                break;
            case ESTADO_MENU:
                mostrarMenuPartida();
                c = getch();
                switch(c){
                    case '1':
                        estado = ESTADO_ESPERANDO;
                        break;
                    case '2':
                        //TODO guardado de partida
                    case '3':
                        return;
                }
                break;

            case ESTADO_PERDIO:
                mostrarUI();
                printf("\n\n");
                center_printf("PERDISTE");
                printf("\n");
                getch();

                return;
            case ESTADO_GANO:
                mostrarUI();
                printf("\n\n");
                center_printf("GANASTE!!!!");
                printf("\n");
                getch();

                return;
        }
    }
    system("pause");
    return;
}

void generar_mapa(){
    srand(semilla);
    //pone todo la matriz a 0
    for(int i=0;i<MAXIMO;i++){
        for(int j=0;j<MAXIMO;j++){
            mapa[i][j] = 0;
            mapaOculta[i][j] = true;
        }
    }
    //Agrega las minas
    for(int i=0;i<minas;i++){
        int x;
        int y;
        do{
            x = rand() % MAXIMO;
            y = rand() % MAXIMO;
        }while(mapa[x][y]==9);
        mapa[x][y] = 9;
        //agrega 1 a todas las celdas pegadas
        for(int x1 = -1; x1 < 2;x1++){
            for(int y1 = -1; y1 < 2;y1++){
                if(x+x1>-1 && x+x1<MAXIMO && y+y1>-1 && y+y1<MAXIMO && mapa[x+x1][y+y1] != 9){
                    mapa[x+x1][y+y1] += 1;
                }
            }
        }
    }
}
void mostrarMenuPartida(){
    system("CLS");
    for(int i=0;i<PANTALLA_ALTO/2-9/2;i++){
        printf("\n");
    }
    center_printf("1. Continuar");
    printf("\n\n");
    center_printf("2. Guardar partida");
    printf("\n\n");
    center_printf("3. Salir sin guardar");
    printf("\n\n");
}
void mostrarUI(){
    //Limpia pantalla
    system("cls");
    //Imprime \n para centrar el contenido
    for(int i=0;i<(PANTALLA_ALTO/3)-(MAXIMO/2);i++){
        printf("\n");
    }
    //Calcula el margen izquierdo para que este centrado
    int margenIzquierdo = (PANTALLA_ANCHO/2)-(MAXIMO);
    //Regla de DEBUG
    //reglaHorizontal();

    //Margen izquierdo de los numeros orientativos
    for(int i=0;i<margenIzquierdo;i++){
        printf(" ");
    }
    //Numeros orientativos horizontales
    for(int i=0;i<MAXIMO;i++){
        if(i<10) printf("%i ",i);
        else printf("%c ",'A'+i-10);
    }
    //Separacion del tablero
    printf("\n");
    //Datos de que fila se selecciono
    int seleccionado = charToPosicion(antChar);
    for(int i=0;i<MAXIMO;i++){
        //Margen izquierdo del tablero
        for(int j=0;j<margenIzquierdo-2-(seleccionado==i);j++){
            printf(" ");
        }
        if(seleccionado==i) printf(">");
        //Numeros orientativos verticales
        if(i<10) printf("%i ",i);
        else printf("%c ",'A'+i-10);

        //Tablero
        for(int j=0;j<MAXIMO;j++){
            if(mapaOculta[i][j]==true){
                printf("%c",CARACTER_CUADRADO);
            }else{
                if(mapa[i][j]==9)
                    printf("*");
                else if(mapa[i][j]==10)
                    printf("+");
                else
                    printf("%c",mapa[i][j]+'0');
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

void destaparCelda(int f,int c){
    if(mapaOculta[f][c]){
        mapaOculta[f][c]=false;
        destapadas++;
        if(mapa[f][c]==0){
            for(int i = max(0,f-1);i<=min(MAXIMO-1,f+1);i++)
                for(int j = max(0,c-1);j<=min(MAXIMO-1,c+1);j++)
                    if(mapa[i][j]!=9)
                        destaparCelda(i,j);

        }

    }
}
