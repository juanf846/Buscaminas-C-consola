#include "utils.h"
#include "main.h"

int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

int min(int num1, int num2)
{
    return (num1 > num2 ) ? num2 : num1;
}

int string_length(char s[]) {
   int c = 0;

   while (s[c] != '\0')
      c++;

   return c;
}

void center_printf(char* texto){
    int length = string_length(texto);
    for(int i=0;i<PANTALLA_ANCHO/2-length/2;i++){
        printf(" ");
    }
    printf(texto);
}
