/* October 10 2016
 * David Solano Perez
 * https://www.facebook.com/pitufo.solanoperez
 * https://twitter.com/Pitufo94XD
 * davsope94@gmail.com
 */

/*NodeMCU pin-Function-Name-Arduino Pin
* 20 - TX GPIO1 Digital 1 
* 28 – D4 GPIO2 Digital 2 
* 21 – RX GPIO3 Digital 3 
* 30 – D2 GPIO4 Digital 4 
* 31 – D1 GPIO5 Digital 5 
* 24 – D6 GPIO12 Digital 12 
* 23 – D7 GPIO13 Digital 13 
* 25 – D5 GPIO14 Digital 14 
* 22 – D8 GPIO15 Digital 15 
* 32 – D0 GPIO16 – USER – WAKE Digital 16 
* 01 – A0 ADC0 - TOUT Analog A0 
*/

/* RC-522 to NodeMCU connection
RST     = GPIO4-D2
SDA(SS) = GPIO2-D4 
MOSI    = GPIO13-D7
MISO    = GPIO12-D6
SCK     = GPIO14-D5
GND     = GND
3.3V    = 3.3V
*/ 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MFRC522.h>

//Network Settings
const char* ssid = "yourSSID";
const char* password = "yourPassword";
const char* mqtt_server = "xx.xx.xx.xx"; //IP Server

WiFiClient espClient;
PubSubClient client(espClient);

#define RST_PIN 4 
#define SS_PIN 2  
MFRC522 mfrc522(SS_PIN, RST_PIN);

int green=5; //D1
int blue=16; //D0
int red=15; //D8

void setup() {
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(red,OUTPUT);
  digitalWrite(green,0);
  digitalWrite(blue,0);
  digitalWrite(red,0);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(500);
  SPI.begin();           
  mfrc522.PCD_Init();   
  Serial.println("RFID Ready!!");
  Serial.println("-----------------------------------");
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("Wifi Connected!!");
  Serial.println("IP Adress: ");
  Serial.println(WiFi.localIP());
  Serial.println("-----------------------------------");
}

int callback(char* topic, byte* payload, unsigned int length){
  digitalWrite(blue,0);
  Serial.println("Received: ");
  String Received = String((char*)payload);
  Serial.println(Received);
  if(Received=="Access"){ //MQTT Response o Broker
    digitalWrite(green,1);
    delay(3000);
    }
  if(Received=="NoAccess"){
    digitalWrite(red,1);
    delay(1000);
    }
  digitalWrite(verde,0);
  digitalWrite(rojo,0);
  Serial.println("-----------------------------------");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT Broker...");
    String clientId = "Principal_Door";
    if (client.connect(clientId.c_str())) {
      Serial.println("Conected!!");
      client.publish("Topic","Activo...");
      client.subscribe("Acceso");} //Topic to response 
    else {
      Serial.print("Fail= ");
      Serial.print(client.state()); //Check in #include PubSubClient Documentation
      Serial.println("Try again in 5 seconds");
      Serial.println("-----------------------------------");
      delay(5000);
    }
  }
}

void loop(){
  if (!client.connected()){reconnect();}
  client.loop();
  digitalWrite(blue,1);
  char Payload[20];
  String str;
  String payload;
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;}
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;}
  for(byte i=0; i<mfrc522.uid.size; i++) {
    str=String(mfrc522.uid.uidByte[i]);
    payload.concat(str);
  }
  payload.toCharArray(Payload,20);
  Serial.println("Sent: ");
  Serial.println(Payload);
  client.publish("Topic",Payload);
  digitalWrite(azul,0);
}
