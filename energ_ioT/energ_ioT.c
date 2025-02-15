#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "lwip/apps/http_client.h"


// ========================================================
// Configurações Wi-Fi
// =========================================================
#define WIFI_SSID "NOME_DA_SUA_REDE" 
#define WIFI_PASSWORD "SENHA_DA_SUA_REDE" 
#define WIFI_COUNTRY CYW43_COUNTRY_BRAZIL
#define WIFI_CHANNEL 6

// Status inicial da conexão Wi-Fi
int wifistatus = WL_IDLE_STATUS;

// ========================================================
// Configurações do Servidor(ThingSpeak)
// =========================================================
const int myChannelNumber = "123456"      
const char* myApiKey = "ABC123XYZ456"
const char* server = "http://api.thingspeak.com/update"


// =========================================================
// Definições do pino GPIO para o Sensor
// ==========================================================
const int SENSOR_PIN = 13;

/// ==========================================================
// Siglas exigidas pelo protocolo do medidor
// ============================================================
#define SIGLA_POTENCIA "PAPP"     // Potência Ativa (W) / (VA), depende do protocolo do medidor
#define SIGLA_HC "HCHC"           // Consumo Horário Fora de Pico (kWh)
#define SIGLA_HP "HPHC"           // Consumo Horário de Pico (kWh)


// ============================================================
// Função para conectar ao Wi- FI
// ============================================================
bool connect_to_wifi() {
    printf("Conectando a %s...\n", WIFI_SSID);
    wifistatus = WL_IDLE_STATUS; // Atualiza status

    cyw43_arch_enable_sta_mode();

    // Configura o canal Wi-Fi
    cyw43_t *cyw = cyw43_arch_get_instance();
    if (cyw) {
        cyw43_wifi_set_channel(cyw, WIFI_CHANNEL);
    }

    wifistatus = WL_CONNECTING; // Atualizando status

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha na conexão Wi-Fi!\n");
        wifistatus = WL_CONNECT_FAILED; // Status de falha
        return false;
    }

    printf("Conectado com sucesso! IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
    wifistatus = WL_CONNECTED; // Status conectado
    return true;
}

// =====================================================================
// Função para ler dados do Sensor (implementação simulada)
// =====================================================================
int read_sensor_data(const char *data_type) {
    // Esta é uma função de exemplo. Implemente a lógica real para ler dados do Sensor.
    // ou seja, a implementação real deve incluir comunicação serial com o Sensor
    static int counter = 0;
    counter++;

    if (strcmp(data_type, SIGLA_POTENCIA) == 0) {
        return 1000 + (counter % 100);   // Potência em watts
    } else if (strcmp(data_type, SIGLA_HC) == 0) {
        return 500 + (counter % 50);       // Consumo HC em kWh
    } else if (strcmp(data_type, SIGLA_HP) == 0) {
        return 300 + (counter % 30);       // Consumo HP em kWh
    }
    return 0;
}

// ==================================================================
// Função para enviar dados ao servidor (ThingSpeak)
// ==================================================================
void send_to_thingspeak(int potencia, int hc, int hp) {
    char url[256];
    snprintf(url, sizeof(url), "%s?api_key=%s&field1=%d&field2=%d&field3=%d",
             server, myApiKey, potenci, hc, hp);

    printf("Enviando para: %s\n", url);

    err_t err = http_client_get(url, NULL, NULL, NULL, 0);
    printf(err == ERR_OK ? "Sucesso!\n" : "Erro: %d\n", err);
}

// ================================================================
// Função principal
// ================================================================
int main() {
    // Inicializa a entrada/saída padrão
    stdio_init_all();
    sleep_ms(2000);

    // Configuração do país para compliance RF (importante!)
    // cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL); // Altere conforme seu país

    // Inicializa o chip Wi-Fi com configuração de país
    if (cyw43_arch_init_with_country(WIFI_COUNTRY)) {
        printf("Falha ao inicializar o Wi-Fi!\n");
        return 1;
    }

    // Conecta ao Wi-Fi
    if (!connect_to_wifi()) {
        cyw43_arch_deinit();
        return 1;
    }

    // Loop principal
    while (true) {
        // Lê dados do Sensor
        int potencia_ativa = read_sensor_data(SIGLA_POTENCIA);
        int consumo_fora_pico = read_sensor_data(SIGLA_HC);
        int consumo_horario_pico = read_sensor_data(SIGLA_HP);

        // Exibe os dados no console
        printf("\n--- Leitura dos Dados ---\n");
        printf("Potência Ativa: %d W\n", potencia_ativa);
        printf("Consumo fora de pico: %d kWh\n", consumo_fora_pico);
        printf("Consumo horário de pico: %d kWh\n", consumo_horario_pico);

        // Envia os dados para o servidor remoto
        send_to_thingspeak(potencia_ativa, consumo_fora_pico, consumo_horario_pico);

        // Aguarda 15 segundos (limite de atualização do ThingSpeak para contas gratuitas)
        // Espera otimizada com polling para manter a conexão
        for (int i = 0; i < 150; i++) {
          sleep_ms(100);
          cyw43_arch_poll();
        }
    }

    // Limpeza
    cyw43_arch_deinit();
    return 0;
}
