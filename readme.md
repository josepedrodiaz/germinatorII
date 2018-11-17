# GERMINATOR
Un controlador de indoor basado en el SoC ESP8266 y desarrollado en C++ con el Arduino IDE

## Synopsis

Implementa una serie de sensores y actuadores para controlar un cultivo de interiores.

## Motivation

Una contribución a la Internet de las Cosas desarrollada a partir del SoC ESP8266 (esp-12).

## Installation

Es necesario configurar en el código
* Nombres y passwords de los APs a los que se podrá conectar el dispositivo
* Período de actualizaciones en Ubidots
* Claves de Dispositivo y variables en Ubidots


## API Reference

* La URL raíz de la IP del módulo (/) devuelve un enlace para decargar el registro de datos que queda como backup en el caso de que el dispositivo no logre conectarse a Ubidots

## Tests

## Contributors

P. Díaz.

## License

MIT


## ToDo
* Conected/Alive Led
* Interfaz gráfica que permita configurar todo slos parametros del dispositivos como SSID/Passwd/Frecuencia de lectura de datos, umbrales de activación de leds de informe, etc
