```
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "lwip/apps/http_client.h"

// Define o pino GPIO para o Linky
#define LINKY_PIN 13

// Configurações do ThingSpeak
#define THINGSPEAK_API_KEY "ABC123XYZ456" // Substitua pela sua API Key do ThingSpeak
#define THINGSPEAK_CHANNEL_ID "123456"      // Substitua pelo ID do seu canal no ThingSpeak
#define THINGSPEAK_URL "http://api.thingspeak.com/update"

// Configurações Wi-Fi
#define WIFI_SSID "NOME_DA_SUA_REDE"       // 
#define WIFI_PASSWORD "SENHA_DA_SUA_REDE"  // 
#define WIFI_COUNTRY CYW43_COUNTRY_BRAZIL  // Altere conforme necessário


// Função para conectar ao Wi-Fi
bool connect_to_wifi() {
    printf("Conectando a %s...\n", WIFI_SSID);
    cyw43_arch_enable_sta_mode();
    
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha na conexão Wi-Fi!\n");
        return false;
    }
    
    printf("Conectado com sucesso!\n");
    printf("IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
    return true;
}

// Função para ler dados do Linky (implementação simulada)
int read_linky_data(const char *data_type) {
    // Esta é uma função de exemplo. Implemente a lógica real para ler dados do Linky.
    // ou seja, a implementação real deve incluir comunicação serial com o Linky
    static int counter = 0;
    counter++;

    if (strcmp(data_type, "PAPP") == 0) {
        return 1000 + (counter % 100);  // Exemplo de valor de potência em watts
    } else if (strcmp(data_type, "HCHC") == 0) {
        return 500 + (counter % 50);               // Exemplo de valor de HC em kWh
    } else if (strcmp(data_type, "HPHC") == 0) {
        return 300 + (counter % 30); // Exemplo de valor de HP em kWh
    }
    return 0;
}

// Função para enviar dados ao ThingSpeak
void send_to_thingspeak(int field1, int field2, int field3) {
    char url[256];
    snprintf(url, sizeof(url), "%s?api_key=%s&field1=%d&field2=%d&field3=%d",
             THINGSPEAK_URL, THINGSPEAK_API_KEY, field1, field2, field3);

    printf("Enviando para: %s\n", url);

    err_t err = http_client_get(url, NULL, NULL, NULL, 0);
    if (err != ERR_OK) {
        printf("Erro no HTTP: %d\n", err);
    } else {
        printf("Dados enviandos com sucesso!\n");
    }          

}

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
        // Lê dados do Linky
        int power = read_linky_data("PAPP");
        int hc = read_linky_data("HCHC");
        int hp = read_linky_data("HPHC");

        // Exibe os dados no console
        printf("\n--- Leitura dos Dados ---\n");
        printf("Potência Atual: %d W\n", power);
        printf("Consumo HC: %d kWh\n", hc);
        printf("Consumo HP: %d kWh\n", hp);

        // Envia os dados para o ThingSpeak
        send_to_thingspeak(power, hc, hp);

        // Aguarda 15 segundos (limite de atualização do ThingSpeak para contas gratuitas)
        // Espera com polling para manter a conexão
        for (int i = 0; i < 150; i++) {
          sleep_ms(100);
          cyw43_arch_poll();
        }
    }

    // Limpeza
    cyw43_arch_deinit();
    return 0;
}

"""
Para comunicação real com o Linky:

Implemente a leitura serial na função read_linky_data

Configure os parâmetros corretos de UART

Adicione o protocolo de decodificação TIC
""" ```
