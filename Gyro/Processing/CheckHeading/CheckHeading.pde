import processing.serial.*;

Serial myPort; // Create object from Serial class


float pitch;
float roll;
float heading;
float cali_Gyro, cali_Acc, cali_Mag;

String input;

int sizeX = 12;
int sizeZ = 8;
int cubeSize = 50;
float gap = 3;
FloatList buffer;
String[] numbers;


void setup()
{
  size(1280, 720, P3D);
  //size(1280, 720);
  smooth(4);
  buffer = new FloatList();


  // List available serial ports.
  println(Serial.list());

  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 38400);
  //delay(2000);
}


void draw() {

  background(40, 40, 60);

  textSize(16);

  text("pitch: " + (pitch), 50, height/12);
  text("roll: " + (roll), 150, height/12);
  text("heading: " + (heading), 250, height/12);

  // calibration status
  text("cali_Gyro: " + (cali_Gyro), 400, height/12);
  text("cali_Acc: " + (cali_Acc), 550, height/12);
  text("cali_Mag: " + (cali_Mag), 700, height/12);


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
}


void serialEvent(Serial myPort) {

  //input = myPort.readString();   
  input = myPort.readStringUntil('\n');

  if (input != null) {
    String[] numbers = splitTokens(input, " ");  
    //println(numbers);
    //println(numbers.length);

    if (numbers.length == 19) {
      if (numbers[0].equals("s") && numbers[numbers.length - 2].equals("e")) {

        heading = float(numbers)[10];
        roll = float(numbers)[11];
        pitch = float(numbers)[12];

        //cali_Gyro = float(numbers)[3];
        //cali_Acc = float(numbers)[4];
        //cali_Mag = float(numbers)[5];


        if (buffer.size() > 900) {
          buffer.remove(0);
        }

        buffer.append(heading);
      }
    }
  }
}
