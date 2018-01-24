
#include <dht.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Process.h>
#include <Servo.h>

#define dht_apin A0 // Analog Pin A0 DHT sensor is connected
dht DHT; // sensor for temperature and humidity 
#define FLAME_DETECT_ANA A1 //input pin for the flame detectors analogue output.
int sound = A2; //sensor for the microphone at Analog Pin A2

//for photo
int countp=0;
String photoshell;
String photoshell1;
String photoshell2;

int ir_sensor1= A3; //for sharp distance sensoer
float reading; //variables that will store the value of the ir sensor
int cm;// variables that we will make the change to cm 

//the speed that each servo will have
int speedservo1=0;
int speedservo2=0;
int speedservo3=0;
int speedservo4=0;


//ESC of quadcopter each number is displayed on quadcopter
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

//for streaming video variables
Process stream;
Process photo;
Process path;
Process closestream;

//variable for the YUN Linux server that will send and receive the commands
YunServer server;


// funcrtion that set the Speed at each servo
void setSpeed(int speed, int numberofservo) {
  // speed is from 0 to 100 where 0 is off and 100 is maximum speed
  //the following maps speed values of 0-40 to angles from 0-180,
  // some speed controllers may need different values, see the ESC instructions
  int angle = map(speed, 0, 50, 0, 180);

  if(numberofservo == 1 ){
     myservo1.write(angle);
    }
  if(numberofservo == 2 ){
     myservo2.write(angle);
    }
  if(numberofservo == 3 ){
     myservo3.write(angle);
    }
  if(numberofservo == 4 ){
     myservo4.write(angle);
    }
 
}


void setup()
{
  Serial.begin(9600); //the band that the YUN could write on Serial Monitor
  pinMode (sound, INPUT);// initialize the microphone sensor as input
  Bridge.begin();
  //open live stream
  stream.runShellCommandAsynchronously("mjpg_streamer -i 'input_uvc.so -d /dev/video0 -r 640x480 -f 25' -o 'output_http.so -p 8080 -w /www/webcam' &");

  //attach at each servo the pins that they are connected
  myservo1.attach(3);
  myservo2.attach(6);
  myservo3.attach(9);
  myservo4.attach(10);

  //open a listener at localhost
  server.listenOnLocalhost();
  server.begin();
 

}

