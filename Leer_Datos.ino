#include <EEPROM.h>
#define NUMERODIAS 1 //14
struct DiaPersonas{
  int dia;
  int personasContadas;
  double personasCalculadas;
};
void setup() {
  int eeAddress = 0;

  Serial.begin(9600);
  Serial.println("Datos leidos de la EEPROM: ");
  DiaPersonas variableAuxiliar;
  for(int i = 1; i <= NUMERODIAS; i++){
   EEPROM.get(eeAddress, variableAuxiliar);
   Serial.print("Dia: ");
   Serial.print(variableAuxiliar.dia);
   Serial.print(", Personas contadas: ");
   Serial.print(variableAuxiliar.personasContadas);
   Serial.print(", Personas calculadas: ");
   Serial.println(variableAuxiliar.personasCalculadas);
   eeAddress += sizeof(DiaPersonas);
  }
}

void loop() {
  /* Empty loop */
}
