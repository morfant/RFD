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
  size(1600, 1600);
  smooth(4);
  background(0);
  rectMode(CENTER);

  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,12000);

  // List available serial ports.
  //println(Serial.list());

  //String portName = Serial.list()[1];
  //myPort = new Serial(this, portName, 38400);
  //myPort.bufferUntil('\n');
  //delay(2000); 


  values = new float[3];

  values[0] = 0.0;
  values[1] = 0.0;
  values[2] = 0.0;
}


void draw() {

  //background(40,40,60);

  mx = values[0];
  my = values[1];
  mz = values[2];

  textSize(16);

  //text("mx: " + mx, 50, height/12);
  //text("my: " + my, 50 + 200, height/12);
  //text("mz:" + mz, 50 + 400, height/12);
  fill(255);
  noStroke();
  text("Mxy: ", 50, height/12);
  text("Mxz: ", 50 + 200, height/12);
  text("Myz:" , 50 + 400, height/12);
  
  
  noStroke();

  stroke(255);
  line(0, height/2, width, height/2);
  line(width/2, 0, width/2, height);
  
  pushMatrix();
  translate(width/2, height/2);

  noStroke();
  fill(255, 0, 0, 40);
  ellipse(mx, my, 10, 10);

  fill(0, 255, 0, 40);
  rect(mx, mz, 10, 10);

  fill(0, 0, 255, 40);
  ellipse(my, mz, 10, 10);

  popMatrix();
}



void serialEvent(Serial myPort) {

  //input = myPort.readString();   
  input = myPort.readStringUntil('\n');
  //
  //println(numbers);
  if (input != null) {

    //println(input);
    numbers = splitTokens(input, " ");

    if (numbers.length == 3) {
      values[0] = float(numbers[0]);
      values[1] = float(numbers[1]);
      values[2] = float(numbers[2]);
    }

    if ( values.length == 3) {
      println("mx: " + values[0]);
      println("my: " + values[1]);
      println("mz: " + values[2]);
    }
  }
  //if (numbers != null) {
  //float[] values = float(numbers);
  //if(number[0] != null) {

  //delay(2000);
  //println(numbers);
  //println(values[0]);

  //}

  //if(numbers != null) {
  //yaw = values[0];
  //pitch = float(numbers[1]);
  //float roll = values[2];
  // }

  //}
}



void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */
  
  if(theOscMessage.checkAddrPattern("/mag")==true) {
    /* check if the typetag is the right one. */
    if(theOscMessage.checkTypetag("fff")) {
      /* parse theOscMessage and extract the values from the osc message arguments. */
      values[0] = theOscMessage.get(0).floatValue();  
      values[1] = theOscMessage.get(1).floatValue();
      values[2] = theOscMessage.get(2).floatValue();
      //print("### received an osc message /test with typetag ifs.");
      //println(" values: "+firstValue+", "+secondValue+", "+thirdValue);
      return;
    }  
  } 
  println("### received an osc message. with address pattern "+theOscMessage.addrPattern());
}
