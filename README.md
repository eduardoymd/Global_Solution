# Global-Solution-Disruptive-Architectures-Iot-Iob-Generative-Ia

## 💧 Projeto FloodGuardian: Sistema Inteligente de Prevenção e Resposta a Enchentes
### Objetivo
Monitorar áreas de risco em tempo real usando IoT e satélites (como a Carta Internacional), prever 
enchentes e alertar moradores via app mobile, além de permitir atuação da Defesa Civil com apoio de 
painéis e APIs. 

## Descrição do Projeto
Este projeto monitora o nível e o volume de um reservatório usando um sensor ultrassônico HC-SR04, ESP32, display LCD I2C, LEDs indicadores e um módulo RTC DS3231 para registro da data e hora. Os dados são enviados para duas plataformas diferentes:
- 📡 ThingSpeak para visualização remota em nuvem
- ☁️ MQTT + Node-RED para monitoramento via dashboard local

## ⚙️ Funcionalidades
- Leitura da altura da água com sensor ultrassônico HC-SR04
- Cálculo do volume (baseado no formato cilíndrico do reservatório)
- Exibição de dados em LCD 16x2
- Indicação de níveis com LEDs (Verde, Amarelo, Vermelho)
- Envio de dados para ThingSpeak (via HTTP) ou para Node-RED (via MQTT)
- Registro do horário de cada medição com o RTC DS3231

## Simulação no Wokwi
O Wokwi permite simular projetos com ESP32 de forma realista. Ambos os códigos podem ser testados diretamente na plataforma.
### Hardware
| Componente                      | Função Principal                 |
| ------------------------------- | -------------------------------- |
| ESP32                           | Controlador principal            |
| HC-SR04                         | Leitura da distância da água     |
| LCD 16x2 com I2C                | Exibição do nível e volume       |
| LEDs (verde, amarelo, vermelho) | Indicação visual do nível        |
| RTC DS3231                      | Registro do horário das medições |


