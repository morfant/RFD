import processing.serial.*;

Serial myPort; // Create object from Serial class


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
  size(1280, 720,P3D);
  smooth(4);

  
  // List available serial ports.
println(Serial.list());

  String portName = Serial.list()[1];
   myPort = new Serial(this, portName, 38400);
  //myPort.bufferUntil('\n');
 //delay(2000); 
 
 
 values = new float[3];
 
 values[0] = 0.0;
 values[1] = 0.0;
 values[2] = 0.0;
 
}


void draw() {
  
 background(40,40,60);

 yaw = values[0];
 pitch = values[1];
 roll = values[2];
   
  textSize(16);
 
  text("yaw: " + (yaw) , 50, height/12);
  text("pitch: " + pitch, 50 + 200, height/12);
  text("roll:" + roll, 50 + 400, height/12);
 
  translate(width/2, height/2 + (cubeSize * 3));
  
  rotateX(radians(-roll));
  rotateZ(radians(pitch));
 rotateY(radians((yaw)));
  
 // rotateY(radians(-pitch));
  
  fill(255,255,255,160);
  
  strokeWeight(2);
  
  /*
  box(400,50,200);
  */
  
  for(int  i = 0; i < sizeZ; i++ ) {
    
    for(int j = 0; j < sizeX; j ++ ) {
       
      
        float xPos = (j - sizeX/2 ) * cubeSize + cubeSize/2 + ( j - (sizeX/2 - 1) ) * gap ;
        float zPos = (i - sizeZ/2) * cubeSize + cubeSize/2 + + ( i - (sizeZ/2 - 1) ) * gap;
       
        pushMatrix();
       translate(xPos,0, zPos);
       stroke(200,200,255);
      box(cubeSize,cubeSize/2,cubeSize);
  
   popMatrix();
    }
    
  }
}


void serialEvent(Serial myPort) {
  
  //input = myPort.readString();   
  input = myPort.readStringUntil('\n');
  //
  //println(numbers);
  if(input != null) {
    
    //println(input);
   numbers = splitTokens(input, " ");
   
   if(numbers.length == 6) {
   values[0] = float(numbers[0]);
   values[1] = float(numbers[1]);
   values[2] = float(numbers[2]);
   }
   
   if( values.length == 3) {
   println("yaw: " + values[0]);
   println("pitch: " + values[1]);
   println("roll: " + values[2]);
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
