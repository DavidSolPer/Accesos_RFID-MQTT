# Accesos_RFID-MQTT
PROYECTO DE GESTIÓN DE ACCESOS A EDIFICIOS MEDIANTE RFID y MQTT.
El Hardware base utilizado para el proyecto es la placa de desarrollo NodeMCU, ésta placa incluye el ESP8266 por lo que es
posible conectarse vía Wifi a un servidor. Se trata de una tarjeta muy versátil, ya que cuenta con GPIOS con distintas
funcionalidades, es posible utilizarlos como entradas y salidas, se puede generar una señal PWM, cuenta con un ADC, soporta
UART, ISP, I²C, etc... Además de ser muy asequible, con un precio de menos de 8 dólares. El sensor RFID utilizado será el 
RC-522.
El NodeMCU se puede programar el Lua, cargando un Firmware a la memoria Flash que hará las veces de intérprete. También es
posible programarlo en una implementación de Python llamada Micropython y en lenguaje Arduino. Cuando se usa un intérprete de
Lua se puede cargar un Firmware completo o se puede generar un Firmware "Custom" (https://nodemcu-build.com/) con las
funcionalidades o recursos que se deseen habilitar para usar en el proyecto, para cargarlo con alguna herramienta como
ESPtool Master (https://github.com/themadinventor/esptool), ésto es muy útil ya que permite optimizar la poca memoria que
posee el NodeMCU. En mi caso yo utilicé lenguaje Arduino para la programación, ya que no es necesario cargar un Firmware
previamente, la compilación genera un fichero .bin que se graba desde la posición 0x00 de Flash. 
En cada uno de los accesos o puertas se montará un punto de acceso que consiste en el NodeMCU, el módulo RFID, un led RGB
para visualizar el estado (Azul= Esperando TAG, Verde=Aceptado, Rojo= Rechazado), una etapa de potencia para accionar el
Solenoide de una chapa electromagnética, y la fuente de alimentación para todo el circuito. La etapa de potencia cambia de
estado dependiendo de la señal que llega al pin del led verde del RGB.
Para la gestión de los accesos se utiliza MQTT, un protocolo de Iot utilizado para la comunicación entre dispositivos
mediante mensajes cortos de la forma:
Tópico/Mensaje
Los mensajes son gestionados por algún servidor o también llamado Broker de MQTT, el que yo utilizo es Mosquitto
(https://mosquitto.org/), en éste se debe de subscribir a un tópico general de los puntos de acceso para recibir sólo éstos 
mensajes, comprobar los UID que envía el NodeMCU en una base de datos y regresar un mensaje para que el NodeMCU evalúe si
se permitirá el acceso del usario o no. 

NOTAS IMPORTANTES:
> Dependiendo del modelo de NodeMCU necesitarás pulsar el botón FLASH de la placa cada vez que requieras cargar el programas.
O si quieres montar el proyecto en un módulo ESP, es necesario utilizar un convertidor USB-Serial con una ALIMENTACIÓN DE 3.3
Vcc y relizar un puente entre GPIO0 y GND para programar el módulo.
> Es necesario contar con una versión del IDE de Arduino 1.6.11 o superior y agregar soporte para los módulos ESP.
