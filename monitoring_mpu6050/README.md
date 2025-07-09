# Sistema de Monitoramento MPU6050

Sistema de monitoramento de inclinação usando o sensor MPU6050 com Raspberry Pi Pico.

## Estrutura do Projeto

```
monitoring_mpu6050/
├── monitoring_mpu6050.c    # Arquivo principal
├── mpu6050/
│   ├── mpu6050.h          # Cabeçalho da biblioteca MPU6050
│   └── mpu6050.c          # Implementação da biblioteca MPU6050
└── README.md              # Este arquivo
```

## Funcionalidades

- **Leitura de dados do MPU6050**: Acelerômetro e giroscópio
- **Cálculo de ângulos**: Pitch e Roll em graus
- **Sistema de alerta**: Alerta quando inclinação > 15°
- **Saída no terminal**: Dados formatados e alertas visuais

## Conexões

### MPU6050
- **SDA**: GP0
- **SCL**: GP1
- **VCC**: 3.3V
- **GND**: GND

## Compilação

```bash
# Compilar o projeto
mkdir build
cd build
cmake ..
make
```

## Uso

1. Conecte o MPU6050 conforme as especificações acima
2. Compile o projeto
3. Execute no Raspberry Pi Pico
4. Monitore a saída no terminal

## Saída no Terminal

```
=== DADOS DO MPU6050 ===
Angulos de Inclinacao:
  Pitch: 15.23° (inclinacao frente/tras)
  Roll:  -2.45° (inclinacao lateral)

Dados Brutos:
  Acelerometro - X: 1234, Y: -567, Z: 8901
  Giroscopio   - X: 45, Y: -12, Z: 78
========================

🚨 ALERTA: Inclinacao excedida! 🚨
   Pitch: 18.45° (limite: 15.0°)
   Ajuste a posicao do sensor!
```

## Configurações

- **Limite de alerta**: 15° (configurável em `mpu6050.h`)
- **Frequência de leitura**: 10Hz (100ms)
- **Interface**: I2C0 (100kHz)

## Biblioteca MPU6050

### Funções Principais

- `mpu6050_init()`: Inicializa o sensor
- `mpu6050_read_all(data)`: Lê todos os dados
- `mpu6050_check_alert(data)`: Verifica alertas de inclinação

### Estrutura de Dados

```c
typedef struct {
    float pitch;        // Ângulo Pitch em graus
    float roll;         // Ângulo Roll em graus
    int16_t accel[3];   // Dados brutos do acelerômetro [X, Y, Z]
    int16_t gyro[3];    // Dados brutos do giroscópio [X, Y, Z]
} mpu6050_data_t;
``` 