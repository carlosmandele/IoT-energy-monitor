/* ======================================================
 * Sistema de Monitoramento de Energia IoT para Raspberry Pi Pico W
 * Integração com ThingSpeak via Wi-Fi
 * Autor: Carlos Mandele K.
 * ====================================================== */

// Bibliotecas principais
#include <stdio.h>          // Funções padrão de I/O
#include <string.h>         // Manipulação de strings
#include <stdlib.h>         // Funções de conversão
#include "pico/stdlib.h"    // SDK do Raspberry Pi Pico
#include "hardware/uart.h"  // Controle de hardware UART
#include "hardware/gpio.h"  // Controle de GPIO
#include "pico/cyw43_arch.h" // Driver Wi-Fi do Pico W
#include "lwip/pbuf.h"      // Buffer de pacotes lwIP
#include "lwip/tcp.h"       // Protocolo TCP

// ================= CONFIGURAÇÕES DE HARDWARE =================
#define UART_ID uart1       // UART1 (GPIO0/GPIO1)
#define BAUD_RATE 9600      // Velocidade de comunicação
#define DATA_BITS 8         // Bits de dados
#define STOP_BITS 1         // Bits de parada
#define PARITY UART_PARITY_NONE // Sem paridade

#define UART_RX_PIN 13      // GPIO13 para RX (dados do sensor)

// ================= CONFIGURAÇÕES DE REDE =====================
#define WIFI_SSID "SUA_REDE"          // Nome da rede Wi-Fi
#define WIFI_PASSWORD "SUA_SENHA"      // Senha da rede
#define WIFI_COUNTRY CYW43_COUNTRY_BRAZIL // Config. regional

// ================= CONFIGURAÇÕES THINGSPEAK =================
#define THINGSPEAK_API_KEY "SUA_CHAVE_API" // Chave de escrita
#define THINGSPEAK_URL "api.thingspeak.com" // Endpoint da API
#define UPDATE_INTERVAL_MS 20000       // Intervalo de envio (20s)

// ============ VARIÁVEIS GLOBAIS PARA ARMAZENAMENTO DE DADOS DO SENSOR =================
int ap = 0;        // Potência ativa atual (Watts)
int hchc = 0;      // Consumo fora de pico acumulado (Wh)
int hphc = 0;      // Consumo horário pico acumulado (Wh)
absolute_time_t last_update = nil_time; // Último envio

/* ============================================================
 * Função: process_line()
 * Descrição: Processa uma linha de dados recebida via UART
 * Parâmetros:
 *   - line: String com dados no formato "LABEL:VALOR"
 * ========================================================== */
void process_line(char *line) {
    // Divide a linha usando ':' como delimitador
    char *label = strtok(line, ":");
    char *value_str = strtok(NULL, " ");
    
    if (label && value_str) {
        int value = atoi(value_str); // Converte para inteiro
        
        // Atualiza variáveis globais conforme o label
        if (strcmp(label, "PAPP") == 0) {
            ap = value;       // Atualiza potência ativa
        } else if (strcmp(label, "HCHC") == 0) {
            hchc = value;     // Atualiza consumo fora de pico
        } else if (strcmp(label, "HPHC") == 0) {
            hphc = value;     // Atualiza consumo horário pico
        }
    }
}

/* ============================================================
 * Função: tcp_result()
 * Descrição: Callback para eventos TCP
 * ========================================================== */
static err_t tcp_result(void *arg, tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("[ERRO] TCP: %d\n", err); // Log de erros
    }
    return ERR_OK;
}

/* ============================================================
 * Função: send_to_thingspeak()
 * Descrição: Envia dados para a plataforma ThingSpeak
 * Parâmetros:
 *   - ap: Potência ativa
 *   - hchc: Consumo fora de pico
 *   - hphc: Consumo horário pico
 * ========================================================== */
