import processing.serial.*;

Serial myPort; // Create object from Serial class

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

final int scale = 1;
float mx;
float my;
float mz;
float pitch;
float roll;
float heading;
String input;
String[] numbers;
float[] values;
FloatList buffer;


int sizeX = 12;
int sizeZ = 8;
int cubeSize = 50;
float gap = 3;

boolean isCal = true;


void setup()
{
  size(1000, 1000, P3D);
  smooth(4);
  background(0);
  rectMode(CENTER);
  buffer = new FloatList();


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

  if (!isCal) {
    background(40, 40, 60);

    text("pitch: " + pitch, 50, height/12);
    text("roll: " + roll, 50 + 200, height/12);
    text("heading: " + heading, 50 + 400, height/12);


    pushMatrix();
    translate(width/2, height/2 + (cubeSize * 3));

    rotateX(radians((pitch)));
    rotateZ(radians((roll)));
    rotateY(radians((heading)));

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
    popMatrix();

    fill(200, 100, 100, 255);
    noStroke();
    pushMatrix();
    translate(100, 100);
    for (int i = 0; i < buffer.size(); i++) {
      //println(buffer.get(i));
      ellipse(i, abs(buffer.get(i)), 3, 3);
    }
    popMatrix();
  } else {


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
    ellipse(mx * scale, my * scale, 10, 10);

    fill(0, 255, 0, 40);
    rect(mx * scale, mz * scale, 10, 10);

    fill(0, 0, 255, 40);
    ellipse(my * scale, mz * scale, 10, 10);

    popMatrix();
  }
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
      println("mx: " + mx);
      println("my: " + my);
      println("mz: " + mz);
    } else if (numbers.length == 6) {
      mx = float(numbers[0]);
      my = float(numbers[1]);
      mz = float(numbers[2]);
      pitch = float(numbers[3]);
      roll = float(numbers[4]);
      heading = float(numbers[5]);
      println("mx: " + mx);
      println("my: " + my);
      println("mz: " + mz);
      println("pitch: " + pitch);
      println("roll: " + roll);
      println("heading: " + heading);


      if (buffer.size() > 900) {
        buffer.remove(0);
      }

      buffer.append(heading);
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
