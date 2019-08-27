#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include "estadisticas.h"
#include "utils.h"
#include "datos.h"

void estadisticas_main(){
    system("cls");
    char* buf = malloc(100);

    printf("\n\n");
    center_printf("Estadisticas del jugador:\n\n\n");

    sprintf(buf, "Partidas jugadas: %i\n\n",getPartidas());
    center_printf(buf);

    sprintf(buf, "Partidas ganadas: %i      Porcentaje ganadas: %%%% %g \n\n",
             getPartidasGanadas(),getPorcentajeGanadas());
    center_printf(buf);

    sprintf(buf, "Partidas perdidas: %i      Porcentaje perdidas: %%%% %g \n\n",
             getPartidasPerdidas(),getPorcentajePerdidas());
    center_printf(buf);

    printf("\n\n\n");
    center_printf("Presione una tecla para continuar . . .");
    getch();
}

