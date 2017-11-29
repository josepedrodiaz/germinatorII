/******************** LOG ************************************************/
/* Vamos a escribir en un archivo txt el registro de la temperatura y de la luz, 
 *  para mantenerlo aun estando offline.
 */
bool logData(String data, char* mod){

  Serial.println(" ");//Just a line break
  
  // open file for writing
  File f = SPIFFS.open("/data.csv", "a+"); // "a+" Opens a file for reading and appending.
  
  if (!f) {
      Serial.println("file open failed (a)");
  }
  
  if(mod == "W"){
  
    Serial.println("====== Writing to SPIFFS file ========="); // Serial info   
    f.println(data);//write data to file
    Serial.println("Escrito: " + data); // Serial info
    
  }else if (mod == "R"){
     
        Serial.println("====== Reading from SPIFFS file data.csv =======");
        /*
         * Señal visible (parpadea Led)
         */
        for(int i = 10; i > 0; i--){
          guiniaLed(1,i*10);
          }
  
        // write i strings to Serial
        for (int i=1; i<=700; i++){
          String s=f.readStringUntil('\n');
          int stringLength = s.length();
          if( stringLength > 1 ){
              //Serial.print(i);
              //Serial.print(":");
              Serial.println(s);
            } else {
            Serial.print(".");  
            }
        }

       /*
         * Señal visible (parpadea Led)
         */
        for(int i = 10; i > 0; i--){
          guiniaLed(1,i*10);
          }
        
      
  }
      
      
      
  
      f.close();

      return true;
  }

  
