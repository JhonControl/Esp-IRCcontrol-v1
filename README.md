# ESP-IRCcontrol v1.0 ESP8266 from the Internet

Version 1.0,   08-04-2018  [PDAControl](http://pdacontrolen.com)


This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Este programa se distribuye con la esperanza de que sea útil, pero SIN NINGUNA GARANTÍA;
sin siquiera la garantía implícita de COMERCIABILIDAD o IDONEIDAD PARA UN PROPÓSITO PARTICULAR.

![alt text](http://pdacontrolen.com/wp-content/uploads/2018/03/Basic-Blynk-PZEM-004_part1-768x432.png "ESP-IRCcontrol v1.0")

## Description

Initial tests of implementation of [IRC (Internet Chat Relay)](https://en.wikipedia.org/wiki/Internet_Relay_Chat) Client 
in ESP8266 12E allowing communication by internet via Chat IRC, There are many methods, Platforms and
IoT solutions for the control and monitoring of ESP8266,but.This implementation allows in a few steps to control 
an ESP8266 from the Internet in a very simple way without very special configurations using the existing ones
Public [IRC](https://en.wikipedia.org/wiki/Internet_Relay_Chat)  servers simply sending messages between devices.

Contains 2 files Arduino IDE:

* Esp-IRCcontrol_v1.ino  - Main routine 

* _ IRCfuntions.ino  - Additional functions IRC 

### Some advantages
* Bidirectional connection through the Internet without registering to Applications, Token's or Platforms, I consider it an IoT application.
* Does not require Hosting, Public Fixed IP Addresses, VPN's, special ports.
* The application is Answer by Request of external client.
* IRC servers like freenode are public have been there always.
* Allows you to create temporary channels for a user, allowing temporary groups between 2 or 3 users could create P2P applications point to point or M2M.
* There are IRC clients for almost all platforms, whether Android, Web, Python, C +, Arduino or Node-RED.
* The protocol guarantees the sending and receiving of messages between clients.

## Descripción
Pruebas iniciales de implementacion de cliente [IRC (Internet Chat Relay)](https://en.wikipedia.org/wiki/Internet_Relay_Chat) en ESP8266 12E 
permitiendo comunicacion por internet a travez Chat IRC, Existen muchos metodos, Plataformas y soluciones IoT para el control y monitoreo de los ESP8266, 
pero muy pocas son sencillas, basicas y sin complicaciones.Esta implementacion permite en pocos pasos controlar un
ESP8266 desde Internet de una manera muy sencilla sin configuraciones muy especialidas utilizando los existentes 
servidores [IRC](https://en.wikipedia.org/wiki/Internet_Relay_Chat)  publicos enviando simplemente mensajes entre dispositivos.

Contiene 2 Archivos Arduino IDE :

* Esp-IRCcontrol_v1.ino  - Main routine 

* _ IRCfuntions.ino  - Additional functions IRC 

###  Algunas ventajas
* Conexión Bidireccional a través de Internet sin registrarse a Aplicaciones, Token's o Plataformas, lo considero una aplicación IoT.
* No requiere Hosting, Direcciones IP Fijas publicas, VPN's, puertos especiales.
* La aplicacion es Respuesta por Peticion de cliente externo.
* Los servidores IRC como freenode son públicos han estado hay siempre.
* Permite crear Canales temporales por un usuario, permitiendo grupos temporales entre 2 o 3 usuarios podríamos crear aplicaciones P2P punto a punto o M2M.
* Existen clientes IRC para casi todas las plataformas sea Android, Web, Python, C+, Arduino o Node-RED.
* El protocolo garantiza el envió y recepción de los mensajes entre clientes.

## Based on / Basado en
* **arduinoirc for Uwe Kamper** - *Initial work* - [arduinoirc](https://github.com/uwekamper/arduinoirc)
Thanks for contributing  [Uwe Kamper](https://github.com/uwekamper).

## Materials / Materiales
* [ESP8266 12E NodeMCU](http://s.click.aliexpress.com/e/b6QNZfy) - Module Espressif
* [Sensor DHT11](http://s.click.aliexpress.com/e/b6QNZfy) - Relative Humidity and Temperature
* [Sensor DS18B20 Onewire](http://s.click.aliexpress.com/e/meqfuNv) - Temperature

## Mount / Montaje
![alt text](http://pdacontroles.com/wp-content/uploads/2018/04/mejor.jpg "PZEM-004 ESP8266 Blynk")

## Connections / Conexiones
Sensors:

```
  ESP8266 12E         DIODE LED
  GPIO12  (D6) <----> Anode
  GND          <----> Catode 
    
  ESP8266 12E         DHT11 Sensor
  3.3v         <----> VCC  PIN1
  GPIO14 (D5)  <----> DATA PIN2  
  GND          <----> GND  PIN4
  
  ESP8266 12E         DS18B20
  GPIO2  (D4)  <----> DATA
```
## Arduino IDE configuration
Parameters that require revision and configuration in Arduino IDE code:

Parametros que requieren revision y configuracion en codigo Arduino IDE:

### Security to Control ESP8266

ENABLE_USERPERMITTED = 1 
Allows the ESP8266 can **only be controlled by users or irc clients included in the Array "nick_users"**, and modify quantity in variable "users"

ENABLE_USERPERMITTED = 0 
Allows any of the irc users or clients in the same channel to control or send commands to our ESP8266.

### Seguridad para Controlar ESP8266

ENABLE_USERPERMITTED = 1 
Permite que el ESP8266  **solo pueda ser controlado por los usuarios o clientes irc incluidos en el Array "nick_users"**, y modificar cantidad en variable "users"

ENABLE_USERPERMITTED = 0 
Permite cualquiera de los usuarios o clientes irc en el mismo canal pueda controlar o enviar comandos nuestro ESP8266.

```
#define ENABLE_USERPERMITTED  1          //  1 =secure mode    0 = Unsafe way, everyone controls.
uint32_t users=2;                                       // Number of users Allowed to control ESP    
String nick_users[]={"PDUSER|xxxxxx", "PDUSER|0s046"};   //Allowed users to control ESP8266          
```
### Parameters Wifi Network
```
const char* ssid     = "************";
const char* password = "************";
```
### Parameters IRC Connection
In case of using in own application, create a new private channel, always placing # at the beginning of the name of the channel example #PDAChannel.

En caso de utilizar en aplicacion propia, crear un nuevo canal privado, colocando siempre # al inicio del nombre del canal ejemplo #PDAChannel. 
```
const char* host =   "chat.freenode.net";  // IRC Server  
uint32_t portirc =    6667;                // IRC Port 
String nickname  =   "ESP8266IRC";         // MAX 10 Characters - nickname module ESP
String channel   =   "#PDAChannel";        //  example "#TestIRCready"; 
```

## Test from IRC WEB client ... / Prueba desde cliente IRC WEB
We have assigned this IRC Web client to perform tests:          [ESP8266 IRCControl WEB IRC Client](http://pdacontrolen.com/irc/). 

Hemos destinado este cliente IRC Web para que realicen pruebas: [ESP8266 IRCControl Cliente WEB IRC](http://pdacontrolen.com/irc/). 

 1. Enter to [http://pdacontrolen.com/irc/](http://pdacontrolen.com/irc/) and Press the Green Button "Connecting ..." 
 
    Ingresar a [http://pdacontroles.com/irc/](http://pdacontroles.com/irc/) y Presionar el Boton Verde "Conectando ..."
![alt text](http://pdacontroles.com/wp-content/uploads/2018/04/WEBIRC1.png "PZEM-004 ESP8266 Blynk")

 2. Entering Channel "#PDAChannel!" with automatic user/nickname example: PDAUSER|31947. 
 
    Ingresando a Canal "#PDAChannel!" con usuario/nickname automatico ejemplo: PDAUSER|31947. 
![alt text](http://pdacontroles.com/wp-content/uploads/2018/04/WEBIRC2.png "PZEM-004 ESP8266 Blynk")

 3. Send example command   "HIGH GPIO12".
 
    Enviar comando ejemplo "HIGH GPIO12".    
![alt text](http://pdacontroles.com/wp-content/uploads/2018/04/WEBIRC3.png "PZEM-004 ESP8266 Blynk")

 4. See response of ESP8266IRCXXXXXX = "Turn ON: ON GPIO12".
 
    Ver respuesta de ESP8266IRCXXXXXX = "Turn ON : ON GPIO12".     
![alt text](http://pdacontroles.com/wp-content/uploads/2018/04/WEBIRC4.png "PZEM-004 ESP8266 Blynk")


## Available Commands / Comandos Disponibles 
Skip Comillas "" when writing commands in [irc web client](http://pdacontrolen.com/irc/). 

Omitir Comillas "" al escribir comandos en [cliente irc web](http://pdacontrolen.com/irc/). 
 
* Command: **"HIGH GPIOXX"**      example: "HIGH GPIO12" Turn on GPIO outputs
* Command: **"LOW GPIOXX"**       example:  "LOW GPIO12" Turn off GPIO outputs
* Command: **"READ ADC0"**
* Command: **"ESP SIGNAL?"**
* Command: **"ESP Temp?"**        Temperature Sensor - DHT11
* Command: **"ESP HR?"**          Relative humidity sensor - DHT11
* Command: **"ESP Temp2?"**       Temperature Sensor - DS18B20
* Command: **"ESP ?????"**
* Command: **"ESP8266IRCXXXXXX Help" or "Nicknameesp Help"** - list available commands
* Command: sad face = :(

## Documentation / Documentacion 
* Read Considerations, Recommendations and Suggestions complete documentation of the project in [ESP8266 IRCControl desde Internet  Interacion IoT v1.0  Part 1](http://pdacontrolen.com/meter-pzem-004-esp8266-platform-iot-blynk-app/). 

* Leer Consideraciones, Recomendaciones y sugerencias cocumentacion Completa del proyecto en  [ESP8266 IRCControl desde Internet  Interacion IoT v1.0  Part 1](http://pdacontroles.com/medidor-pzem-004-esp8266-plataforma-iot-blynk-app/). 

## Donations / Donaciones 
Collaborate this project and other projects in [PDAControl](http://pdacontrolen.com)  via [Paypal](https://www.paypal.me/pdacontrol). 

Respalde este proyecto y otros proyectos en [PDAControl](http://pdacontrolen.com)  mediante [Paypal](https://www.paypal.me/pdacontrol).

[![Support via PayPal](https://cdn.rawgit.com/twolfson/paypal-github-button/1.0.0/dist/button.svg)](https://www.paypal.me/pdacontrol)

## More about PDAControl / Mas sobre PDAControl
* [PDAControl English](http://pdacontrolen.com). 
* [PDAControl Español](http://pdacontroles.com). 
* [PDAControl Youtube Channel](https://www.youtube.com/channel/UCv1D6zrC0ZL0PSgM6tdEpPg/videos). 
* [Jhon_Control Twitter](https://twitter.com/Jhon_Control). 

