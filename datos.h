#ifndef DATOS_H_INCLUDED
#define DATOS_H_INCLUDED

void agregarPartidaGanada();
void agregarPartidaPerdida();
int getPartidas();
int getPartidasPerdidas();
int getPartidasGanadas();
float getPorcentajePerdidas();
float getPorcentajeGanadas();
void guardarEstadisticas();
void cargarEstadisticas();
void guardarPartida(int semilla, int minas, int maximo, bool** mapaOculta);
bool cargarPartidaMetadata(int* semilla, int* minas, int* maximo);
bool cargarPartidaMapa(bool** mapaOculta, int maximo, int* destapadas);

#endif // DATOS_H_INCLUDED
