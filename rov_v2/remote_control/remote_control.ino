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

#define normalStringLength 4

#define j1X_calibration_offset 520
#define j1Y_calibration_offset 511 
#define j2X_calibration_offset 507
#define j2Y_calibration_offset 526 

#define debug false


String j1_xValue, j1_yValue, j1_bValue, j2_xValue, j2_yValue, j2_bValue;

const byte numChars = 10;
char receivedChars[numChars];   // an array to store the received data

bool newData    = false;
bool isNegative = false;


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
  delay(500);
 
  sendToLCD("Inicializando...","Joysticks");
  joystic1 = new AxisJoystick(JS1_SW_PIN, JS1_VRX_PIN, JS1_VRY_PIN);
  joystic2 = new AxisJoystick(JS2_SW_PIN, JS2_VRX_PIN, JS2_VRY_PIN);
  delay(500);
  lcd.clear();
}

// the loop function runs over and over again forever
void loop() {

j1_xValue = normalizeValue(String((joystic1->xAxis())-j1X_calibration_offset));
j1_yValue = normalizeValue(String((joystic1->yAxis())-j1Y_calibration_offset)); 
j1_bValue = (digitalRead(JS1_SW_PIN));
j2_xValue = normalizeValue(String((joystic2->xAxis())-j2X_calibration_offset)); 
j2_yValue = normalizeValue(String((joystic2->yAxis())-j2Y_calibration_offset));
j2_bValue = (digitalRead(JS2_SW_PIN));




  printToLCD();
  Serial.println(j1_xValue + "," + j1_yValue + "," + j2_bValue + "," + j2_xValue + "," + j2_yValue + "," + j2_bValue  );

  while (!digitalRead(JS1_SW_PIN) || !digitalRead(JS2_SW_PIN)){
    delay(300);
      while (!digitalRead(JS1_SW_PIN) && !digitalRead(JS2_SW_PIN)){
      }
      while (!digitalRead(JS1_SW_PIN)){
         sendToLCD("TELEMETRIA 1", "ph:12.5 mS:0.5uS");
         delay(1000);
         lcd.clear();
      }
      while (!digitalRead(JS2_SW_PIN)){
         sendToLCD("TELEMETRIA 2", "tmp:17  pre:????");
         delay(1000);
         lcd.clear();
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

String showNewData() {
    if (newData == true) {
        newData = false;
        return String(receivedChars);
    }
}


void printToLCD(){
  lcd.setCursor(0,0);
  lcd.print("FR:" + j1_xValue + "    ");
  lcd.setCursor(8,0);
  lcd.print("LR:" + j1_yValue + "    ");
  lcd.setCursor(4,1);
  lcd.print("SU:" + j2_xValue + "    ");
}

void sendToLCD(String L1, String L2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(L1);
    lcd.setCursor(0,1);
    lcd.print(L2);
}



String normalizeValue(String data){

  int length = data.length();  
  String finalValue, tmp;
  data[0] == '-' ? isNegative = true : isNegative = false;

  if(isNegative){

    if(length == normalStringLength) return data;
      for(int n=0; n < normalStringLength-length; n++){
        tmp += "0";
      }
      finalValue = data[0] + tmp + data.substring(1);
      return finalValue;

  }else{

    if(length == normalStringLength) return data;
      for(int n=0; n < normalStringLength-length-1; n++){
        tmp += "0";
      }
      finalValue = "+" + tmp + data.substring(0);
      return finalValue;
  }
  return "0000";
}










