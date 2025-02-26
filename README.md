# Energy Monitoring System with Raspberry Pi Pico W

![Tech Stack](https://img.shields.io/badge/tech_stack-C%20|%20Pico%20W%20|%20ThingSpeak%20|%20UART%20|%20lwIP-00979D?style=flat)

![Pico W](https://www.raspberrypi.com/app/uploads/2022/06/COBS-1.0-1.png)

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
```c
// Bibliotecas principais
pico_stdlib
hardware_uart
pico_cyw43_arch
pico_lwip
lwip
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

## 🌐 Arquitetura do Sistema
```mermaid
graph TD
    A[Sensor de Energia] -->|UART| B(Pico W)
    B -->|Processamento| C{Dados}
    C -->|Armazenamento| D[Variáveis Locais]
    C -->|Envio| E[ThingSpeak via Wi-Fi]
    E -->|Visualização| F[Dashboard Web]
