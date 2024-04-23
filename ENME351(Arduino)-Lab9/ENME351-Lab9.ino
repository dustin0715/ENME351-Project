// Declare pins as variables
int trig = 5;
int echo = 6;
int servo = 3;
int b1 = 10;
int b2 = 9;
int b3 = 4;
int b4_reset = 8;
int led = 11;
int trig2 = 13;
int echo2 = 12;

// Declare an array for password and a counter for indexing while loop
int pass[4];
int counter = 0;

// Variables for LED fade
int brightness = 0; 
int fadeAmount = 5;

void setup() {
  // Declare the mode of all the pins and set the serial at 9600 bauds rate
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(servo, OUTPUT);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4_reset, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Serial.begin(9600); 
}

void loop() {
  // Read the buttons
  int a = digitalRead(b1);
  int b = digitalRead(b2);
  int c = digitalRead(b3);
  int d = digitalRead(b4_reset);
    
  // When all the password buttons are pressed at same time, 
  // the system is ready to recieve the code combination
  // Door closed 
  // Wait for the LEDs to blink 3 times before entering the code
  if(a==0 && b==0 && c==0){
    
    Serial.println("enter_passcode");
    
    long timeStart = micros(); // timming circuit 
    digitalWrite(servo, HIGH);
    while(micros() < timeStart + 1500-800){
    }
    digitalWrite(servo, LOW);
    while(micros() < timeStart + 20000){
    }

    for(int i=0; i<3; i++){
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
    }

    // This loops let you put the code in the pass array
    while(counter < 4){
      
      // Read the state of the buttons
      a = digitalRead(b1);
      b = digitalRead(b2);
      c = digitalRead(b3);
      
      // if the buttons is pressed, store the values and increase count
      // wait a short time for the button to release
      if(a==0){
        pass[counter] = 1;
        Serial.println(pass[counter]);
        Serial.println("button_pressed");
        counter++;
        delay(300);
      }
      if(b==0){
        pass[counter] = 2;
        Serial.println(pass[counter]);
        Serial.println("button_pressed");
        counter++;
        delay(300);
      }
      if(c==0){
        pass[counter] = 3; 
        Serial.println(pass[counter]);
        Serial.println("button_pressed");
        counter++;
        delay(300);
      }
      
    }
    
  }
  
  // If the pass array matches the code (1,3,2,3), 
  // the door will be open and the LEDs will be in fading mode
  if(pass[0] == 1 && pass[1] == 3 && pass[2] == 2 && pass[3] == 3){
    
    // Fade code from Arduino built in examples
    analogWrite(led, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(30);

    // Door opened 
    long timeStart1 = micros(); // timming circuit 
    digitalWrite(servo, HIGH);
    while(micros() < timeStart1 + 1500){
    }
    digitalWrite(servo, LOW);
    while(micros() < timeStart1 + 20000){
    }
    
    Serial.println("UnLocked");
  }

  // if password is incorrect counter reset to 0
  // Ultrasonic sensor activate
  else{
    counter = 0;

    long timeStart2 = micros(); // timing circuit
    
    // Got this code from: https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
    digitalWrite(trig, LOW);      // Clears the trigPin
    while(micros() < timeStart2 + 2){        
    }
    digitalWrite(trig, HIGH);     // Sets the trigPin on HIGH state
    while(micros() < timeStart2 + 10){        
    }
    digitalWrite(trig, LOW);
  
    // Got this code from: https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
    int duration = pulseIn(echo, HIGH);   // Reads the echoPin, returns the sound wave travel time 
    float distance = duration*0.034/2;    // Calculate the distance
    Serial.print("Distance:");
    Serial.print("\t");
    Serial.println(distance);

    // If the sensor sense an object, servo activate and vice versa
    if(distance < 10){
      long timeStart3 = micros(); // timming circuit 
      digitalWrite(servo, HIGH);
      while(micros() < timeStart3 + 1500-800){
      }
      digitalWrite(servo, LOW);
      while(micros() < timeStart3 + 20000){
      }
      digitalWrite(led, HIGH);

      Serial.println("object_detected");
    }
    else{
      long timeStart4 = micros(); // timming circuit 
      digitalWrite(servo, HIGH);
      while(micros() < timeStart4 + 1500){
      }
      digitalWrite(servo, LOW);
      while(micros() < timeStart4 + 20000){
      }
      digitalWrite(led, LOW);
      
      Serial.println("no_object_detected");
    }
  }

  // if the reset button is pressed, 
  // the pass array and counter will be set to 0
  if(d == 0){
    digitalWrite(led, LOW);
    Serial.println("Locked");
    for(int i=0; i<4; i++){
      pass[i]=0;
    }
    counter = 0;
  }

}
