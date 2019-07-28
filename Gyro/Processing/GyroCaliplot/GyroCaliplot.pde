import processing.serial.*;

Serial myPort; // Create object from Serial class

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

float yaw;
float pitch;
float roll;
String input;
String[] numbers;
float[] values;

int sizeX = 12;
int sizeZ = 8;
int cubeSize = 50;
float gap = 3;


void setup()
{
  size(1280, 720, P3D);
  smooth(4);

  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 12001);

  // List available serial ports.
  println(Serial.list());

  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 38400);
  //myPort.bufferUntil('\n');
  //delay(2000); 


  values = new float[14];

  values[0] = 0.0;
  values[1] = 0.0;
  values[2] = 0.0;
  values[3] = 0.0;
  values[4] = 0.0;
  values[5] = 0.0;
  values[6] = 0.0;
  values[7] = 0.0;
  values[8] = 0.0;
  values[9] = 0.0;
  values[10] = 0.0;
  values[11] = 0.0;


  myRemoteLocation = new NetAddress("127.0.0.1", 12000);
}

void sendOSC(float x, float y, float z) {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/mag");

  myMessage.add(x); /* add an int to the osc message */
  myMessage.add(y); /* add a float to the osc message */
  myMessage.add(z); /* add a string to the osc message */

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation);
}


void draw() {

  background(40, 40, 60);

  yaw = values[9];
  pitch = values[10];
  roll = values[11];

  textSize(16);

  text("yaw: " + (yaw), 50, height/12);
  text("pitch: " + pitch, 50 + 200, height/12);
  text("roll:" + roll, 50 + 400, height/12);

  translate(width/2, height/2 + (cubeSize * 3));

  rotateX(radians(-roll));
  rotateZ(radians(pitch));
  rotateY(radians((yaw)));

  // rotateY(radians(-pitch));

  fill(255, 255, 255, 160);

  strokeWeight(2);

  /*
  box(400,50,200);
   */

  for (int  i = 0; i < sizeZ; i++ ) {

    for (int j = 0; j < sizeX; j ++ ) {
 

      float xPos = (j - sizeX/2 ) * cubeSize + cubeSize/2 + ( j - (sizeX/2 - 1) ) * gap ;
      float zPos = (i - sizeZ/2) * cubeSize + cubeSize/2 + + ( i - (sizeZ/2 - 1) ) * gap;

      pushMatrix();
      translate(xPos, 0, zPos);
      stroke(200, 200, 255);
      box(cubeSize, cubeSize/2, cubeSize);

      popMatrix();
    }
  }
}


void serialEvent(Serial myPort) {

  //input = myPort.readString();   
  input = myPort.readStringUntil('\n');
  // 's', mx, my, mz, ax,ay, az, gx, gy, gz, yaw, pitch, roll, 'e'
  
  //println(numbers);
  if (input != null) {

    //println(input);
    numbers = splitTokens(input, " ");
    //println(numbers);
    //println(numbers.length);
    //println(numbers[0]);
    //println(numbers[numbers.length-2]);

    if (numbers.length == 15) {
      values[0] = float(numbers[1]);
      values[1] = float(numbers[2]);
      values[2] = float(numbers[3]);
      values[3] = float(numbers[4]);
      values[4] = float(numbers[5]);
      values[5] = float(numbers[6]);
      values[6] = float(numbers[7]);
      values[7] = float(numbers[8]);
      values[8] = float(numbers[9]);
      values[9] = float(numbers[10]);
      values[10] = float(numbers[11]);
      values[11] = float(numbers[12]);

      //println("yaw: " + values[0]);
      //println("pitch: " + values[1]);
      //println("roll: " + values[2]);

      sendOSC(values[0], values[1], values[2]); // mx, my, mz
    } else {
      println(input);
    }
  }
}
