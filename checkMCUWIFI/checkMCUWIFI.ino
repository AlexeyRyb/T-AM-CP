#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <time.h>

const char* ssid = "MiInternet";
const char* password = "NubasIvan";

const int backLeft = D5; 
const int upLeft = D6;
const int backRight = D7; 
const int upRight = D8;
const int speedRight = D1;
const int speedLeft = D2;

bool reverseMove = false;
int spd = 255;

String IPstr = "";
const int numLog = 10;
String statusStr[numLog];

void stopMove()
{
  digitalWrite(upRight, false);
  digitalWrite(upLeft, false);
  digitalWrite(backLeft, false);
  digitalWrite(backRight, false);
  delay(100);
}

String timePrint()
{
  String timeStr = "";
  int timeNow = millis() / 1000;
  
  if (timeNow / 60 / 60 < 10) 
  { 
    timeStr +="0"; 
  }
  
  timeStr += String(timeNow / 60 / 60);
  
  timeStr += ":";
  if ( (timeNow / 60) % 60 < 10) 
  { 
    timeStr += "0"; 
  }
  
  timeStr += String( (timeNow/60)%60);
  timeStr += ":";
  if (timeNow % 60 < 10) 
  { 
    timeStr += "0"; 
  }
  
  timeStr += String(timeNow % 60);

  return timeStr;
}

WiFiServer server(80);

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(100); 
  
  pinMode(upRight, OUTPUT); 
  pinMode(upLeft, OUTPUT); 
  pinMode(backLeft, OUTPUT); 
  pinMode(backRight, OUTPUT); 
  pinMode(speedRight, OUTPUT); 
  pinMode(speedLeft, OUTPUT); 
  
  analogWrite(speedRight, 255);
  analogWrite(speedLeft, 255);
  
  
  Serial.println("Ready!"); 
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    
    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("WiFi connected");
  
  server.begin();
  Serial.println("Server started");\
  
  Serial.println(WiFi.localIP());
  for (int i = 0; i < numLog; i++)
  {
    if (i == 0)
    {
      statusStr[i] = timePrint() + "  Start";
    }
    else
    {
      statusStr[i] = "";
    }
  }
} 

void statusChange(String newStatus)
{
  
  for(int i = numLog - 1; i > 0; i--)
  {
    statusStr[i] = statusStr[i-1];
  }
  
  statusStr[0] = timePrint() + "  " + newStatus;

}

void moveUp()
{
  stopMove();
  statusChange("move up");
  
  if (reverseMove)
  {
    digitalWrite(backRight, true);
    digitalWrite(backLeft, true);
  }
  else
  {
    digitalWrite(upRight, true);
    digitalWrite(upLeft, true); 
  }
}

void moveBack()
{
  stopMove();
  statusChange("move back");
  if (reverseMove)
  {
    digitalWrite(upRight, true);
    digitalWrite(upLeft, true); 
  }
  else
  {
    digitalWrite(backRight, true);
    digitalWrite(backLeft, true);
  }
}

void right()
{
  
  stopMove();
  statusChange("right");
  
  digitalWrite(backRight, true);
  digitalWrite(upLeft, true);
}

void left()
{
  
  stopMove();
  statusChange("left");
  
  digitalWrite(upRight, true);
  digitalWrite(backLeft, true);
}

void moveUpRight()
{

  stopMove();
  statusChange("move up and right");
  
  if (reverseMove)
  {
    digitalWrite(backRight, true); 
  }
  else
  {
    digitalWrite(upLeft, true);
  }
}

void moveUpLeft()
{

  stopMove();
  statusChange("move up and left");
  
  if (reverseMove)
  {
    digitalWrite(backLeft, true); 
  }
  else
  {
    digitalWrite(upRight, true);
  }
}
  
void moveBackRight()
{
  
  stopMove();
  statusChange("move back and right");
  
  if (reverseMove)
  {
    digitalWrite(upRight, true); 
  }
  else
  {
    digitalWrite(backLeft, true);
  }
}

