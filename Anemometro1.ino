/*
  Anemómetro de copas - conteo de vueltas por interrupción
  ----------------------------------------------------------
  - 1 pulso (flanco de subida) = 1 vuelta del eje
  - Se cuenta 1 sola vez por activación del sensor (no mientras se
    mantiene en HIGH), gracias a attachInterrupt(..., RISING) + debounce
  - Calcula Hz instantáneo (por ventana) y Hz medio (media móvil)
  - Convierte Hz a velocidad de viento [m/s] usando la constante k
  - Salida por Serial en formato "etiqueta:valor" apta para graficar
    en tiempo real (Serial Plotter del IDE, o un script Python/Processing)

  IMPORTANTE - a revisar según tu hardware real:
  - Sensor debe ir conectado a un pin con soporte de interrupción
    (en Arduino Uno/Nano: pines 2 o 3)
  - INPUT_PULLUP asume un sensor tipo "contacto a GND" (reed switch,
    hall effect en colector abierto, etc). Si tu sensor entrega su
    propia señal HIGH/LOW activa, puede que necesites INPUT simple
    (sin pull-up) o incluso invertir la lógica (FALLING en vez de RISING)
  - debounceMicros: ajustalo probando. Empezá con 2000 (2 ms) y subilo
    si ves conteos falsos a bajas revoluciones, o bajalo si a alta
    velocidad de giro estás perdiendo pulsos reales
*/

// ---------- Configuración de pines y constantes ----------
const int Sensor = 2;                    // pin con soporte de interrupción (2 o 3 en Uno/Nano)

const unsigned long intervalMs   = 1000; // ventana de medición del Hz instantáneo (ms)
const unsigned long debounceMicros = 2000; // tiempo mínimo entre pulsos válidos (us)

const int N_MEDIA = 10;                  // cantidad de ventanas usadas para el Hz medio (media móvil)

// Constante de calibración: velocidad [m/s] = k * frecuencia [Hz]
// Si tu anemómetro tiene un umbral de arranque, podrías necesitar
// una fórmula tipo v = k*Hz + offset -> ajustar en calcularVelocidad()
float k = 0.15;

// ---------- Variables compartidas con la interrupción ----------
volatile unsigned long pulseCount = 0;     // pulsos acumulados en la ventana actual
volatile unsigned long lastPulseMicros = 0; // marca de tiempo del último pulso válido (debounce)

// ---------- Variables de temporización y cálculo ----------
unsigned long prevMillis = 0;

// Buffer circular para la media móvil de Hz
float bufferHz[N_MEDIA];
int idxBuffer = 0;
bool bufferLleno = false;

// ---------- ISR: se ejecuta en cada flanco de subida del sensor ----------
void contarPulso() {
  unsigned long ahora = micros();
  if (ahora - lastPulseMicros > debounceMicros) {
    pulseCount++;
    lastPulseMicros = ahora;
  }
}

// ---------- Conversión Hz -> m/s ----------
float calcularVelocidad(float hz) {
  return k * hz;
  // Si necesitás offset por umbral de arranque, usar:
  // return k * hz + offset;
}

// ---------- Actualiza la media móvil y devuelve el promedio actual ----------
float actualizarMedia(float hzInstantaneo) {
  bufferHz[idxBuffer] = hzInstantaneo;
  idxBuffer++;
  if (idxBuffer >= N_MEDIA) {
    idxBuffer = 0;
    bufferLleno = true;
  }

  int cantidad = bufferLleno ? N_MEDIA : idxBuffer;
  if (cantidad == 0) return 0.0; // todavía no hay datos

  float suma = 0.0;
  for (int i = 0; i < cantidad; i++) {
    suma += bufferHz[i];
  }
  return suma / cantidad;
}

void setup() {
  pinMode(Sensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Sensor), contarPulso, RISING);

  Serial.begin(9600);

  for (int i = 0; i < N_MEDIA; i++) {
    bufferHz[i] = 0.0;
  }

  prevMillis = millis();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= intervalMs) {

    // Sección crítica: copiar y resetear el contador sin que la ISR
    // interfiera a mitad de la operación
    noInterrupts();
    unsigned long count = pulseCount;
    pulseCount = 0;
    interrupts();

    float elapsedSec = (currentMillis - prevMillis) / 1000.0;
    if (elapsedSec <= 0) elapsedSec = intervalMs / 1000.0; // resguardo por si acaso

    float hzInstantaneo = count / elapsedSec;
    float hzMedio = actualizarMedia(hzInstantaneo);

    float velInstantanea = calcularVelocidad(hzInstantaneo);
    float velMedia = calcularVelocidad(hzMedio);

    // Formato limpio, sin texto decorativo, apto para graficar:
    Serial.print("HzInst:");
    Serial.print(hzInstantaneo, 3);
    Serial.print(",HzMedio:");
    Serial.print(hzMedio, 3);
    Serial.print(",VelInst:");
    Serial.print(velInstantanea, 3);
    Serial.print(",VelMedia:");
    Serial.println(velMedia, 3);

    prevMillis = currentMillis;
  }
}
