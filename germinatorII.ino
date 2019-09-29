/**
 GerminatorII for NodeMCU  based on Porruino <josepedrodiaz@gmail.com>
 Created on: 2019
**/
#include <ESP8266WiFi.h> // Basic Esp class
#include <ESP8266HTTPClient.h>// HTTP POST json to Ubidots
#include <FS.h> // SPIFFS class - Support for ESP file system
/* Web SERVER */
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

/******************** ONE WIRE DS18b20************************************************/
// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the ESP
#define ONE_WIRE_BUS D2
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire ds(ONE_WIRE_BUS);
/********************************************************************/






// /******************** Start Web Server on port 80 ************************************************/
// ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
// void handleRoot();              // function prototypes for HTTP handlers
// void handleNotFound();


unsigned long ultimo_update = 0;
//inicializa la variable que controla los millis() desde el ultimo posteo a ubidots
/*
   Estas variables manejan los períodos del loop y se tocan siempre
   para debuggear
*/
unsigned long periodo_update = 5000;
// Período de posteos a Ubidot en milisegundos 900.000 = 15 minutos

//Período de delay pre y post lectura del pin ADC para que no se sobrepongan las lecturas
int analogReadDelay = 100;


/*
******************** ASIGNACIÓN DE PINES ************************************************
*/
int adcPin = A0; //ADC (LDR/Sensor de humedad del suelo)
int pinSwitchLDR = D6; // El pin que activa el sensor LDR
int pinSwitchSensorHumedadSuelo = D3; // El pin que activa el sensor de humedad en suelo

// LEDs
int infoLed = D5; //info led
int riegoLed = D0; //info agua led
int deleteLed = D1; //Format info led

/*
******************** BOTONES ************************************************
*/

/*
 * Button on GPIO16 - Manda datos.csv al Serial
 */
 int button1 = D7;

/*
 * Button on GPIO14 - FORMATEA EL SISTEMA DE ARCHIVOS SPIFFS
 * Este botón está marcado con un cuadrado negro abajo en el protoboard (J/55 más o menos)
 */
int button2 = D4;

/*
   Flag para formatear la memoria SPIFFS
*/

int button2State = 0; // button to format

void setup() {
  /*
     General Setup
  */

  // Init LEDs
//   pinMode(infoLed, OUTPUT); // led de info
  pinMode(riegoLed, OUTPUT); // led de agua info
  pinMode(deleteLed, OUTPUT); // le de delete SPIFFS info

  // Init buttons
  pinMode(button2, INPUT);
  pinMode(button1, INPUT);

//   //GPIOs
//   pinMode(pinSwitchSensorHumedadSuelo, OUTPUT);
//   pinMode(pinSwitchLDR, OUTPUT);

  //Start Serial
  Serial.begin(115200);
  Serial.print("Serial port is initialized");


//   //Start SPIFFS file system
  SPIFFS.begin();
  Serial.print("SPIFSS is initialized");

}

void loop() {

 







  /*
     FORMATEA SPIFFS al presionar button 2
  */

  // read the state of the pushbutton value:
//   button2State = digitalRead(button2);
//   if (button2State == HIGH) {

//     /*
//        Format SPIFFS
//     */
//     Serial.println("Please wait 30 secs for SPIFFS to be formatted");
//    //  SPIFFS.format();
//     Serial.println("Spiffs formatted");
//     // @TODO turn LED on @see ticker()
    
//   } else {

//     // turn LED off:
//     digitalWrite(deleteLed, LOW);

//   }

//arbitrariamente obetner los milllis aqui 
  unsigned long milis = millis();

  if ((milis - ultimo_update) >= periodo_update) { //timer


    ultimo_update = millis();//update timer


    /******************** Write LOG  ************************************************/

    /*
       Log data generation
    */


   //  int sensorHumedadSueloVal = adcRead(pinSwitchSensorHumedadSuelo); //read Sensor humedad val

   //  int sensorLDRVal = adcRead(pinSwitchLDR); //read Sensor LDR

   //  float temp = getTemp(); // get sensor Temp DS18b20

   //  String tempToPost = String(temp); // Temperature to post to Ubidots

   //  /*
   //     Proceso las señales visuales (leds) si se necesita riego
   //  */
   //  if (!infoRiego(sensorHumedadSueloVal)) {
   //    Serial.println("");
   //    Serial.println("ERROR procesando info de Riego");
   //  }

    /*
       Print sensor values to Serial
    */
   //  Serial.println("");
     Serial.println("Info de Sensores");
   //  Serial.println("LDR val: " + String(sensorLDRVal));
   //  Serial.println("Sensor Humedad val: " + String(sensorHumedadSueloVal));
   //  Serial.println("DS18b20 val: " + tempToPost);
   //  Serial.println("");

    /*
       Log data to an csv (SPIFFS)
    */
   //  String logTxt = getUptime() + ", " + tempToPost + ", " + sensorHumedadSueloVal + ", " + sensorLDRVal;
   //  logData(logTxt , "W");





   //  /******************** wait for WiFi connection  ************************************************/
   //  if ((WiFiMulti.run() == WL_CONNECTED)) {




   //    /******************** Arma String de posteo http a Ubidots  ************************************************/
   //    String postUrl = "http://things.ubidots.com/api/v1.6/collections/values"  ;
   //    // URL base de Ubidots para postear la colección de valores

   //    //Formato para postear colecciones (múltiples variables) >> '[{"variable": "547518da762542016381b2e9", "value":12}, {"variable": "547518e27625427f85fca05d", "value":43}]'
   //    String postValues = "[{\"variable\": \"5a0cc810c03f977f9aee886c\", \"value\":" + String(sensorLDRVal) + "},"; //variable Sensor LDR
   //    postValues += "{\"variable\": \"5a1f786ec03f972f26b5c9a1\", \"value\":" + String(sensorHumedadSueloVal) + "},"; //variable Sensor humedad del suelo
   //    postValues +=  "{\"variable\": \"5a051dccc03f97100985b714\", \"value\":" + String(millis()) + "},"; //variable Millis
   //    postValues +=  "{\"variable\": \"5a183b39c03f975427e80c5e\", \"value\":" + tempToPost + "}]"; //variable Temperatura DS18b20



   //    int content_lenght = postValues.length();
   //    //cuenta el total de caracteres del String del POST para enviarlo como contenido del Header


   //    /******************** Ubidots Device Post ************************************************/
   //    HTTPClient http;
   //    http.begin(postUrl);
   //    http.addHeader("Content-Type", "application/json");
   //    http.addHeader("X-Auth-Token", "A1E-atyjY8PBDzlwd99sAf0o5TJ49O0dc0");
   //    http.addHeader("Content-Length", String(content_lenght));
   //    http.POST(postValues);
   //    http.writeToStream(&Serial);
   //    http.end();


   //    /*
   //       guiña un led
   //       @param guinios ( int  - veces que se enciende)
   //       @param periodo (int - periodo entre guinios)
   //       @param ledId (str - cadena identificadora del led de información que guiñará)
   //    */
   //    guiniaLed(10, 100, infoLed);//avisa que posteó a Ubidots

   //  }//End Wait for Wifi Connection

  }//end timer between posts to Ubidots

}//End Loop
