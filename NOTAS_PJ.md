### Explicação:

- **Conexão Wi-Fi**: A função connect_to_wifi usa a função cyw43_arch_wifi_connect_timeout_ms do Pico SDK para conectar-se a uma rede Wi-Fi.

- **Leitura de dados Linky**: A função read_linky_data é um espaço reservado. Você precisa implementar a lógica real para ler dados do dispositivo Linky conectado ao pino GPIO especificado.

- **Solicitação HTTP POST**: A função send_http_post envia uma solicitação HTTP POST para o URL especificado. Esta função usa o http_client do Pico SDK.

- **Loop principal**: O loop principal lê os dados do Linky, imprime-os no console e os envia ao servidor Emoncms por meio de uma solicitação HTTP POST.

- **Sleep**: A função sleep_ms é usada para esperar 1 segundo entre as iterações do loop.

### Notes:

Linky Implementation: The actual implementation of reading data from the Linky device is not provided. You need to implement this based on the specific protocol used by the Linky device.

Error Handling: The code includes basic error handling, but you may want to add more robust error handling depending on your application's needs.

Wi-Fi Credentials: Replace "votre_réseau" and "votre_clé" with your actual Wi-Fi SSID and password.

API Key: Replace "votre_cle" with your actual API key for Emoncms.
