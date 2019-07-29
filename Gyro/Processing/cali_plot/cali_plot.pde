import processing.serial.*;

Serial myPort; // Create object from Serial class

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

float mx;
float my;
float mz;
String input;
String[] numbers;
float[] values;

int sizeX = 12;
int sizeZ = 8;
int cubeSize = 50;
float gap = 3;


void setup()
{
  size(900, 900);
  smooth(4);
  background(0);
  rectMode(CENTER);


  // List available serial ports.
  //println(Serial.list());


  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 38400);
  delay(2000);

  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 12000);



  //values = new float[3];

  //values[0] = 0.0;
  //values[1] = 0.0;
  //values[2] = 0.0;
  
}


void draw() {

  //background(40,40,60);

  //mx = values[0];
  //my = values[1];
  //mz = values[2];

  textSize(16);

  //text("mx: " + mx, 50, height/12);
  //text("my: " + my, 50 + 200, height/12);
  //text("mz:" + mz, 50 + 400, height/12);
  fill(255);
  noStroke();
  text("Mxy: ", 50, height/12);
  text("Mxz: ", 50 + 200, height/12);
  text("Myz:", 50 + 400, height/12);


  noStroke();

  stroke(255);
  line(0, height/2, width, height/2);
  line(width/2, 0, width/2, height);

  pushMatrix();
  translate(width/2, height/2);

  noStroke();
  fill(255, 0, 0, 40);
  ellipse(mx/2, my/2, 10, 10);

  fill(0, 255, 0, 40);
  rect(mx/2, mz/2, 10, 10);

  fill(0, 0, 255, 40);
  ellipse(my/2, mz/2, 10, 10);

  popMatrix();
}



void serialEvent(Serial myPort) {

  //println("SerialEvent()");
  //input = myPort.readString();   

  input = myPort.readStringUntil('\n');
  //println(input);

  //
  //println(numbers);
  if (input != null) {
    println(input);

    String[] numbers = splitTokens(input, " ");

    if (numbers.length == 3) {
      mx = float(numbers[0]);
      my = float(numbers[1]);
      mz = float(numbers[2]);
      //println("mx: " + mx);
      //println("my: " + my);
      //println("mz: " + mz);
    }
  }
}

void keyPressed() {
  if (key == 32) {
    background(0);
  }
}



void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */

  if (theOscMessage.checkAddrPattern("/mag")==true) {
    /* check if the typetag is the right one. */
    if (theOscMessage.checkTypetag("fff")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      mx = theOscMessage.get(0).floatValue();  
      my = theOscMessage.get(1).floatValue();
      mz = theOscMessage.get(2).floatValue();
      //print("### received an osc message /test with typetag ifs.");
      //println(" values: "+firstValue+", "+secondValue+", "+thirdValue);
      return;
    }
  } 
  println("### received an osc message. with address pattern "+theOscMessage.addrPattern());
}
