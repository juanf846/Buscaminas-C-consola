#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "datos.h"

const char* NOMBRE_ARCHIVO_ESTADISTICAS = "estadisticas.txt";
const char* NOMBRE_ARCHIVO_PARTIDA = "partida.txt";


int partidasGanadas = 0;
int partidasPerdidas = 0;

void agregarPartidaGanada(){
    partidasGanadas++;
    guardarEstadisticas();
}
void agregarPartidaPerdida(){
    partidasPerdidas++;
    guardarEstadisticas();
}
int getPartidas(){
    return partidasGanadas+partidasPerdidas;
}
int getPartidasGanadas(){
    return partidasGanadas;
}
int getPartidasPerdidas(){
    return partidasPerdidas;
}
float getPorcentajePerdidas(){
    if(getPartidas()!=0)
        return (float)partidasPerdidas*100.0f/(float)getPartidas();
    else
        return 0;
}
float getPorcentajeGanadas(){
    if(getPartidas()!=0)
        return (float)partidasGanadas*100.0f/(float)getPartidas();
    else
        return 0;
}

void guardarEstadisticas(){
    FILE* archivo = fopen(NOMBRE_ARCHIVO_ESTADISTICAS,"w");
    if(archivo==NULL){
        printf("ERROR: No se pudo guardar las estadisticas\n");
        system("pause");
        return;
    }
    fprintf(archivo, "%i\n%i",partidasGanadas,partidasPerdidas);
    fclose(archivo);
}

void cargarEstadisticas(){
    FILE* archivo = fopen(NOMBRE_ARCHIVO_ESTADISTICAS,"a+");
    if(archivo==NULL){
        printf("ERROR: No se pudo cargar las estadisticas\n");
        system("pause");
    }
    char* buf = malloc(10);
    fgets(buf,10,archivo);
    partidasGanadas = atoi(buf);
    fgets(buf,10,archivo);
    partidasPerdidas = atoi(buf);
    fclose(archivo);
}

const int VERSION_ARCHIVO_DE_GUARDADO = 1;

void guardarPartida(int semilla, int minas, int maximo, bool** mapaOculta){
    FILE* archivo = fopen(NOMBRE_ARCHIVO_PARTIDA,"w");
    if(archivo==NULL){
        printf("ERROR: No se pudo abrir el archivo de la partida\n");
        system("pause");
        return;
    }
    fprintf(archivo,"%i\n",VERSION_ARCHIVO_DE_GUARDADO);
    fprintf(archivo,"%i\n",semilla);
    fprintf(archivo,"%i\n",minas);
    fprintf(archivo,"%i\n",maximo);
    for(int i=0;i<maximo;i++){
        for(int j=0;j<maximo;j++){
            fprintf(archivo,"%i",mapaOculta[i][j]);
        }
    }
    fclose(archivo);
}

FILE* archivo;
bool cargarPartidaMetadata(int* semilla, int* minas, int* maximo){
    archivo = fopen(NOMBRE_ARCHIVO_PARTIDA,"r");
    if(archivo==NULL){
        printf("ERROR: No se pudo abrir el archivo de la partida\n");
        system("pause");
        return false;
    }
    int tamBuf = 15;
    char* buf = malloc(tamBuf);
    fgets(buf,tamBuf,archivo);
    if(atoi(buf)!=VERSION_ARCHIVO_DE_GUARDADO){
        printf("ERROR: El archivo de guardado no es compatible\n");
        fclose(archivo);
        system("pause");
        return false;
    }
    fgets(buf,tamBuf,archivo);
    *semilla = atoi(buf);

    fgets(buf,tamBuf,archivo);
    *minas = atoi(buf);

    fgets(buf,tamBuf,archivo);
    *maximo = atoi(buf);

    free(buf);
    return true;
}
bool cargarPartidaMapa(bool** mapaOculta, int maximo, int* destapadas){
    for(int i=0;i<maximo;i++){
        for(int j=0;j<maximo;j++){
            char a = fgetc(archivo);
            a = a-'0';
            mapaOculta[i][j] = a;
            if(a==0)
                *destapadas=*destapadas+1;
        }
    }
    fclose(archivo);
    return true;
}
