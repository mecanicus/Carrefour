
#include <EEPROM.h>
#include <LowPower.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

#define trigPin 7
#define echoPin 6
#define HORAACTIVACION 8
#define HORAAPAGADO 23
#define ANCHOCALLE 200
#define LONGITUDSENSOR 200
#define MINUTOGUARDADO1 15
#define MINUTOGUARDADO2 48

boolean diaNuevo = false;
int eeAddress = 0;
int personas = 0;
double personasCalculadas = 0;
double multiplicador = 1;
int distanciaMaxConteo = LONGITUDSENSOR;
double pastDistance;
double arrays[10];
int index = 0;
int aux2 = 0;
boolean infinito = false;

struct DiaPersonas{
  int dia;
  int personasContadas;
  double personasCalculadas;
};
void setup() {
  Serial.begin(9600);
  if (ANCHOCALLE >= LONGITUDSENSOR){
    multiplicador = ANCHOCALLE/LONGITUDSENSOR;
  }
  else{
    distanciaMaxConteo = ANCHOCALLE;
  }
  tmElements_t tm;
//Para reiniciar la escritura en la 1º posicion de memoria
/*
  if (RTC.read(tm)) {
  DiaPersonas inicializacion = {
         tm.Day,
         personas,
         personasCalculadas
  };
  EEPROM.put( eeAddress, inicializacion ); 
  }
  */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  
  compruebaHora();
}

void compruebaHora(){
  tmElements_t tm;

  if (RTC.read(tm)) {
    if (tm.Hour >= HORAACTIVACION && tm.Hour <= HORAAPAGADO){
      //Despierta
      //Serial.println("Estoy Vivo");
      if ((tm.Minute == MINUTOGUARDADO1 || tm.Minute == MINUTOGUARDADO2) && tm.Second == 30){
        DiaPersonas comprobarDia;
        EEPROM.get( eeAddress, comprobarDia );
        if(tm.Day != comprobarDia.dia){
          diaNuevo = true;
        } 
        DiaPersonas variableGuardada = {
         tm.Day,
         personas,
         personasCalculadas
        };
        
        if(diaNuevo == true){
          eeAddress += sizeof(DiaPersonas);
        }
        
        EEPROM.put( eeAddress, variableGuardada ); 
        Serial.println("Guardado");
    
        if(diaNuevo == true){
          diaNuevo = false;
        }
        delay(1000);  //Para que no se cumpla el if 2 veces en dos pasadas consecutivas
      }
     
      codigoSensor();
      personasCalculadas = personas*multiplicador;
      
    }
    else{
      //Duermete  
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(2000);
  }
}
void codigoSensor(){
  double duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  //Serial.println(distance);
  if (infinito == true){
    infinito = false;
    delay(18);
    return;
  }
  if (distance >= distanciaMaxConteo){
    if(distance >= 1000){
          infinito = true;
          }
    
       if (aux2 < 3){
          aux2++;
        }else{
          index = 0;
          pastDistance = 1000;
        }
        
        delay(18);
      
  }else{
      //Serial.println(distance);
       if (aux2 > 0){
         aux2--;
       }
      arrays[index] = distance;
      index++;
      if (index == 10){
        index = 0;
        double media = 0;
        double mediareal = 0;  
        for (int x = 0; x<10; x++){
          media = media + arrays[x]; 
        }
        media = media/10;
        int n = 0;
        for (int x = 0; x<10; x++){
          if ((arrays[x] >= media - 20) && (arrays[x] <= media + 20)){
            mediareal = mediareal + arrays[x]; 
            n++;
          }else{
          }
        }
        mediareal = mediareal/n;
        if (notSamePerson(mediareal)){
          pastDistance = mediareal;
          personas++;
          Serial.print("Número de personas: ");
          Serial.println(personas);
          delay(20);
        }else{
          delay(20);
        }
      }else{
        delay(18);
      }
  }
} 
boolean notSamePerson(double distance){
  if ((distance >= pastDistance - 20) && (distance <= pastDistance + 20)){
    return false;
  }
  else{
    return true;
  }
}