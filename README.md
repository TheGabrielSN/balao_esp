| Modelos suportados | ESP32 |
| ------------------ | ----- |

###### _Projeto Balão [ESP32]_

Programa para **ESP32 wroom.**

![img](https://github.com/TheGabrielSN/balao_esp/blob/main/imagens/Diagrama%20-%20ESP.png)

O Código realiza a função de controlar os efeitos a serem apresentados nos LEDs. Sua função é receber pacotes de informações do servidor MQTT referente ao efeito a ser aplicado, além disso, o mesmo fornece informações sobre a situação atual do ESP (nível de sínal, efeito atual e nível de bateria).

Sua execução é feita em dois níveis o **controle da fita de LEDs** e a **conexão Wifi** juntamente com a **conexão com o servidor MQTT.**

## Modo de Uso

A execução de forma paralela, onde o ESP32 utiliza um núcleo para gerenciamento da comunicação com o servidor MQTT enquanto o segundo núcleo realiza a execução dos efeitos.


![img](https://github.com/TheGabrielSN/balao_esp/blob/main/imagens/Diagrama%20de%20fluxo%20-%20ESP.png)

### Fita de LED

Principal arquivo: [effects.c](./main/effects.c)/[effects.h](./main/effects.h)

Para a conexão com a fita de LED, são utilizados 3 pinos PWM do ESP, definidos no arquivo [effects.c](./main/effects.c) (função configPin) sendo os **pinos 12, 14 e 27.**

Efeitos disponíveis (controladas pela função MENU):

* Cores estaticas
  1. red
  2. gree
  3. blue
  4. white
  5. greenLemon
  6. pink
  7. cyan
  8. purple
  9. yellow
  10. orange
* Efeitos
  1. flashes
  2. standByPulsate
  3. standByFade
  4. fire
  5. rain
  6. hot
  7. cold
  8. randomEffects
  9. sparklingFire

**AVISO:** Cuidado ao modificar a effects.c, a função configPin realiza a configuração correta para trabalhar com os pinos no modo PWM.

### Conexão WiFi & MQTT

Princial arquivo: [main.c](./main/main.c)

Para a conexão WiFi é preciso ajustar dois parâmetros na [main.c](./main/main.c) **WIFI_SSID** e **WIFI_PASSWORD,** permitindo o acesso a rede.

Para a conexão com o servidor MQTT é preciso ajustar o parâmetro **MQTT_URI,** permitindo o acesso ao servidor. É importante que esteja na mesma rede.

## Diretorio do Projeto

Os arquivos do projeto estão distribuídos no esquema abaixo

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   ├── effects.c
│   ├── effects.h
│   └── main.c
└── README.md                  This is the file you are currently reading
```

Caso adicionado algum arquivo a ser compilado junto do programa, é necessário atualizar o arquivo [CMakeLists.txt](./main/CMakeLists.txt) na pasta main.

## Autores
[Eduardo dos Reis Souza]()

[Gabriel da Silva Nascimento](https://github.com/TheGabrielSN)

[Jhonatan Guilherme Oliveira Pereira](https://github.com/jhonatangopereira)

[Lílian Honorio Teixeira](https://github.com/lilianhteixeira)
