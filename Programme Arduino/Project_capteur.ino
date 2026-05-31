#include <Wire.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

//----------------- BlueTooth--------------------
#include <SoftwareSerial.h>
#define pinBT_TXD 8
#define pinBT_RXD 7
SoftwareSerial BTSerial(pinBT_RXD, pinBT_TXD); //
bool bluetoothMeasureGraphite = false;
bool bluetoothMeasureFlex = false;
unsigned long lastSampleMs = 0;
// ---------------- Encoder pins ----------------
#define encoder0PinA 2   // Encoder CLK (interrupt pin)
#define encoder0PinB 4  // Encoder DT
#define Switch 5       // Encoder push button

volatile int encoder0Pos = 0; // Updated inside ISR

// ---------------- OLED config ----------------
#define OLED_W 128
#define OLED_H 64
#define OLED_RST -1
#define OLED_ADDR 0x3C
Adafruit_SSD1306 ecranOLED(OLED_W, OLED_H, &Wire, OLED_RST);

// ---------------- Digital potentiometer / R2 model ----------------
// Note: This code only computes resistanceWB from the wiper position.
// It does not send commands to an MCP42100 (SPI part not included here).
const int  maxPositions = 256;
const long rAB = 92500;
const byte rWiper = 125;

long resistanceWB = 0;   // Estimated resistance W->B (ohms)
int  r2WiperPos   = 0;   // 0..255

// ---------------- Sensors ----------------
#define pinFlexSensor A2
const float VCC   = 5.0;
const float R_DIV = 1000;   // Divider resistor for flex sensor

#define pinGraphiteSensor A0
const float R1 = 100000;
const float R3 = 100000;
const float R5 = 10000;

// ---------------- Menu state ----------------
// Only 3 screens as requested.
enum MenuLevel { MAIN_MENU, MESURE_MENU, ADJUST_R2 };
MenuLevel menuLevel = MAIN_MENU;

int menuIndex = 0;
int lastMenuIndex = -1;
MenuLevel lastMenuLevel = MAIN_MENU;

// Menu entries
const char* mainItems[]   = { "Resistance R2", "Mesure" };
const int mainCount = sizeof(mainItems) / sizeof(mainItems[0]);

const char* mesureItems[] = { "Flex sensor", "Capteur graphite", "< Back" };
const int mesureCount = sizeof(mesureItems) / sizeof(mesureItems[0]);

// ---------------- Button debounce ----------------
bool lastBtn = HIGH;
unsigned long lastBtnMs = 0;
const unsigned long btnDebounceMs = 30;

// ---------------- UI rate limiting (encoder) ----------------
int lastPosUI = 0;
unsigned long lastEncMs = 0;
const unsigned long encStepMs = 10;

// ---------------- Measurement settings ----------------
// Measurements run only on Serial Monitor (OLED does not show measurement screen).
const int N_SAMPLES = 50;
const unsigned long samplePeriodMs = 50;

// ---------------- Prototypes ----------------
// Adding explicit prototypes avoids Arduino auto-prototype quirks.
void drawMenu(bool force);
void drawAdjustR2Screen();
void onSelect();
void handleEncoderUI();
void handleButton();
float Flex_Mesure();
float Graphite_Mesure();
void startMeasurementFlex();
void startMeasurementGraphite();
void updateResistanceFromWiper();
//----------------- Function bluetooth----------
void handleBluetoothCommands() {
  if (BTSerial.available() > 0) {
    char cmd = BTSerial.read();
    Serial.println(cmd);
    if (cmd == 'm') {
      bluetoothMeasureGraphite = true;
    } 
    if (cmd == 'f') {
      bluetoothMeasureFlex = true;
    } 
    else if (cmd == 's') {
      bluetoothMeasureGraphite = false;
      bluetoothMeasureFlex = false;
    }
  }
  //else  Serial.println("is not connected");
}

void sendGraphiteDataBluetooth() {
  if (!bluetoothMeasureGraphite) return;

  if (millis() - lastSampleMs >= 100) {
    lastSampleMs = millis();

    float r = Graphite_Mesure();

    if (!isnan(r)) {
      BTSerial.println(r, 3);
      Serial.println(r,3);

    } else {
      BTSerial.println(0);
    }
  }
}
void sendFlexDataBluetooth() {
  if (!bluetoothMeasureFlex) return;

  if (millis() - lastSampleMs >= 100) {
    lastSampleMs = millis();

    float r = Flex_Mesure();

    if (!isnan(r)) {
      BTSerial.println(r, 3);
      Serial.println(r,3);

    } else {
      BTSerial.println(0);
    }
  }
}
// ---------------- Encoder ISR ----------------
// Very short interrupt routine: updates encoder0Pos.
void doEncoder() {
  if (digitalRead(encoder0PinA) == HIGH) {
    if (digitalRead(encoder0PinB) == HIGH) encoder0Pos++;
    else encoder0Pos--;
  }
}

// Compute the estimated resistance from wiper position.
// Also clamps against 0 to avoid divide-by-zero in graphite math.
void updateResistanceFromWiper() {
  resistanceWB = ((rAB * (long)r2WiperPos) / maxPositions) + rWiper;
  if (resistanceWB < 1) resistanceWB = 1;
}

// Read flex sensor resistance from a voltage divider.
float Flex_Mesure() {
  int adc = analogRead(pinFlexSensor);
  float v = (adc * VCC) / 1023.0f;

  // Avoid divide-by-zero if v is too close to 0 or VCC
  if (v <= 0.001f || v >= (VCC - 0.001f)) return NAN;

  float r = R_DIV * (v / (VCC - v));
  return r;
}

// Read graphite sensor resistance based on your formula.
// Protects against invalid ADC voltage and resistanceWB == 0.
float Graphite_Mesure() {
  int adc = analogRead(pinGraphiteSensor);
  float v_adc = adc * VCC / 1023.0f;

  if (v_adc <= 0.001f) return NAN;
  if (resistanceWB <= 0) return NAN;

  float gain = 1.0f + (R3 / (float)resistanceWB);
  float r = gain * R1 * (VCC / v_adc) - (R1 + R5);
  return r;
}

// Run a short measurement burst and print to Serial only.
void startMeasurementFlex() {
  Serial.println("BEGIN,FLEX");
  float sum = 0;
  for (int i = 0; i < N_SAMPLES; i++) {
    float v = Flex_Mesure();
    Serial.print("FLEX,");
    Serial.print(i);
    Serial.print(",");
    if (isnan(v)) Serial.println("nan");
    else {
      Serial.println(v, 3);
      sum = sum + v;
      delay(samplePeriodMs);
    }
  }
  float avg = sum/N_SAMPLES;
  Serial.println(avg);
  Serial.println("END");
}

// Run a short measurement burst and print to Serial only.
void startMeasurementGraphite() {
  Serial.println("BEGIN,GRAPHITE");
  float sum = 0;
  for (int i = 0; i < N_SAMPLES; i++) {
    float v = Graphite_Mesure();
    Serial.print("GRAPHITE,");
    Serial.print(i);
    Serial.print(",");
    if (isnan(v)) Serial.println("nan");
    else {
      Serial.println(v, 3);
      sum = sum + v;
      delay(samplePeriodMs);
    }
  }
  float avg = sum/N_SAMPLES;
  Serial.println(avg);
  Serial.println("END");
}

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  // Encoder setup
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(Switch, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, RISING);

  // I2C setup for OLED
  Wire.begin();
  Wire.setClock(100000);

  // OLED init (will stop here forever if failed)
  if (!ecranOLED.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (1) {}
  }

  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(1);
  ecranOLED.setTextColor(SSD1306_WHITE);

  // Initialize R2 position and computed resistance
  r2WiperPos = constrain(r2WiperPos, 0, 255);
  encoder0Pos = r2WiperPos;
  updateResistanceFromWiper();

  // Draw initial menu
  drawMenu(true);
}

