#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// Variáveis  globais para armazenamento dos dados
int ap = 0;     // Potência Ativa (W)
int hchc = 0;   // Consumo Fora de Pico (Wh)
int hphc = 0;   // Consumo Horário de Pico (Wh)

void process_line(char *line) {
    char *label = strtok(line, ":");
    char *value_str = strtok(NULL, " ");
    
    if (label && value_str) {
        int value = atoi(value_str);
        
        if (strcmp(label, "PAPP") == 0) {
            ap = value;
        } else if (strcmp(label, "HCHC") == 0) {
            hchc = value;
        } else if (strcmp(label, "HPHC") == 0) {
            hphc = value;
        }
    }
}

int main() {
    stdio_init_all();
    
    printf("=== Modo de Teste Terminal ===\n");
    printf("Digite comandos no formato PAPP:1500 e pressione ENTER\n\n");

    char buffer[256];
    int buffer_index = 0;

    while(true) {
        // Leitura do terminal USB (não bloqueante)
        int c = getchar_timeout_us(0);
        
        if(c != PICO_ERROR_TIMEOUT) {
            if(c == '\r' || c == '\n') {  // Final da linha
                buffer[buffer_index] = '\0';
                printf("Processando: %s\n", buffer);
                process_line(buffer);
                buffer_index = 0;
            } else {
                buffer[buffer_index++] = c;
            }
        }

        // Exibe dados a cada 1 segundo
        static absolute_time_t last_print = {0};
        absolute_time_t now = get_absolute_time();
        
        if(absolute_time_diff_us(last_print, now) >= 1000000) {
            printf("\n--- Status Atual ---");
            printf("\n| Potência Ativa: %d W", ap);
            printf("\n| Consumo Fora de Pico: %d Wh", hchc);
            printf("\n| Consumo Horário Pico: %d Wh", hphc);
            printf("\n----------------------\n");
            last_print = now;
        }
        
        sleep_ms(10);
    }
    return 0;
}