#include <stdio.h>
#include <stdlib.h>
#include "datos.h"

void guardarEstadisticas();

const char* NOMBRE_ARCHIVO_GUARDADO = "estadisticas.txt";
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
    FILE* archivo = fopen(NOMBRE_ARCHIVO_GUARDADO,"w");
    if(archivo==NULL){
        printf("ERROR: No se pudo guardar las estadisticas");
        return;
    }
    fprintf(archivo, "%i\n%i",partidasGanadas,partidasPerdidas);
    fclose(archivo);
}

void cargarEstadisticas(){
    FILE* archivo = fopen(NOMBRE_ARCHIVO_GUARDADO,"a+");
    if(archivo==NULL){
        printf("ERROR: No se pudo cargar las estadisticas");
    }
    char* buf = malloc(10);
    fgets(buf,10,archivo);
    partidasGanadas = atoi(buf);
    fgets(buf,10,archivo);
    partidasPerdidas = atoi(buf);
    fclose(archivo);
}
