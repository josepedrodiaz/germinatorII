/*
 * guiña un led 
 * @param guinios ( int  - veces que se enciende)
 * @param periodo (int periodo entre guinios)
 */
bool guiniaLed(int guinios, int periodo){

          guinios = guinios * 2 - 1;
  
          for (int i=0; i <= guinios; i++){ // 3 blinks
            
            digitalWrite(led, !digitalRead(led));
            delay(periodo);
            
          }
          
          return true;
          
          }



