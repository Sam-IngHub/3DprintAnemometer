// incluir librerías
 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>
  //leer ctes 
  int Sensor = 3;
  int Rev = 0;
  int MaxRev;   //calcula picos máximos de hz
  int PrevRev; //calcula picos máximos de hz
  unsigned long prevMillis = 0;
  const long s = 1000;
  // 1000 = intervalo de 1 segundo
  //500  = intervalo de medio segundo

 // Constante de calibración (ejemplo: 2.5 m/s por Hz)
 // usa float porque puede contener decimales
  float k = 2.5;

 LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  pinMode(Sensor,INPUT);
  Serial.begin(9600);

  lcd.init();        // inicializa LCD
  lcd.backlight();   // enciende la luz de fondo
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Listo!");
}


void loop() {
  
    unsigned long currentMillis = millis();    
    if (currentMillis - prevMillis <= s) { //si no supera el intervalo de un segundo, contar vueltas
      if(digitalRead(Sensor)==HIGH){ // si prende el sensor cuenta un click
       Rev++;
       delay(10); //evita rebotes
       if (Rev>MaxRev) {
       PrevRev = max(Rev,MaxRev);
      }
      }
     } else { //si supera 1 segundo, reinicia
       float Hz = Rev;
       float v = k*Rev;

       Serial.print("Hz: ");
       Serial.println(Hz);
       Serial.print("   Velocidad [m/s]: ");
       Serial.println(v);


       // Mostrar en LCD
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Hz: ");
       lcd.print(Hz);
       lcd.setCursor(0,1);
       lcd.print("Vel: ");
       lcd.print(velocidad);
       lcd.print(" m/s");

       prevMillis = currentMillis;
       Rev = 0; 
    } 

    MaxRev = max(Rev,PrevRev);


    Serial.print("  - max: ");
    Serial.println(max(Rev,PrevRev));
    
}
  
