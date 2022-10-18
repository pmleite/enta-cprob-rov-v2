// Example 2 - Receive with an end-marker

const byte numChars = 10;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

void setup() {
    Serial.begin(9600);
    while(!Serial);
    Serial.println("<Arduino is ready>");
    Serial1.begin(9600);
    while(!Serial1);
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvWithEndMarker();
    showNewData();

    Serial1.println("ola...!");
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = ':';
    char rc;
    
    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();

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
        Serial.println(receivedChars);
        newData = false;
    }
}