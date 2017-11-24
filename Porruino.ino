
/**
   Porruino <josepedrodiaz@gmail.com>

   Created on: 2017
 

*/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>


/******************** ONE WIRE DS18b20************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 


#define USE_SERIAL Serial


ESP8266WiFiMulti WiFiMulti; 
// Se pueden configurar multiples APs y se conecta a la red con mayor nivel de señal

unsigned long ultimo_update = 0;
//inicializa la variable que controla el ultimo posteo
unsigned long periodo_update = 9000; 
// Período de posteos a Ubidot900000 = 15 minutos 


int sensorPin = A0; //ADC

int led = 5; //info led



void setup() {

  pinMode(led, OUTPUT);

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  //Add routers/APs
  WiFiMulti.addAP("Cisco68896", "elperrococoliso");

  
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    unsigned long milis = millis();
    
    if((milis - ultimo_update) >= periodo_update){//timer
  
        ultimo_update = millis();

        int LDRval = analogRead(sensorPin);

        String temp = String(DS18B20.getTempCByIndex(0));

        Serial.println("LDR val: "+ String(LDRval));
      
        int content_lenght = String(milis).length();//prepara el valor paostear

        String postUrl = "http://things.ubidots.com/api/v1.6/collections/values"  ;
        // URL para postear de una variable: http://things.ubidots.com/api/v1.6/variables/5a0cc810c03f977f9aee886c/values

        //Formato para postear colecciones (múltiples variables) >> '[{"variable": "547518da762542016381b2e9", "value":12}, {"variable": "547518e27625427f85fca05d", "value":43}]'
        String postValues = "[{\"variable\": \"5a0cc810c03f977f9aee886c\", \"value\":" + String(LDRval) + "},"; //variable Sensor LDR
        postValues +=  "{\"variable\": \"5a051dccc03f97100985b714\", \"value\":" + String(millis()) + "},"; //variable Millis
        postValues +=  "{\"variable\": \"5a183b39c03f975427e80c5e\", \"value\":" + temp + "}]"; //variable Temperatura DS18b20

 /******************** Ubidots Device Post ************************************************/       
        HTTPClient http;
        http.begin(postUrl);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-Auth-Token", "A1E-atyjY8PBDzlwd99sAf0o5TJ49O0dc0");
        http.addHeader("Content-Length", String(content_lenght));
        http.POST(postValues);
        //http.POST("{\"variable\":\"5a0cc810c03f977f9aee886c\",\"value\":\""+String(LDRval)+"\"}");
        http.writeToStream(&Serial);
        http.end();

        //blink
        for (int i=0; i <= 5; i++){ // 3 blinks
          digitalWrite(led, !digitalRead(led));
          delay(500);
           }
      }
  }
}
