#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "MiInternet";
const char* password = "NubasIvan";

const int backLeft = D1; 
const int upLeft = D2;
const int backRight = D3; 
const int upRight = D4;
const int speedRight = D5;
const int speedLeft = D6;

bool reverseMove = false;

void stopMove()
{
  digitalWrite(upRight, false);
  digitalWrite(upLeft, false);
  digitalWrite(backLeft, false);
  digitalWrite(backRight, false);
  delay(100);
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

} 

void moveUp()
{
  stopMove();
  
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
  digitalWrite(backRight, true);
  digitalWrite(upLeft, true);
}

void left()
{
  stopMove();
  digitalWrite(upRight, true);
  digitalWrite(backLeft, true);
}

void moveUpRight()
{

  stopMove();
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
  if (reverseMove)
  {
    digitalWrite(upLeft, true); 
  }
  else
  {
    digitalWrite(backRight, true);
  }
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
  s += "<br><a href='http://"+IP+"/gpio/reverse'>REVERSE</a>";
  s += "</center><br></html>\n";
  
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  
  
  

}
