#include <LiquidCrystal.h>
//leer ctes 
int Sensor = 6;
int Rev = 0;
int MaxRev;   //calcula picos máximos de hz
int PrevRev; //calcula picos máximos de hz
int Hz;
int v;
unsigned long prevMillis = 0;
const long s = 1000;
  // 1000 = intervalo de 1 segundo
  //500  = intervalo de medio segundo

// Constante de calibración (ejemplo: 2.5 m/s por Hz)
// usa float porque puede contener decimales
float k = 2.5;

//pantalla
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
  
  pinMode(Sensor,INPUT);
  Serial.begin(9600);

  lcd.begin(16,2);
  lcd.print("Listo!");

}


void loop() {
  
  unsigned long currentMillis = millis();    
  if (currentMillis - prevMillis <= s) { //si no supera el intervalo de un segundo, contar vueltas
    if(digitalRead(Sensor)==HIGH){ // si prende el sensor cuenta un click
      Rev++;
      // delay(10); //evita rebotes
      if (Rev>MaxRev) {
       PrevRev = max(Rev,MaxRev);
      }
    }
  } else { //si supera 1 segundo, reinicia
      float v = k*Rev;

      Serial.print("Hz: ");
      Serial.println(Rev);
      Serial.print("   Velocidad [m/s]: ");
      Serial.println(v);


       // Mostrar en LCD
       lcd.setCursor(0,0);
       lcd.print("Hz: ");
       lcd.print(Rev);
       lcd.setCursor(0,1);
       lcd.print("Vel [m/s]: ");
       lcd.print(v);

      prevMillis = currentMillis;
      Rev = 0; 
    } 

  //MaxRev = max(Rev,PrevRev);
  //Serial.print("  - max: ");
  //Serial.println(max(Rev,PrevRev));
  
   
  lcd.noCursor();

  delay(500);

  // Turn on the cursor:

  lcd.cursor();

  delay(500);

}
  
