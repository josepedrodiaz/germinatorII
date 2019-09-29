/*
 * guiña un led 
 * @param guinios ( int  - veces que se enciende)
 * @param periodo (int periodo entre guinios)
 */
bool guiniaLed(int guinios, int periodo, int led){

          guinios = guinios * 2 - 1;
  
          for (int i=0; i <= guinios; i++){ // 3 blinks
            
            digitalWrite(led, !digitalRead(led));
            delay(periodo);
            
          }
          
          return true;
          
         }

/*
 * Esta funcion lee y devuelve el valor del pin ADC analog(0)
 */
int adcRead(int pin){
  
  int val;

  digitalWrite(pin, HIGH);
  delay(analogReadDelay);
  val = analogRead(adcPin);
  delay(analogReadDelay);
  digitalWrite(pin, LOW);
  
  return val;
  }