void send_to_thingspeak(int ap, int hchc, int hphc) {
    cyw43_arch_lwip_begin(); // Inicia contexto lwIP
    
    // Cria nova estrutura TCP
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("[ERRO] Falha ao criar PCB\n");
        cyw43_arch_lwip_end();
        return;
    }

    // Resolução DNS do servidor
    ip_addr_t server_ip;
    printf("[NET] Resolvendo DNS...\n");
    err_t dns_result = dns_gethostbyname(THINGSPEAK_URL, &server_ip, NULL, NULL);
    if (dns_result != ERR_OK) {
        printf("[ERRO] DNS: %d\n", dns_result);
        tcp_close(pcb);
        cyw43_arch_lwip_end();
        return;
    }

    // Conexão TCP na porta 80 (HTTP)
    printf("[NET] Conectando ao servidor...\n");
    if (tcp_connect(pcb, &server_ip, 80, tcp_result) != ERR_OK) {
        printf("[ERRO] Conexão TCP falhou\n");
        tcp_close(pcb);
        cyw43_arch_lwip_end();
        return;
    }

    // Monta requisição HTTP GET
    char request[256];
    snprintf(request, sizeof(request),
             "GET /update?api_key=%s&field1=%d&field2=%d&field3=%d HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",  // Fecha conexão após resposta
             THINGSPEAK_API_KEY, ap, hchc, hphc, THINGSPEAK_URL);

    // Envio da requisição
    printf("[NET] Enviando dados...\n");
    if (tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY) != ERR_OK) {
        printf("[ERRO] Falha no envio TCP\n");
        tcp_close(pcb);
        cyw43_arch_lwip_end();
        return;
    }

    // Finalização
    tcp_close(pcb);
    cyw43_arch_lwip_end();
    printf("[SUCESSO] Dados enviados!\n");
}

/* ============================================================
 * Função: wifi_init()
 * Descrição: Inicializa e conecta à rede Wi-Fi
 * ========================================================== */
void wifi_init() {
    // Inicializa hardware Wi-Fi
    if (cyw43_arch_init()) {
        printf("[ERRO] Inicialização Wi-Fi falhou\n");
        return;
    }
    
    // Configurações regionais
    cyw43_wifi_set_country(&cyw43_state, WIFI_COUNTRY);
    
    // Modo estação (cliente Wi-Fi)
    cyw43_arch_enable_sta_mode();
    printf("[Wi-Fi] Conectando a %s...\n", WIFI_SSID);
    
    // Tentativas de conexão com retry
    int retry = 0;
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, 
           CYW43_AUTH_WPA2_AES_PSK, 10000) != 0) {
        printf("[Wi-Fi] Falha na conexão. Tentativa %d\n", ++retry);
        sleep_ms(5000);
        if (retry >= 3) {
            printf("[Wi-Fi] Abortando após 3 tentativas\n");
            return;
        }
    }
    printf("[Wi-Fi] Conectado com sucesso!\n");
    printf("[Wi-Fi] IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
}

/* ============================================================
 * Função Principal
 * ========================================================== */
int main() {
    // Inicializações
    stdio_init_all();  // SDK e USB
    wifi_init();       // Conexão Wi-Fi

    // Configuração da UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false); // Sem controle de fluxo
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false); // FIFO desabilitado

    // Buffer para dados UART
    char buffer[256];
    int buffer_index = 0;
    int last_ap = -1, last_hchc = -1, last_hphc = -1;

    printf("\n=== Sistema Iniciado ===\n");
    printf("Aguardando dados dos sensores...\n\n");

    // Loop principal
    while (true) {
        // Leitura contínua da UART
        while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            
            if (c == '\r') {  // Fim de comando
                buffer[buffer_index] = '\0';
                process_line(buffer);
                buffer_index = 0;
                printf("[UART] Dado recebido: %s\n", buffer);
            } else if (buffer_index < sizeof(buffer) - 1) {
                buffer[buffer_index++] = c; // Armazena caracter
            }
        }

        // Atualiza display local se houver mudanças
        if (ap != last_ap || hchc != last_hchc || hphc != last_hphc) {
            printf("\n--- STATUS ATUAL ---");
            printf("\n| Potência Ativa: %d W", ap);
            printf("\n| Consumo Fora de Pico: %d Wh", hchc);
            printf("\n| Consumo Horário Pico: %d Wh", hphc);
            printf("\n----------------------\n");
            
            // Atualiza cache
            last_ap = ap;
            last_hchc = hchc;
            last_hphc = hphc;
        }

        // Envio periódico para nuvem
        if (absolute_time_diff_us(last_update, get_absolute_time()) >= UPDATE_INTERVAL_MS * 1000) {
            if (cyw43_arch_wifi_connect_status() == CYW43_LINK_UP) {
                send_to_thingspeak(ap, hchc, hphc);
                last_update = get_absolute_time();
            } else {
                printf("[Wi-Fi] Reconectando...\n");
                wifi_init(); // Tentativa de reconexão
            }
        }

        sleep_ms(100); // Controle do loop
    }
    return 0;
}