#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "time.h"

#define UART_ID uart1
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_RX_PIN 13

// Variáveis globais para armazenamento dos dados
int ap = 0;        // Potência Ativa (kW)
int hchc = 0;      // Consumo Fora de Pico (kWh)
int hphc = 0;      // Consumo Horário de Pico (kWh)

// Flags para controle da simulação
bool simulacao_ativa = false;
absolute_time_t ultima_simulacao;

void process_line(char *line) {
    // Mantido para compatibilidade com dados reais
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

void simular_dados() {
    // Gera valores realistas para o contexto brasileiro
    ap = 1500 + (rand() % 1000);        // 1500-2500 W
    hchc += 1 + (rand() % 3);           // Incremento acumulativo
    hphc += 1 + (rand() % 2);           // Incremento menor no horário de pico
}

int main() {
    stdio_init_all();
    srand(time(NULL));  // Inicializa gerador de números aleatórios
    
    printf("Iniciando sistema...\n");
    printf("Modo: Simulação Integrada (sem hardware)\n\n");

    // Configuração da UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);

    char buffer[256];
    int buffer_index = 0;

    while(true) {
        // Simulação automática se nenhum dado for recebido em 3 segundos
        if(!simulacao_ativa && absolute_time_diff_us(get_boot_time(), get_absolute_time()) > 3000000) {
            printf("Ativando modo de simulação...\n");
            simulacao_ativa = true;
            ultima_simulacao = get_absolute_time();
        }

        // Atualiza dados simulados a cada 2 segundos
        if(simulacao_ativa && absolute_time_diff_us(ultima_simulacao, get_absolute_time()) > 2000000) {
            simular_dados();
            ultima_simulacao = get_absolute_time();
        }

        // Leitura da UART (simulada via terminal)
        if(uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if(c == '\r') {
                buffer[buffer_index] = '\0';
                process_line(buffer);
                buffer_index = 0;
                simulacao_ativa = false;  // Desativa simulação se receber dados
            } else {
                buffer[buffer_index++] = c;
            }
        }

        // Exibição periódica
        static absolute_time_t last_print = {0};
        absolute_time_t now = get_absolute_time();
        
        if(absolute_time_diff_us(last_print, now) >= 1000000) {
            printf("\n--- Leitura %s ---", simulacao_ativa ? "SIMULADA" : "REAL");
            printf("\n| Potência Ativa: %d kW", ap/1000);
            printf("\n| Consumo Fora de Pico: %d kWh", hchc);
            printf("\n| Consumo Horário de Pico: %d kWh", hphc);
            printf("\n----------------------------\n");
            last_print = now;
        }
        
        sleep_ms(100); // Evita sobrecarga do processador (Reduz consumo de CPU)
    }
    return 0;
}