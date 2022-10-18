#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Joystick.h>
#include <AxisJoystick.h>
//#include <VirtualWire.h>

/*JS 01*/
#define JS1_SW_PIN 5
#define JS1_VRX_PIN A0
#define JS1_VRY_PIN A1
/*JS 02*/
#define JS2_SW_PIN 6
#define JS2_VRX_PIN A2
#define JS2_VRY_PIN A3

#define col  16 //Define o número de colunas do display utilizado
#define lin   2 //Define o número de linhas do display utilizado
#define ende  0x27 //Define o endereço do display

String j1_xValue, j1_yValue, j2_xValue, j2_yValue;

const byte numChars = 10;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;


Joystick* joystic1;
Joystick* joystic2;

LiquidCrystal_I2C lcd(ende,col,lin);  // set the LCD address to 0x27 for a 16 chars and 2 line display



// the setup function runs once when you press reset or power the board
void setup() {
 
  lcd.init();
  delay(100);
  lcd.clear();
  lcd.backlight();

  sendToLCD("   ENTA CPROB   "," ROV CTRL  V1.1  ");
  delay(2000);
 
  sendToLCD("Inicializando...","Serial COMM");
  Serial.begin(9600);
  delay(2000);
 
  sendToLCD("Inicializando...","Joysticks");
  joystic1 = new AxisJoystick(JS1_SW_PIN, JS1_VRX_PIN, JS1_VRY_PIN);
  joystic2 = new AxisJoystick(JS2_SW_PIN, JS2_VRX_PIN, JS2_VRY_PIN);
  
  delay(2000);
}

// the loop function runs over and over again forever
void loop() {

  j1_xValue = (joystic1->xAxis())-520; 
  j1_yValue = (joystic1->yAxis())-511;
  j2_xValue = (joystic2->xAxis())-507; 
  j2_yValue = (joystic2->yAxis())-526;

  printToLCD();
  sentViaRX();

  while (!digitalRead(JS1_SW_PIN) || !digitalRead(JS2_SW_PIN)){
    delay(300);
      while (!digitalRead(JS1_SW_PIN) && !digitalRead(JS2_SW_PIN)){

      }
      while (!digitalRead(JS1_SW_PIN)){
        // sendToLCD("TELEMETRIA", "");
         receiveTelemetrics();
         showNewData();
      }
      while (!digitalRead(JS2_SW_PIN)){
          sendToLCD("", "BT2 PRESSED");
      }
  }
}

void receiveTelemetrics(){
   static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        lcd.clear();
        lcd.setCursor(4,1);
        lcd.print(receivedChars);
        newData = false;
    }
    delay(500);
}


void printToLCD(){
  lcd.setCursor(0,0);
  lcd.print("FR:" + j1_xValue + "    ");
  lcd.setCursor(8,0);
  lcd.print("LR:" + j1_yValue + "    ");
  lcd.setCursor(0,1);
  lcd.print("SU:" + j2_xValue + "    ");
  lcd.setCursor(8,1);
  lcd.print("OO:" + j2_yValue + "    ");
}

void sentViaRX(){
  Serial.print("j1x" + j1_xValue + ":");
  Serial.print("j1y" + j1_yValue + ":");
  Serial.print("j2x" + j2_xValue + ":");
  Serial.print("j2y" + j2_yValue + ":");
  Serial.print("j1b" + String(digitalRead(JS1_SW_PIN)) + ":");
  Serial.print("j2b" + String(digitalRead(JS2_SW_PIN)) + ":");
}

void sendToLCD(String L1, String L2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(L1);
    lcd.setCursor(0,1);
    lcd.print(L2);
}


/**
  Return title of the input joystick mov
  e.
*/
String moveTitle(const Joystick::Move move) {
  switch (move) {
    case Joystick::Move::NOT:
      return "NOT";
    case Joystick::Move::PRESS:
      return "PRESS";
    case Joystick::Move::UP:
      return "UP";
    case Joystick::Move::DOWN:
      return "DOWN";
    case Joystick::Move::RIGHT:
      return "RIGHT";
    case Joystick::Move::LEFT:
      return "LEFT";
    default:
      return "???";
  }
}








