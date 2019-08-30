#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <conio.h>
#include "utils.h"
#include "partida.h"
#include "main.h"
#include "datos.h"

/*Estados:
ESPERANDO(0) = Se puede ingresar posicion o ir al menu
POSICION(1) = Se ingreso el primer caracter de la posicion
MENU(2) = Se abrio el menu
PERDIO(3) = El jugador perdió
GANO(4) = El jugador ganó
*/
#define ESTADO_ESPERANDO 0
#define ESTADO_POSICION 1
#define ESTADO_MENU 2
#define ESTADO_PERDIO 3
#define ESTADO_GANO 4

#define CARACTER_CUADRADO 254

void mostrarUI();
bool mostrarDificultades();

void estadoEsperando();
void estadoPosicion();
void estadoMenu();

void generar_mapa(bool);

void destaparCelda(int,int);
int charToPosicion(char);

int maximo;
int semilla;
int minas;
int destapadas;
char** mapa;
bool** mapaOculta;
bool continuar;
char estado;
char antChar;
char c;

/** Funcion principal de la pantalla partida
 *  Parametros:
 *  *cPartida: Indica si se tiene que cargar la partida o iniciar una nueva
 */
void partida_main(bool cPartida)
{
    destapadas = 0;
    if(!cPartida){
        //nueva partida
        if(!mostrarDificultades()){
            return;
        }
        struct timeval time;
        gettimeofday(&time,NULL);
        semilla = time.tv_sec;

        mapa = malloc(sizeof(char*)*maximo);
        mapaOculta = malloc(sizeof(bool*)*maximo);
        for(int i=0;i<maximo;i++){
            mapa[i] = malloc(sizeof(char)*maximo);
            mapaOculta[i] = malloc(sizeof(bool)*maximo);
        }
    }else{
        //cargar partida
        if(cargarPartidaMetadata(&semilla,&minas,&maximo)){
            mapa = malloc(sizeof(char*)*maximo);
            mapaOculta = malloc(sizeof(bool*)*maximo);
            for(int i=0;i<maximo;i++){
                mapa[i] = malloc(sizeof(char)*maximo);
                mapaOculta[i] = malloc(sizeof(bool)*maximo);
            }
            cargarPartidaMapa(mapaOculta,maximo,&destapadas);
        }else{
            return;
        }
    }
    estado = ESTADO_ESPERANDO;
    antChar = 0;
    continuar = true;

    generar_mapa(!cPartida);
    while(continuar){
        switch(estado){
            case ESTADO_ESPERANDO:
                estadoEsperando();
                break;
            case ESTADO_POSICION:
                estadoPosicion();
                break;
            case ESTADO_MENU:
                estadoMenu();
                break;
            case ESTADO_PERDIO:
                mostrarUI();
                printf("\n\n");
                center_printf("PERDISTE");
                printf("\n");
                agregarPartidaPerdida();
                getch();
                return;
            case ESTADO_GANO:
                mostrarUI();
                printf("\n\n");
                center_printf("GANASTE!!!!");
                printf("\n");
                agregarPartidaGanada();
                getch();
                return;
        }
    }
    return;
}

/** Esta funcion se llama para mostrar la interfaz y recibir el primer caracter. ESC abre menú
 */
void estadoEsperando(){
    mostrarUI();
    center_printf("Ingrese la fila\n");
    center_printf("Aprete ESC para abrir el menu\n");

    /*
    printf("DEBUG:\n");
    printf("Maximo: %i\n",maximo);
    printf("Minas: %i\n",minas);
    printf("Destapadas: %i\n",destapadas);
    */

    c = getch();
    if(charToPosicion(c)!=-1){
        antChar = c;
        estado = ESTADO_POSICION;
    }else if(c == 27){
        estado = ESTADO_MENU;
    }else{
        center_printf("\nERROR: Valor no valido\n");
        system("PAUSE");
    }
}

/** Esta funcion se llama para mostrar la interfaz y recibir el segundo caracter. ESC cancela y vuelve a estadoEsperando
 */
void estadoPosicion(){
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
            for(int i=0;i<maximo;i++)
                for(int j=0;j<maximo;j++)
                    if(mapa[i][j]==9)
                        mapaOculta[i][j]=false;
            estado = ESTADO_PERDIO;
            return;
        }
        if(destapadas == maximo*maximo-minas){
            //Ganó
            printf("gano");
            estado = ESTADO_GANO;
            return;
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
}

/** Menu in-game
*/
void estadoMenu(){
    system("CLS");
    //Imprime \n para centrar el contenido
    for(int i=0;i<PANTALLA_ALTO/2-9/2;i++){
        printf("\n");
    }
    center_printf("1. Continuar");
    printf("\n\n");
    center_printf("2. Guardar partida");
    printf("\n\n");
    center_printf("3. Salir sin guardar");
    printf("\n\n");
    c = getch();
    switch(c){
        case '1':
            estado = ESTADO_ESPERANDO;
            return;
        case '2':
            guardarPartida(semilla,minas,maximo,mapaOculta);
            estado = ESTADO_ESPERANDO;
            return;
        case '3':
            continuar = false;
            return;
    }
}
/** Genera el mapa segun la semilla. Si mapaOcultaInit es falso, no inicializa la matriz de celdas ocultas
 * (porque esta cargando una partida)
 */
