/*
 * De acuerdo a la lectura del sensor de humedad del suelo
 * blink el led de aviso de riego
 */
bool infoRiego(int sensorHumedadSueloVal){
  
  int guinios; // número de guinios que hará elled de info

  if( sensorHumedadSueloVal > 1000){
      //no hay señal de led si esta bien regado
    } else if ( sensorHumedadSueloVal <= 650 && sensorHumedadSueloVal >= 450 ){
      guinios = 2;
      guiniaLed(guinios, 400, riegoLed);
    } else if ( sensorHumedadSueloVal < 450 ){
      guinios = 5;
      guiniaLed(guinios, 100, riegoLed);
    }

      return true;
      
  }

