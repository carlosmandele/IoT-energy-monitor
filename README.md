# Energy Monitoring System with Raspberry Pi Pico W

![Pico W](https://www.raspberrypi.com/app/uploads/2022/06/COBS-1.0-1.png)

Sistema de monitoramento de consumo energético em tempo real com integração IoT via plataforma nuvem. Desenvolvido para Raspberry Pi Pico W.

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
