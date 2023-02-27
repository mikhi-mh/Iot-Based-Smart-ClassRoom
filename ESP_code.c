#include <Servo.h>

Servo myservo;  // create servo object to control a servo

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "smart-classroom-c390b.firebaseio.com"
#define FIREBASE_AUTH "qaR8ZkbFtc4iqpa6ZELK59kdA5rYwAI3GwVIcvrf"
#define WIFI_SSID "mikhilesh"
#define WIFI_PASSWORD "1234567890"

#define LAMP   12
#define FAN    13
#define PROJECTOR 14
#define CURTAIN 15

#include <Adafruit_Fingerprint.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  return finger.fingerID; 
}


uint8_t id;
int val;
int present = 0;

int curt = 1;
void setup() {

  Serial.begin(9600);
  finger.begin(57600);
Serial1.begin(9600);
  
pinMode(LAMP,OUTPUT);
pinMode(FAN,OUTPUT);
pinMode(PROJECTOR,OUTPUT);

myservo.attach(CURTAIN);  // attaches the servo on pin 9 to the servo object
myservo.write(5);

digitalWrite(LAMP,HIGH);
digitalWrite(FAN,HIGH);
digitalWrite(PROJECTOR,HIGH);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

    Firebase.setFloat("p1",0);    
    Firebase.setFloat("p2",0);    
    Firebase.setFloat("p3",0);    


}

int n = 0;
int  i = 0;
float Temp = 0.0;
int fire = 0, smoke = 0;
int done = 0;
int msg = 0;
String data   = "";

void loop() 
{
  while (Serial.available()) // check if the esp is sending a message
  {
    if (Serial.find("DATA"))
    {
      delay(50);
      for (i = 0; i < 7; i++)
        {
        char c=Serial.read();
        data += c;
        } 
     
     Temp = ((data[0]-48)*1000)+((data[1]-48)*100)+((data[2]-48)*10)+(data[3]-48);
     Temp = Temp/100.0;
   
     smoke = (data[4]-48);

     fire = (data[5]-48);
     msg = (data[6]-48);

     data = "";
     done = 1;
    }
  }   


if (done == 1)
{  
  done = 0;
  Firebase.setFloat("Smoke",smoke);
  Firebase.setFloat("Fire",fire);
  Firebase.setFloat("msg",msg);
  Firebase.setFloat("Temp",Temp);

  
  val = getFingerprintIDez();

  if(val == 1)
  {
    Firebase.setFloat("p1",1);    
  }

  if(val == 2)
  {
    Firebase.setFloat("p2",1);    
  }

  if(val == 3)
  {
    Firebase.setFloat("p3",1);    
  }
/*
  if(val!= -1)
  present++;

  if(present > 3)
  present = 3;
  */
  Serial.print("Number of Present Students :  ");
  Serial.println(present);
  Firebase.setFloat("STUDENT",present);  

  if(Firebase.getString("LAMP")=="1")
  {
       digitalWrite(LAMP,LOW);
  }
  
  if(Firebase.getString("LAMP")=="0")
  {
       digitalWrite(LAMP,HIGH);
  }


  if(Firebase.getString("FAN")=="1")
  {
       digitalWrite(FAN,LOW);
  }
  
  if(Firebase.getString("FAN")=="0")
  {
       digitalWrite(FAN,HIGH);
  }


  if(Firebase.getString("PROJECTOR")=="1")
  {
       digitalWrite(PROJECTOR,LOW);
       myservo.write(5);
  }

  
  if(Firebase.getString("PROJECTOR")=="0")
  {
       digitalWrite(PROJECTOR,HIGH);
  }

  if(Firebase.getString("inst1")=="1")
  {
       Serial1.print('H');
       Firebase.setString("inst1","0");    
  }

  if(Firebase.getString("inst2")=="1")
  {
       Serial1.print('O');
       Firebase.setString("inst2","0");    
  }

  if(Firebase.getString("PROJECTOR")=="0")
  {
       digitalWrite(PROJECTOR,HIGH);
  }


  if(Firebase.getString("CURTAIN")=="1" && digitalRead(PROJECTOR)==1)
  {
      myservo.write(175);
  }
  
  if(Firebase.getString("CURTAIN")=="0" && digitalRead(PROJECTOR)==1)
  {
      myservo.write(10);
  }

  done = 0;
}
}