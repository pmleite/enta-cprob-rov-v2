#include <Wire.h>

#define m1_pwm_pin 9
#define m2_pwm_pin 10
#define m3_pwm_pin 11
#define m4_pwm_pin 12

#define m1a_pin    22
#define m1b_pin    23
#define m2a_pin    24
#define m2b_pin    25
#define m3a_pin    26
#define m3b_pin    27
#define m4a_pin    28
#define m4b_pin    29    

#define debug false
#define printData true

#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection

int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

String j1_xValue, j1_yValue, j1_bValue, j2_xValue, j2_yValue, j2_bValue;
bool is_j1_xValue_neg, is_j1_yValue_neg, is_j1_bValue_neg, is_j2_xValue_neg, is_j2_yValue_neg, is_j2_bValue_neg;

const byte numChars = 25;
char receivedChars[numChars];   // an array to store the received data

bool newData = false;


void setup() {

    //Init Serials
    Serial.begin(9600);
    while(!Serial);
    Serial1.begin(9600);
    while(!Serial1);
    Wire.begin();
    Serial.println("Comm OK!");
    
    pinMode(LED,OUTPUT);

    //Init pwm Pins
    pinMode(m1_pwm_pin, OUTPUT);
    pinMode(m2_pwm_pin, OUTPUT);
    pinMode(m3_pwm_pin, OUTPUT);
    pinMode(m4_pwm_pin, OUTPUT);

    //Init Motors Pins
    pinMode(m1a_pin, OUTPUT);
    pinMode(m1b_pin, OUTPUT);
    pinMode(m2a_pin, OUTPUT);
    pinMode(m2b_pin, OUTPUT);
    pinMode(m3a_pin, OUTPUT);
    pinMode(m3b_pin, OUTPUT);
    pinMode(m4a_pin, OUTPUT);
    pinMode(m4b_pin, OUTPUT);

    //Stop Motors
    analogWrite(m1_pwm_pin, 0);
    analogWrite(m2_pwm_pin, 0);
    analogWrite(m3_pwm_pin, 0);
    analogWrite(m4_pwm_pin, 0);

    //Motor to same direction
    digitalWrite(m1a_pin, LOW);
    digitalWrite(m1b_pin, HIGH);
    digitalWrite(m2a_pin, LOW);
    digitalWrite(m2b_pin, HIGH);
    digitalWrite(m3a_pin, LOW);
    digitalWrite(m3b_pin, HIGH);
    digitalWrite(m4a_pin, LOW);
    digitalWrite(m4b_pin, HIGH);
}


void loop() {
    recvWithEndMarker();
    moveROV(receivedChars);
}




void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        } 
    }
}

void moveROV(String data){

 if (newData == true) {

     receivedChars[0]  == '-' ? is_j1_xValue_neg = true : is_j1_xValue_neg = false;
     receivedChars[5]  == '-' ? is_j1_yValue_neg = true : is_j1_yValue_neg = false;
     receivedChars[12] == '-' ? is_j2_xValue_neg = true : is_j2_xValue_neg = false;
     receivedChars[17] == '-' ? is_j2_yValue_neg = true : is_j2_yValue_neg = false;

     j1_xValue = String(String(receivedChars[1]) + String(receivedChars[2]) + String(receivedChars[3]));
     j1_yValue = String(String(receivedChars[6]) + String(receivedChars[7]) + String(receivedChars[8]));
     j1_bValue = String(receivedChars[10]);
     j2_xValue = String(String(receivedChars[13]) + String(receivedChars[14]) + String(receivedChars[15]));
     j2_yValue = String(String(receivedChars[18]) + String(receivedChars[19]) + String(receivedChars[20]));
     j2_bValue = String(receivedChars[22]);

     int j1x_mapped = map(j1_xValue.toInt(), 0, 600, 0, 255);
     int j1y_mapped = map(j1_yValue.toInt(), 0, 600, 0, 255);
     int j2x_mapped = map(j2_xValue.toInt(), 0, 600, 0, 255);

    //Up, Down
     if (is_j2_xValue_neg){
       digitalWrite(m3a_pin, HIGH);
       digitalWrite(m3b_pin, LOW);
       digitalWrite(m4a_pin, HIGH);
       digitalWrite(m4b_pin, LOW);
       if(debug) Serial.println("DOWN");
     }else{
       digitalWrite(m3a_pin, LOW);
       digitalWrite(m3b_pin, HIGH);
       digitalWrite(m4a_pin, LOW);
       digitalWrite(m4b_pin, HIGH);
      if(debug) Serial.println("UP");
     }
     analogWrite(m3_pwm_pin, j2x_mapped);
     analogWrite(m4_pwm_pin, j2x_mapped);


     //Forward, Backward
     if (is_j1_xValue_neg){
       digitalWrite(m1a_pin, HIGH);
       digitalWrite(m1b_pin, LOW);
       digitalWrite(m2a_pin, HIGH);
       digitalWrite(m2b_pin, LOW);
       if(debug) Serial.println("REVERSE");
     }else{
       digitalWrite(m1a_pin, LOW);
       digitalWrite(m1b_pin, HIGH);
       digitalWrite(m2a_pin, LOW);
       digitalWrite(m2b_pin, HIGH);
       if(debug) Serial.println("FORWARD");
     }

      if (j1_yValue.toInt() <= 15){
        analogWrite(m1_pwm_pin, j1x_mapped);
        analogWrite(m2_pwm_pin, j1x_mapped);
        if(debug) Serial.println("TOLERANCE");
      }else{
        if(is_j1_yValue_neg){
          analogWrite(m1_pwm_pin, j1y_mapped);
          analogWrite(m2_pwm_pin, j1x_mapped);
          if(debug) Serial.println("ESQUERDA");
        }else{
          analogWrite(m1_pwm_pin, j1x_mapped);
          analogWrite(m2_pwm_pin, j1y_mapped);
          if(debug) Serial.println("DIREITA");
        }
      } 
  }
  newData = false;
}

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}












