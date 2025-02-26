# Energy Monitoring System with Raspberry Pi Pico W

![Tech Stack](https://img.shields.io/badge/tech_stack-C%20|%20Pico%20W%20|%20ThingSpeak%20|%20UART%20|%20lwIP-00979D?style=flat)

Sistema de monitoramento de consumo energético em tempo real com integração IoT via nuvem. Desenvolvido para Raspberry Pi Pico W.

## 🔧 Tech Stack
### Principais Tecnologias
![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi%20Pico%20W-C51A4A?style=for-the-badge&logo=Raspberry-Pi&logoColor=white)
![ThingSpeak](https://img.shields.io/badge/ThingSpeak-00979D?style=for-the-badge&logo=thingspeak&logoColor=white)
![UART](https://img.shields.io/badge/Protocolo-UART-007ACC?style=for-the-badge)

### Ferramentas de Desenvolvimento
![VS Code](https://img.shields.io/badge/Editor-VS%20Code-007ACC?style=for-the-badge&logo=visual-studio-code)
![CMake](https://img.shields.io/badge/Build-CMake-064F8C?style=for-the-badge&logo=cmake)
![Git](https://img.shields.io/badge/Versionamento-Git-F05032?style=for-the-badge&logo=git)

### Bibliotecas Chave
![Pico SDK](https://img.shields.io/badge/SDK-Raspberry%20Pi%20Pico-FF3366?style=for-the-badge)
![lwIP](https://img.shields.io/badge/Network-lwIP-3A75BD?style=for-the-badge)
![CYW43](https://img.shields.io/badge/WiFi-CYW43%20Driver-00A98F?style=for-the-badge)



## 📋 Índice
- [Funcionalidades](#-funcionalidades)
- [Pré-requisitos](#-pré-requisitos)
- [Dependências](#-dependências)
- [Instalação](#-instalação)
- [Configuração](#-configuração)
- [Uso](#-uso)
- [Fluxo de Desenvolvimento](#-fluxo-de-desenvolvimento)
- [Arquitetura do Sistema](#-arquitetura-do-sistema)
- [Solução de Problemas](#-solução-de-problemas)
- [Licença](#-licença)

## 🚀 Funcionalidades
- Leitura de sensores via UART (9600 baud)
- Conexão Wi-Fi automática com reconexão
- Envio periódico de dados para ThingSpeak
- Monitoramento local via console serial
- Suporte a múltiplas grandezas elétricas:
  - Potência Ativa (W)
  - Consumo Horário (Wh)
  - Consumo Fora de Pico (Wh)

## 🛠 Pré-requisitos

### Hardware
| Componente               | Especificações                          |
|--------------------------|-----------------------------------------|
| Raspberry Pi Pico W      | Com chip Wi-Fi CYW43439                 |
| Sensor de Energia        | Interface UART, 3.3V                    |
| Cabo USB-C               | Para alimentação e programação          |
| Conexões                 | GPIO13 (UART1 RX) ↔ Sensor TX           |

### Software
| Componente               | Versão Mínima | Link                              |
|--------------------------|---------------|-----------------------------------|
| Pico SDK                 | 1.5.0         | [GitHub](https://github.com/raspberrypi/pico-sdk) |
| Toolchain ARM GCC        | 10.3.1        | [Arm Developer](https://developer.arm.com/) |
| Python (para scripts)    | 3.9           | [Python.org](https://www.python.org/) |

## 📦 Dependências

```
// Linguagem
C
// Bibliotecas principais
pico_stdlib
hardware_uart
pico_cyw43_arch
pico_lwip
lwip
```

## Instalação
1. **Configurar Ambiente**:

```
# Clone o repositório
git clone https://github.com/carlosmandele/energ_ioT.git
cd energ_ioT

# Configure as variáveis de ambiente
export PICO_SDK_PATH=~/pico/pico-sdk
```
2. **Compilar projeto**:

```
mkdir build
cd build
cmake -DPICO_BOARD=pico_w ..
make -j4
```
3. **Flash no Pico**:
   - Mantenha pressionado o botão `BOOTSEL` ao conectar o USB
   - Copie o arquivo `.uf2` para a unidade montada

## Configuração
- **Arquivo `main.c`**:

```
// Configure suas credenciais:
#define WIFI_SSID "NOME_DA_REDE"
#define WIFI_PASSWORD "SENHA_DA_REDE"
#define THINGSPEAK_API_KEY "XXXXXXXXXXXXXXXX"
```

# Uso

1. **Monitoramento Serial**:

```
# Linux
screen /dev/ttyACM0 115200

# Windows
# Use Putty ou Terminal Serial USB
```

2. **Saída Esperada**:

```
[Wi-Fi] Conectado com sucesso!
[Wi-Fi] IP: 192.168.1.10
[UART] Dado recebido: PAPP:1500
[Dados] Enviados para ThingSpeak!
```

3. **Plataforma IoT Nuvem (ThingSpeak)**:
   
   i. Crie um canal com 3 campos:
   
   - Field 1: Potência Ativa (W)

   - Field 2: Consumo Fora de Pico (Wh)

   - Field 3: Consumo Horário Pico (Wh)

   ii. Acesse:
   ```
   https://thingspeak.com/channels/[CHANNEL_ID]
   ```

---

## 🛠️ Fluxo de Desenvolvimento
1. **Codificação**:  
   - Linguagem C com Pico SDK  
   - VS Code com extensões para C/C++ e PlatformIO

2. **Testes**:  
   - Monitor serial integrado (`screen`/`minicom`)  
   - Simulação de dados via Python

3. **Deploy**:  
   - Compilação via CMake  
   - Flash via USB-C com arquivo `.uf2`
  
---
## Solução de problemas

### **Problemas Comuns**:
| Sintoma               | Possíveis Causas | Solução                              |
|--------------------------|---------------|-----------------------------------|
| Wi-Fi não conecta                 | SSID/Senha incorretos         | Verifique caracteres especiais |
| Dados não aparecem        | API Key inválida        | Revise permissões do canal |
| Erros UART    | Baud rate incompatível           | Confira especificação do sensor |
| Timeout DNS    | Sem conexão com internet           | Teste ping para `api.thingspeak.com` |

### **Logs de Depuração**:

```
# Habilite debug do Wi-Fi
printf("Status Wi-Fi: %d\n", cyw43_arch_wifi_connect_status());
```

## 🌐 Arquitetura do Sistema
```
graph TD
    A[Sensor de Energia] -->|UART| B(Pico W)
    B -->|Processamento| C{Dados}
    C -->|Armazenamento| D[Variáveis Locais]
    C -->|Envio| E[ThingSpeak via Wi-Fi]
    E -->|Visualização| F[Dashboard Web]

## Licença
License - Veja o arquivo LICENSE para detalhes.
Suporte:
