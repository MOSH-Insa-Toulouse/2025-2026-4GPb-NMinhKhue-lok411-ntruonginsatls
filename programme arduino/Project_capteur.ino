#include <Adafruit_SSD1306.h>

//-----encoder--------
#define encoder0PinA  2  //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB  4  //DT Output B
#define Switch 5 // Switch connection if available

volatile int encoder0Pos = 0;
//---------------------

//----OLED---------
#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);
//------------------

// MCP42100
const byte csPin = 10;
const int maxPositions = 256;
const long rAB = 92500;
const byte rWiper = 125;
const byte pot0 = 0x11;

//MENU init 
int menuIndex = 0;
bool adjustingResistance = false;
bool buttonPressed = false;
long resistanceWB = ((rAB * encoder0Pos) / maxPositions) + rWiper;
String menuItems[] = { "Resistance R2", "Mesure" };



void setup() { 


  //setup encodeur rotation
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor

  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  attachInterrupt(0, doEncoder, RISING); // encoder pin on interrupt 0 - pin2
  //-----------------------------------------------------

  showMenu();


  Serial.begin (9600);
  Serial.println("start");                // a personal quirk
  
} 

//encodeur fonction 
void doEncoder() {
  if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
    encoder0Pos++;
  } else if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
    encoder0Pos--;
  }
  // Serial.println (encoder0Pos, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  delay(200);
}



//setup OLED
void showMenu() {
  ecranOLED.clearDisplay();

  for (int i = 0; i < 2; i++) {
    ecranOLED.setTextColor(i == menuIndex ? SSD1306_BLACK : SSD1306_WHITE,
                         i == menuIndex ? SSD1306_WHITE : SSD1306_BLACK);
    ecranOLED.setCursor(10, 10 + (i * 20));
    ecranOLED.println(menuItems[i]);
  }

  if (adjustingResistance) {
    ecranOLED.clearDisplay();
    ecranOLED.setCursor(10, 50);
    ecranOLED.print("R: ");
    resistanceWB = ((rAB * encoder0Pos) / maxPositions) + rWiper;
    ecranOLED.print(resistanceWB);
    ecranOLED.println(" ohms");
  }

  ecranOLED.display();
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.println (encoder0Pos, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  delay(100);
}
