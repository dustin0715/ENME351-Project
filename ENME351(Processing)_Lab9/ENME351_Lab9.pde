// Most of the code came from Lab3
import processing.serial.*; // add the Serial library
Serial myPort;  // the serial port to monitor

// Declare image variables
PImage img1; 
PImage img2;

// Declare variables
int spacing = 0;

void setup() {
  size(600, 600);      // set the window size
  background(255,255,255);      // white background
  printArray(Serial.list());       // list all available serial ports
  myPort = new Serial(this, Serial.list()[2], 9600);       //define input port
  myPort.clear();       // clear the port of any initial junk
  
  // Load in the images into the image variables
  img1 = loadImage("Sleeping_Terp.PNG");
  img2 = loadImage("Angry_Terp.PNG");
}

void draw() {
  if (myPort.available() > 0) {    // make sure port is open
    String inString = myPort.readStringUntil('\n');    // read input string
    if (inString != null) {      // ignore null strings
      inString = trim(inString);      // trim off any whitespace
      
      // If an object is dectected, display angry terp image
      if(inString.equals("object_detected")){
        imageMode(CENTER);
        image(img2, 300, 300, img2.width+20, img2.height+20);
      }
      
      // If no object is detected, display sleeping terp image
      if(inString.equals("no_object_detected")){
        fill(255,255,255); 
        noStroke();
        rectMode(CENTER);
        rect(300, 300, img2.width+20, img2.height+20);
      }
      
      // display enter passcode
      if(inString.equals("enter_passcode")){
        textSize(45);
        textAlign(CENTER);
        fill(0);
        text("Enter Passcode:", 300, 270);
      }
      
      // if button is pressed, display circles
      if(inString.equals("button_pressed")) {
        if(spacing > 210){
          spacing = 0;
        }
        fill(0, 0, 0);
        ellipse(195 + spacing, 330, 50, 50);
        spacing = spacing + 70;
      }
       
      // If the door is unlocked, display sleeping terp image
      if(inString.equals("UnLocked")){
        imageMode(CENTER);
        image(img1, 300, 300, img2.width+20, img2.height+20);
      }
    }
  }
}
