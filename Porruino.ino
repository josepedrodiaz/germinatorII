/**
   Porruino <josepedrodiaz@gmail.com>

   Created on: 2017
*/
#include <Arduino.h> // Basic Arduino IDE Esp class ????
#include <ESP8266WiFi.h> // Basic Esp class
#include <ESP8266WiFiMulti.h> //Save ultiple APs access Data
#include <ESP8266HTTPClient.h>// HTTP POST json to Ubidots
#include <FS.h> // SPIFFS class


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
// Referencia a la librería que permite configurar multiples APs 
// Se conecta a la red con mayor nivel de señal


unsigned long ultimo_update = 0;
//inicializa la variable que controla la hora del ultimo posteo
unsigned long periodo_update = 900000;
// Período de posteos a Ubidot en milisegundos 900000 = 15 minutos 


int sensorPin = A0; //ADC (LDR)
int led = 5; //info led
int button1 = 16;//Button on GPIO16 - Manda datos.csv al Serial

/*
 * FORMAT variables
 */
int led2 = 12; //Format info led
int button2 = 14;//Button on GPIO14 - FORMAT SPIFFS
int button2State = 0; 

void setup() {

  pinMode(led, OUTPUT); // led de info
  pinMode(button1, INPUT);//button1 attached to pin 4
  // initialize the LED2 pin as an output:
  pinMode(led2, OUTPUT);
  // initialize the pushbutton2 pin as an input:
  pinMode(button2, INPUT);
  Serial.begin(115200);

  //Add routers/APs
  WiFiMulti.addAP("Cisco68896", "elperrococoliso"); // Router @casita
  WiFiMulti.addAP("CeluP", "cocoliche"); // AP Mobile Pedro

  SPIFFS.begin();//start SPIFFS file system
  
}

void loop() {

  /*
   * LOG DATA on SPIFFS
    Imprime el log en el puerto Serial
   */
   boolean button1State = digitalRead(button1);

   if(button1State == HIGH){//al presionar button1 escribe data en el puerto serie
      
     logData("", "R");
     delay(100);
     
   }

   /*
    * FORMATEA SPIFFS
    */

    // read the state of the pushbutton value:
    button2State = digitalRead(button2);
    if (button2State == HIGH) {
       /*
        * Format SPIFFS
        */
       Serial.println("Please wait 30 secs for SPIFFS to be formatted");
       SPIFFS.format();
       Serial.println("Spiffs formatted");
    // turn LED on:
      digitalWrite(led2, HIGH);
    } else {
      // turn LED off:
      digitalWrite(led2, LOW);
    }

    unsigned long milis = millis();
        
    if((milis - ultimo_update) >= periodo_update){//timer  


      ultimo_update = millis();//update timer
      

      /******************** Write LOG  ************************************************/       

        /*
         * Log data generation
         */

         int LDRval = analogRead(sensorPin); //read LDR val

         float temp = getTemp(); // get sensor Temp DS18b20
         
         String tempToPost = String(temp); // Temperature to post to Ubidots

        /*
         * Print sensor values to Serial
         */
        Serial.println("");
        Serial.println("Info de Sensores");
        Serial.println("LDR val: "+ String(LDRval));
        Serial.println("DS18b20 val: "+ tempToPost);
        Serial.println("");
      
        /*
         * Log data to an csv (SPIFFS)
         */
        String logTxt = "\"" + getUptime() + "\", \"" + tempToPost + "\", \"" + LDRval + "\"";
        
        logData(logTxt , "W");
   




/******************** wait for WiFi connection  ************************************************/       
  if ((WiFiMulti.run() == WL_CONNECTED)) {


  
     
/******************** Arma String de posteo http a Ubidots  ************************************************/       
        String postUrl = "http://things.ubidots.com/api/v1.6/collections/values"  ;
        // URL base de Ubidots para postear la colección de valores

        //Formato para postear colecciones (múltiples variables) >> '[{"variable": "547518da762542016381b2e9", "value":12}, {"variable": "547518e27625427f85fca05d", "value":43}]'
        String postValues = "[{\"variable\": \"5a0cc810c03f977f9aee886c\", \"value\":" + String(LDRval) + "},"; //variable Sensor LDR
        postValues +=  "{\"variable\": \"5a051dccc03f97100985b714\", \"value\":" + String(millis()) + "},"; //variable Millis
        postValues +=  "{\"variable\": \"5a183b39c03f975427e80c5e\", \"value\":" + tempToPost + "}]"; //variable Temperatura DS18b20


        int content_lenght = postValues.length();
        //cuenta el total de caracteres del String del POST para enviarlo com ocontenido del Header
        

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
         * guiña un led 
         * @param guinios ( int  - veces que se enciende)
         * @param periodo (int periodo entre guinios)
         */
        guiniaLed(100,100);//avisa que posteó a Ubidots
        
  
      }//end timer between posts to Ubidots
      
    }//End Wait for Wifi Connection
    
  }