void moveBackLeft()
{

  stopMove();
  statusChange("move back and left");
  
  if (reverseMove)
  {
    digitalWrite(upLeft, true); 
  }
  else
  {
    digitalWrite(backRight, true);
  }
}

void moveSlow()
{
  if (spd < 25)
  {
    spd = 0;
  }
  else
  {
    spd = spd - 25;
  }

  if (reverseMove)
  {
    analogWrite(speedRight, spd);
  }
  else
  {
    analogWrite(speedLeft, spd);
  }
  
}

void moveFast()
{
  
  if (spd > 230)
  {
    spd = 255;
  }
  else
  {
    spd = spd + 25;
  }
  
    analogWrite(speedLeft, spd);
    analogWrite(speedRight, spd);
  
}

void loop() 
{ 

  WiFiClient client = server.available();
  
  if (!client) 
  {
    return;
  }
  
  Serial.println("new client");
  while(!client.available())
  {
     delay(1);
  }
  
  String req = client.readStringUntil('\r');
  
  Serial.println(req);
  client.flush();
  
  if (req.indexOf("/gpio/up") != -1)
  {
  
    Serial.println("go up");
    moveUp();
    
  }
  else if (req.indexOf("/gpio/down") != -1)
  {
    
    Serial.println("go back");
    moveBack();
    
  }
  else if (req.indexOf("/gpio/left") != -1)
  {
    
    Serial.println("left");
    left();
    
  } 
  else if (req.indexOf("/gpio/right") != -1)
  {
    
    Serial.println("right");
    right();
    
  } 
  else if (req.indexOf("/gpio/stop") != -1)
  {
    
    Serial.println("stop");
    stopMove();
    
  }
  else if (req.indexOf("/gpio/moveUpRight") != -1)
  {
  
    Serial.println("go up and right");
    moveUpRight();
    
  }
  else if (req.indexOf("/gpio/moveUpLeft") != -1)
  {
    
    Serial.println("go up and left");
    moveUpLeft();
  
  }
  else if (req.indexOf("/gpio/moveBackRight") != -1)
  {
  
    Serial.println("go back and right");
    moveBackRight();
  
  }
  else if (req.indexOf("/gpio/moveBackLeft") != -1)
  {
  
    Serial.println("go back and left");
    moveBackLeft();
    
  }
  else if (req.indexOf("/gpio/reverse") != -1)
  {
  
    Serial.println("reverse");
    
    if (reverseMove)
    {
      reverseMove = false;
    }
    else
    {
      reverseMove = true;
    }
    
  }
  else if (req.indexOf("/gpio/moveSlow") != -1)
  {
  
    Serial.print("move slow");
    Serial.println(spd);
    
    moveSlow();
    
  }
  else if (req.indexOf("/gpio/moveFast") != -1)
  {
  
    Serial.println("move fast");
    Serial.println(spd);
    
    moveFast();
    
  }
  else
  {
  
   Serial.println("invalid");
  
  }
  
  client.flush();
  
  String IP = "192.168.43.171";
  
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
  
  s += "<center><a href='http://"+IP+"/gpio/moveUpLeft'>UP AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/up'>UP</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/moveUpRight'>UP AND RIGHT</a><br>";
  s += "<br><a href='http:/"+IP+"/gpio/left'>LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/stop'>STOP</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/right'>RIGHT</a><br>";
  s += "<br><a href='http://"+IP+"/gpio/moveBackLeft'>DOWN AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/down'>DOWN</a> &emsp;&emsp;<a href='http://"+IP+"/gpio/moveBackRight'>DOWN AND RIGHT</a><br>";
  s += "<br><a href='http://"+IP+"/gpio/moveSlow'>SLOW MOVE</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/reverse'>REVERSE</a> &emsp;&emsp; <a href='http:/"+IP+"/gpio/moveSlowLeft'>FAST</a> <br>";
  s += "<br>STATUS: <br><br>";
  for (int i = 0; i < numLog; i++)
  {
    s += statusStr[i] + "<br>";
  }
  s += "</center><br></html>\n";
  
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  
  
  

}