void generar_mapa(bool mapaOcultaInit){
    srand(semilla);
    //pone todo la matriz a 0
    for(int i=0;i<maximo;i++){
        for(int j=0;j<maximo;j++){
            mapa[i][j] = 0;
            if(mapaOcultaInit)
                mapaOculta[i][j] = true;
        }
    }
    //Agrega las minas
    for(int i=0;i<minas;i++){
        int x;
        int y;
        do{
            x = rand() % maximo;
            y = rand() % maximo;
        }while(mapa[x][y]==9);
        mapa[x][y] = 9;
        //agrega 1 a todas las celdas pegadas
        for(int x1 = -1; x1 < 2;x1++){
            for(int y1 = -1; y1 < 2;y1++){
                if(x+x1>-1 && x+x1<maximo && y+y1>-1 && y+y1<maximo && mapa[x+x1][y+y1] != 9){
                    mapa[x+x1][y+y1] += 1;
                }
            }
        }
    }
}

/** Muestra el tablero
 */
void mostrarUI(){
    //Limpia pantalla
    system("cls");
    //Imprime \n para centrar el contenido
    for(int i=0;i<(PANTALLA_ALTO/3)-(maximo/2);i++){
        printf("\n");
    }
    //Calcula el margen izquierdo para que este centrado
    int margenIzquierdo = (PANTALLA_ANCHO/2)-(maximo);
    //Regla de DEBUG
    //reglaHorizontal();

    //Margen izquierdo de los numeros orientativos
    for(int i=0;i<margenIzquierdo;i++){
        printf(" ");
    }
    //Numeros orientativos horizontales
    for(int i=0;i<maximo;i++){
        if(i<10) printf("%i ",i);
        else printf("%c ",'A'+i-10);
    }
    //Separacion del tablero
    printf("\n");
    //Datos de que fila se selecciono
    int seleccionado = charToPosicion(antChar);
    for(int i=0;i<maximo;i++){
        //Margen izquierdo del tablero
        for(int j=0;j<margenIzquierdo-2-(seleccionado==i);j++){
            printf(" ");
        }
        if(seleccionado==i) printf(">");
        //Numeros orientativos verticales
        if(i<10) printf("%i ",i);
        else printf("%c ",'A'+i-10);

        //Tablero
        for(int j=0;j<maximo;j++){
            if(mapaOculta[i][j]==true){
                printf("%c",CARACTER_CUADRADO);
            }else{
                if(mapa[i][j]==9)
                    printf("*");
                else if(mapa[i][j]==10)
                    printf("+");
                else if(mapa[i][j]==0)
                    printf(" ");
                else
                    printf("%c",mapa[i][j]+'0');
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

bool mostrarDificultades(){
    system("cls");
    //Imprime \n para centrar el contenido
    for(int i=0;i<PANTALLA_ALTO/2-9/2;i++){
        printf("\n");
    }
    center_printf("1. Facil (7x7, 10 minas)");
    printf("\n\n");
    center_printf("2. Medio (12x12, 25 minas");
    printf("\n\n");
    center_printf("3. Dificil (15x15, 50 minas)");
    printf("\n\n");
    center_printf("0. Cancelar");
    printf("\n\n");

    bool valido;
    do{
        valido = true;
        c = getch();
        switch(c){
            case '1':
                maximo = 7;
                minas = 10;
                break;
            case '2':
                maximo = 12;
                minas = 25;
                break;
            case '3':
                maximo = 15;
                minas = 50;
                break;
            case '0':
                return false;
            default:
                valido = false;
        }
    }while(!valido);
    return true;
}

/** Funcion para destapar una celda. Si la celda es un 0, destapa tambien las celdas pegadas (recursivamente)
 */
void destaparCelda(int f,int c){
    if(mapaOculta[f][c]){
        mapaOculta[f][c]=false;
        destapadas++;
        if(mapa[f][c]==0){
            for(int i = max(0,f-1);i<=min(maximo-1,f+1);i++)
                for(int j = max(0,c-1);j<=min(maximo-1,c+1);j++)
                    if(mapa[i][j]!=9)
                        destaparCelda(i,j);

        }

    }
}
/**
 * Convierte el caracter ingresado a una posicion de la matriz. Recibe de 0 a 9 y de A(10) a K(19). Devuelve -1 si es invalido.
 */
int charToPosicion(char c){
    int a = -1;
    if((c >= '0' && c <= '9')){
        a = c-'0';
    }else if((c >= 'a' && c <= 'k')){
        a = c-'a'+10;
    }
    if(a>=maximo){
        a = -1;
    }
    return a;
}