// Handle encoder rotation:
// - In ADJUST_R2: changes wiper position
// - In menus: changes highlighted item
void handleEncoderUI() {
  if (millis() - lastEncMs < encStepMs) return;
  lastEncMs = millis();

  int pos = encoder0Pos;
  if (pos == lastPosUI) return;

  int delta = (pos > lastPosUI) ? 1 : -1;
  lastPosUI = pos;

  if (menuLevel == ADJUST_R2) {
    r2WiperPos += delta;

    if (r2WiperPos < 0) r2WiperPos = 0;
    if (r2WiperPos > 255) r2WiperPos = 255;

    // Keep encoder position aligned with the wiper value
    encoder0Pos = r2WiperPos;
    updateResistanceFromWiper();
    return;
  }

  int count = (menuLevel == MAIN_MENU) ? mainCount : mesureCount;

  menuIndex += delta;
  if (menuIndex < 0) menuIndex = 0;
  if (menuIndex > count - 1) menuIndex = count - 1;
}

// Debounced button handling:
// calls onSelect() once per press.
void handleButton() {
  bool btn = digitalRead(Switch);

  if (btn != lastBtn) {
    lastBtnMs = millis();
    lastBtn = btn;
  }

  if (millis() - lastBtnMs < btnDebounceMs) return;

  static bool handledPress = false;
  if (btn == LOW && !handledPress) {
    handledPress = true;
    onSelect();
  }
  if (btn == HIGH) handledPress = false;
}

// Menu selection behavior:
// - MAIN_MENU: enter ADJUST_R2 or MESURE_MENU
// - MESURE_MENU: run measurement on Serial OR go back
// - ADJUST_R2: back to MAIN_MENU
void onSelect() {
  if (menuLevel == MAIN_MENU) {
    if (menuIndex == 0) {
      menuLevel = ADJUST_R2;
      r2WiperPos = constrain(r2WiperPos, 0, 255);
      encoder0Pos = r2WiperPos;
      updateResistanceFromWiper();
    } else if (menuIndex == 1) {
      menuLevel = MESURE_MENU;
      menuIndex = 0;
    }
  } else if (menuLevel == MESURE_MENU) {
    if (menuIndex == 0) {
      startMeasurementFlex();
    } else if (menuIndex == 1) {
      startMeasurementGraphite();
    } else if (menuIndex == 2) {
      menuLevel = MAIN_MENU;
      menuIndex = 0;
    }
  } else if (menuLevel == ADJUST_R2) {
    menuLevel = MAIN_MENU;
    menuIndex = 0;
  }

  // Redraw menu after any selection
  drawMenu(true);
}

// OLED screen for R2 adjustment only.
void drawAdjustR2Screen() {
  ecranOLED.clearDisplay();
  ecranOLED.setCursor(0, 0);
  ecranOLED.println("R2");
  ecranOLED.println();
  ecranOLED.print("Wiper: ");
  ecranOLED.println(r2WiperPos);
  ecranOLED.print("R: ");
  ecranOLED.print(resistanceWB);
  ecranOLED.println(" ohms");
  ecranOLED.println();
  ecranOLED.println("Press to back");
  ecranOLED.display();
}

// OLED rendering:
// - ADJUST_R2 shows R2 screen
// - MAIN_MENU and MESURE_MENU show a simple list with ">" cursor
void drawMenu(bool force) {
  if (menuLevel == ADJUST_R2) {
    drawAdjustR2Screen();
    return;
  }

  // Skip redraw if nothing changed
  if (!force && menuLevel == lastMenuLevel && menuIndex == lastMenuIndex) return;

  lastMenuLevel = menuLevel;
  lastMenuIndex = menuIndex;

  ecranOLED.clearDisplay();
  ecranOLED.setCursor(0, 0);

  const char** items;
  int count;

  if (menuLevel == MAIN_MENU) {
    ecranOLED.println("MENU");
    items = mainItems;
    count = mainCount;
  } else {
    ecranOLED.println("MESURE");
    items = mesureItems;
    count = mesureCount;
  }

  for (int i = 0; i < count; i++) {
    ecranOLED.print(i == menuIndex ? "> " : "  ");
    ecranOLED.println(items[i]);
  }

  ecranOLED.display();
}

void loop() {
  handleEncoderUI();
  handleButton();
  drawMenu(false);

  handleBluetoothCommands();
  sendGraphiteDataBluetooth();
  sendFlexDataBluetooth();
  delay(100);
}
