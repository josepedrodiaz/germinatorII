/**
 Porruino <josepedrodiaz@gmail.com>
 Created on: 2017
**/
#include <ESP8266WiFi.h> // Basic Esp class
#include <ESP8266WiFiMulti.h> //Save múltiple APs access Data
#include <ESP8266HTTPClient.h>// HTTP POST json to Ubidots
#include <FS.h> // SPIFFS class
/* Web SERVER */
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

/******************** ONE WIRE DS18b20************************************************/
// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the ESP
#define ONE_WIRE_BUS 2
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire ds(ONE_WIRE_BUS);
/********************************************************************/



/******************** WIFI client multiple APs ************************************************/
ESP8266WiFiMulti WiFiMulti;
// Inicializa la clase que permite configurar multiples APs
// Se conecta a la red con mayor nivel de señal


/******************** Start Web Server on port 80 ************************************************/
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();


unsigned long ultimo_update = 0;
//inicializa la variable que controla los millis() desde el ultimo posteo a ubidots
/*
   Estas variables manejan los períodos del loop y se tocan siempre
   para debuggear
*/
unsigned long periodo_update = 60000;
// Período de posteos a Ubidot en milisegundos 900.000 = 15 minutos

//Período de delay pre y post lectura del pin ADC para que no se sobrepongan las lecturas
int analogReadDelay = 100;


/*
******************** ASIGNACIÓN DE PINES ************************************************
*/
int adcPin = A0; //ADC (LDR/Sensor de humedad del suelo)
int pinSwitchLDR = 15; // El pin que activa el sensor LDR
int pinSwitchSensorHumedadSuelo = 13; // El pin que activa el sensor de humedad en suelo

// LEDs
int infoLed = 5; //info led
int riegoLed = 0; //info agua led
int deleteLed = 12; //Format info led

/*
******************** BOTONES ************************************************
*/

/*
 * Button on GPIO16 - Manda datos.csv al Serial
 */
 int button1 = 16;

/*
 * Button on GPIO14 - FORMATEA EL SISTEMA DE ARCHIVOS SPIFFS
 * Este botón está marcado con un cuadrado negro abajo en el protoboard (J/55 más o menos)
 */
int button2 = 14;

/*
   Flag para formatear la memoria SPIFFS
*/

int button2State = 0; // button to format