void loop()
{
 
    
   YunClient client = server.accept();// accept the request for the client(browser)
  
  if (client) {//if the request is exist(is true)
    String command = client.readString();//read the URL which is also the command
    command.trim();        //kill whitespace
    Serial.println(command); //print it on the Serial Monitor
    //if the command is arming the quadcopter
    if (command == "arm") {
        // arm the speed controller, modify as necessary for your ESC
        setSpeed(30,1);
        setSpeed(30,2);
        setSpeed(30,3);
        setSpeed(30,4);
        delay(2000); //delay 2 second,  some speed controllers may need longer
        setSpeed(0,1);
        setSpeed(0,2);
        setSpeed(0,3);
        setSpeed(0,4);
        delay(2000); //delay 2 second,  some speed controllers may need longer

    }
  //if the command is forward 
  if (command == "forward") {
      
        if(speedservo3<40){
        speedservo3=speedservo3+5;
        }
        if(speedservo4<40){
        speedservo4=speedservo4+5;
        }
       /* if(speedservo1>5){
        speedservo1=speedservo1-5;
        }
        if(speedservo2>5){
        speedservo2=speedservo2-5;
        }*/
        
       // setSpeed(speedservo1,1);
       // setSpeed(speedservo2,2);
        setSpeed(speedservo3,3);
        setSpeed(speedservo4,4);
       
       /*delay(200); //
        speedservo3=speedservo3-10;  
        speedservo4=speedservo4-10;
       setSpeed(speedservo3,3);
        setSpeed(speedservo4,4);*/
       

    }
  //if the command is reverse 
  if (command == "reverse") {
      
        if(speedservo1<40 ){
        speedservo1=speedservo1+5;
        }
        if(speedservo2<40 ){
        speedservo2=speedservo2+5;
        }
        /*if(speedservo3>5){
        speedservo3=speedservo3-5;
        }
        if(speedservo4>5){
        speedservo4=speedservo4-5;
        }*/
        
        setSpeed(speedservo1,1);
        setSpeed(speedservo2,2);
        //setSpeed(speedservo3,3);
        //setSpeed(speedservo4,4);
        
      /* delay(200); //
        speedservo1=speedservo1-10;  
        speedservo2=speedservo2-10;
       setSpeed(speedservo1,1);
        setSpeed(speedservo2,2);*/

    }
      //if the command is up 
      if (command == "up") {

        if(speedservo1<40){
        speedservo1=speedservo1+5;
        }
         if(speedservo2<40){
        speedservo2=speedservo2+5;
         }
         if(speedservo3<40){
        speedservo3=speedservo3+5;
         }
         if(speedservo4<40){
        speedservo4=speedservo4+5;
        }
        setSpeed(speedservo1,1);
        setSpeed(speedservo2,2);
        setSpeed(speedservo3,3);
        setSpeed(speedservo4,4);
        

    }
      //if the command is down 
      if (command == "down") {

        if(speedservo1>5){
        speedservo1=speedservo1-5;
        }
         if(speedservo2>5){
        speedservo2=speedservo2-5;
         }
         if(speedservo3>5){
        speedservo3=speedservo3-5;
         }
        if(speedservo4>5){
        speedservo4=speedservo4-5;
         }
        setSpeed(speedservo1,1);
        setSpeed(speedservo2,2);
        setSpeed(speedservo3,3);
        setSpeed(speedservo4,4);
       

    }
   //if the command is left 
  if (command == "left") {
      
        if(speedservo2<40 ){
        speedservo2=speedservo2+5;
        }
        if(speedservo3<40 ){
        speedservo3=speedservo3+5;
        }
        /*if(speedservo1>5){
        speedservo1=speedservo1-5;
        }
        if(speedservo4>5){
        speedservo4=speedservo4-5;
        }*/
        
       // setSpeed(speedservo1,1);
        setSpeed(speedservo2,2);
        setSpeed(speedservo3,3);
      //  setSpeed(speedservo4,4);
          
      /* delay(200); //
        speedservo3=speedservo3-10;  
        speedservo2=speedservo2-10;
        setSpeed(speedservo2,2);
        setSpeed(speedservo3,3);*/

    }
       //if the command is right 
      if (command == "right") {
      
        if(speedservo1<40 ){
        speedservo1=speedservo1+5;
        }
        if(speedservo4<40 ){
        speedservo4=speedservo4+5;
        }
        /*if(speedservo3>5){
        speedservo3=speedservo3-5;
        }
        if(speedservo2>5){
        speedservo2=speedservo2-5;
        }*/
        
        setSpeed(speedservo1,1);
        //setSpeed(speedservo2,2);
      //  setSpeed(speedservo3,3);
        setSpeed(speedservo4,4);
       
       /*delay(200); //
         speedservo1=speedservo1-10;  
        speedservo4=speedservo4-10;
         setSpeed(speedservo1,1);
        setSpeed(speedservo4,4);*/

    }
     //if the command is to read the temperature
    if (command == "temp") {

      DHT.read11(dht_apin);
      
      //client.print("temperature = ");

      client.print(DHT.temperature);

      client.println("C  ");

      String key = "TEMPRATURE";
      Bridge.put(key, String(DHT.temperature));

    }

  //if the command is to read the humidity
  if (command == "humidity") {

      DHT.read11(dht_apin);
      client.print(DHT.humidity);


      String key = "HUMIDITY";
      Bridge.put(key, String(DHT.humidity));

    }

    //if the command is to read the flame sensor
    if (command == "flame") {

      //client.print("Sensor Value: "); // Read the sensors analogue output and send it to the serial port
      int flameValue = analogRead(FLAME_DETECT_ANA);
      //client.print(flameValue);
      if (flameValue <= 490) // Read the status of the sensors digital output and if it is high then send an alert to the UART and the led is lighten
      {
        client.println("FLAME!");

      }
      else{
        client.println("NO FLAME!");
        }
      String key = "FLAME";
      Bridge.put(key, String(flameValue));

    }
    //if the command is to read the microphone value
    if (command == "microphone") {

      int beat = analogRead(sound);
      

      if ( beat > 630)
      { client.println("Noise detect");

      }
      else {
        client.println("It is quite");

      }
      String key = "SOUND";
      Bridge.put(key, String(beat));

    }

    //if the command is to read the distance value
    if (command == "distance") {

      reading=analogRead(ir_sensor1);
      cm= pow(3027.4 / reading, 1.2134);
      client.print(cm);
      client.print("cm");
      
      String key = "Distance1";
      Bridge.put(key, String(cm));

    }

     /*if the command is to take a photo
   if(command == "photo"){
       //close stream to take photo
       closestream.runShellCommandAsynchronously("kill -9 `pidof mjpg_streamer`");
       
       while(closestream.running());
       //take photo
       photoshell1="fswebcam /mnt/sda1/photo";
       photoshell2=".png";
       photoshell=photoshell1+countp+photoshell2;
       photo.runShellCommand(photoshell);
       //waits till the picture is saved
       while(photo.running());
       //restart stream
       stream.runShellCommandAsynchronously("mjpg_streamer -i 'input_uvc.so -d /dev/video0 -r 640x480 -f 25' -o 'output_http.so -p 8080 -w /www/webcam' &");
       
       countp++;
    }*/ 

    client.stop(); //close the client connection 
  }

}
