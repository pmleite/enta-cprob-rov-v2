#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Joystick.h>
#include <AxisJoystick.h>


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

int j1_xValue, j1_yValue, j2_xValue, j2_yValue;

Joystick* joystic1;
Joystick* joystic2;

LiquidCrystal_I2C lcd(ende,col,lin);  // set the LCD address to 0x27 for a 16 chars and 2 line display


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  delay(500);
  lcd.init(); //Inicializa a comunicação com o display já conectado
  lcd.clear(); //Limpa a tela do display
  lcd.backlight(); //Aciona a luz de fundo do display

 
  delay(500);
  joystic1 = new AxisJoystick(JS1_SW_PIN, JS1_VRX_PIN, JS1_VRY_PIN);
  joystic2 = new AxisJoystick(JS2_SW_PIN, JS2_VRX_PIN, JS2_VRY_PIN);
  
  lcd.setCursor(0,0);
  lcd.print("ENTA-CPROB   ROV");
  delay(3000);
  lcd.clear();
}

// the loop function runs over and over again forever
void loop() {
  j1_xValue = (joystic1->xAxis())-520; 
  j1_yValue = (joystic1->yAxis())-511;
  j2_xValue = (joystic2->xAxis())-507; 
  j2_yValue = (joystic2->yAxis())-526;
  lcd.setCursor(0,0);
  lcd.print("FR:" + String(j1_xValue) + "    ");
  lcd.setCursor(8,0);
  lcd.print("LR:" + String(j1_yValue) + "    ");
  lcd.setCursor(0,1);
  lcd.print("SU:" + String(j2_xValue) + "    ");
  lcd.setCursor(8,1);
  lcd.print("OO:" + String(j2_yValue) + "    ");

  Serial.println(j1_xValue);
  delay(100);
  Serial.println(j1_yValue);
  delay(100);
  Serial.println(j2_xValue);
  delay(100);
  Serial.println(j2_yValue);
  delay(100);
}



/**
  Return title of the input joystick move.
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








