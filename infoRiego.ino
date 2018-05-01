/*
 * De acuerdo a la lectura del sensor de humedad del suelo
 * blink el led de aviso de riego
 */
bool infoRiego(int sensorHumedadSueloVal){
  
  int guinios; // número de guinios que hará el led de info

  //Las mediciones en la maceta arrojan resultados entre 70 y 556 
  // pero por debajo de 420 ya esta muy seca la tierra
  
  if( sensorHumedadSueloVal > 420){
      //no hay señal de led si esta bien regado
      digitalWrite(riegoLed, LOW); 
    } else if ( sensorHumedadSueloVal <= 420 && sensorHumedadSueloVal >= 400 ){
      guinios = 10;
      guiniaLed(guinios, 400, riegoLed);
    } else if ( sensorHumedadSueloVal < 400 ){
      digitalWrite(riegoLed, HIGH); 
    }

      return true;
      
  }