void setup() {
  /*
     General Setup
  */

  // Init LEDs
  pinMode(infoLed, OUTPUT); // led de info
  pinMode(riegoLed, OUTPUT); // led de agua info
  pinMode(deleteLed, OUTPUT); // le de delete SPIFFS info

  // Init buttons
  pinMode(button2, INPUT);
  pinMode(button1, INPUT);

  //GPIOs
  pinMode(pinSwitchSensorHumedadSuelo, OUTPUT);
  pinMode(pinSwitchLDR, OUTPUT);

  //Start Serial
  Serial.begin(115200);

  //Add routers/APs
  WiFiMulti.addAP("Cisco68896", "elperrococoliso"); // Router principal @casita
  WiFiMulti.addAP("CeluP", "cocoliso"); // AP Mobile Pedro
  WiFiMulti.addAP("cocina", "gatolagarto"); // Router cocina
  WiFiMulti.addAP("Dormi Nieco", "gatolagarto"); // Router Dormitorio Nieco

  //Start SPIFFS file system
  SPIFFS.begin();


  // WEB SERVER
  Serial.println("Connecting ...");
  int i = 0;
  while (WiFiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.serveStatic("/data.csv", SPIFFS, "/data.csv"); //Apunta una URL a un archivo del sistema de archivos SPIFFS
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop() {

  /*
     WEB Server
  */
  server.handleClient();                    // Listen for HTTP requests from clients



  /*
     LOG DATA on SPIFFS
    Imprime el log en el puerto Serial
  */
  boolean button1State = digitalRead(button1);

  if (button1State == HIGH) { //al presionar button1 escribe data loggeada en el puerto serie

    logData("", "R");
    delay(100);

  }




  /*
     FORMATEA SPIFFS al presionar button 2
  */

  // read the state of the pushbutton value:
  button2State = digitalRead(button2);
  if (button2State == HIGH) {
    /*
       Format SPIFFS
    */
    Serial.println("Please wait 30 secs for SPIFFS to be formatted");
    SPIFFS.format();
    Serial.println("Spiffs formatted");
    // turn LED on:
    digitalWrite(deleteLed, HIGH);
    delay(500);
    digitalWrite(deleteLed, LOW);
  } else {
    // turn LED off:
    digitalWrite(deleteLed, LOW);
  }

  unsigned long milis = millis();

  if ((milis - ultimo_update) >= periodo_update) { //timer


    ultimo_update = millis();//update timer


    /******************** Write LOG  ************************************************/

    /*
       Log data generation
    */


    int sensorHumedadSueloVal = adcRead(pinSwitchSensorHumedadSuelo); //read Sensor humedad val

    int sensorLDRVal = adcRead(pinSwitchLDR); //read Sensor LDR

    float temp = getTemp(); // get sensor Temp DS18b20

    String tempToPost = String(temp); // Temperature to post to Ubidots

    /*
       Proceso las señales visuales (leds) si se necesita riego
    */
    if (!infoRiego(sensorHumedadSueloVal)) {
      Serial.println("");
      Serial.println("ERROR procesando info de Riego");
    }

    /*
       Print sensor values to Serial
    */
    Serial.println("");
    Serial.println("Info de Sensores");
    Serial.println("LDR val: " + String(sensorLDRVal));
    Serial.println("Sensor Humedad val: " + String(sensorHumedadSueloVal));
    Serial.println("DS18b20 val: " + tempToPost);
    Serial.println("");

    /*
       Log data to an csv (SPIFFS)
    */
    String logTxt = getUptime() + ", " + tempToPost + ", " + sensorHumedadSueloVal + ", " + sensorLDRVal;
    logData(logTxt , "W");





    /******************** wait for WiFi connection  ************************************************/
    if ((WiFiMulti.run() == WL_CONNECTED)) {




      /******************** Arma String de posteo http a Ubidots  ************************************************/
      String postUrl = "http://things.ubidots.com/api/v1.6/collections/values"  ;
      // URL base de Ubidots para postear la colección de valores

      //Formato para postear colecciones (múltiples variables) >> '[{"variable": "547518da762542016381b2e9", "value":12}, {"variable": "547518e27625427f85fca05d", "value":43}]'
      String postValues = "[{\"variable\": \"5a0cc810c03f977f9aee886c\", \"value\":" + String(sensorLDRVal) + "},"; //variable Sensor LDR
      postValues += "{\"variable\": \"5a1f786ec03f972f26b5c9a1\", \"value\":" + String(sensorHumedadSueloVal) + "},"; //variable Sensor humedad del suelo
      postValues +=  "{\"variable\": \"5a051dccc03f97100985b714\", \"value\":" + String(millis()) + "},"; //variable Millis
      postValues +=  "{\"variable\": \"5a183b39c03f975427e80c5e\", \"value\":" + tempToPost + "}]"; //variable Temperatura DS18b20



      int content_lenght = postValues.length();
      //cuenta el total de caracteres del String del POST para enviarlo como contenido del Header


      /******************** Ubidots Device Post ************************************************/
      HTTPClient http;
      http.begin(postUrl);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-Auth-Token", "A1E-atyjY8PBDzlwd99sAf0o5TJ49O0dc0");
      http.addHeader("Content-Length", String(content_lenght));
      http.POST(postValues);
      http.writeToStream(&Serial);
      http.end();


      /*
         guiña un led
         @param guinios ( int  - veces que se enciende)
         @param periodo (int - periodo entre guinios)
         @param ledId (str - cadena identificadora del led de información que guiñará)
      */
      guiniaLed(10, 100, infoLed);//avisa que posteó a Ubidots

    }//end timer between posts to Ubidots

  }//End Wait for Wifi Connection

}//End Loop
