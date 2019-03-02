#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <time.h>

#include <controlTaMcP.h>

bool reverseMode = false;

const char* ssid = "MiInet";
const char* password = "NubasIvan";

String IP = "";
int pins[6] = {D5, D6, D7, D8, D1, D2};

WiFiServer server(80);

controlTaMcP tankAM = controlTaMcP(pins);

int spdLeft = 255;
int spdRight = 255;
int per = 10;

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(100); 
  
  
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
  Serial.println("Server started");
  
  IP = WiFi.localIP().toString();
  tankAM.moveT(0, 0);
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
  
    tankAM.moveT(spdLeft, spdRight);
    
  }
  else if (req.indexOf("/gpio/down") != -1)
  {
    
    tankAM.moveT(-spdLeft, -spdRight);
    
  }
  else if (req.indexOf("/gpio/left") != -1)
  {
    
    tankAM.moveT(-spdLeft, spdRight);
    
  } 
  else if (req.indexOf("/gpio/right") != -1)
  {
    
    tankAM.moveT(spdLeft, spdRight);
    
  } 
  else if (req.indexOf("/gpio/stop") != -1)
  {
    
    tankAM.moveT(0, 0);
    
  }
  else if (req.indexOf("/gpio/moveUpRight") != -1)
  {
  
    tankAM.moveT(spdLeft, 0);
    
  }
  else if (req.indexOf("/gpio/moveUpLeft") != -1)
  {
    
   tankAM.moveT(0, spdRight);
  
  }
  else if (req.indexOf("/gpio/moveBackRight") != -1)
  {
  
    tankAM.moveT(-spdLeft, 0);
  
  }
  else if (req.indexOf("/gpio/moveBackLeft") != -1)
  {
  
    tankAM.moveT(0, -spdRight);
    
  }
  else if (req.indexOf("/gpio/reverse") != -1)
  {
  
    if (reverseMode)
    {
      reverseMode = false;
    }
    else
    {
      reverseMode = true;
    }
    tankAM.setReverse(reverseMode);
    
  }
  else if (req.indexOf("/gpio/moveSlow") != -1)
  {
     spdLeft *= per;
     spdRight *=per;
     tankAM.moveSlow(1, per);
     tankAM.moveFast(0, per);
    
  }
  else if (req.indexOf("/gpio/moveFast") != -1)
  {
    spdLeft *= per;
    spdRight *=per;
    tankAM.moveFast(1, per);
    tankAM.moveFast(0, per);
    
  }
  else
  {
  
   Serial.println("invalid");
  
  }
  
  client.flush();
  
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
  
  s += "<center><a href='http://"+IP+"/gpio/moveUpLeft'>UP AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/up'>UP</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/moveUpRight'>UP AND RIGHT</a><br>";
  s += "<br><a href='http:/"+IP+"/gpio/left'>LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/stop'>STOP</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/right'>RIGHT</a><br>";
  s += "<br><a href='http://"+IP+"/gpio/moveBackLeft'>DOWN AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/down'>DOWN</a> &emsp;&emsp;<a href='http://"+IP+"/gpio/moveBackRight'>DOWN AND RIGHT</a><br>";
  s += "<br><a href='http://"+IP+"/gpio/moveSlow'>SLOW MOVE</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/reverse'>REVERSE</a> &emsp;&emsp; <a href='http:/"+IP+"/gpio/moveFast'>FAST</a> <br>";
  s += "<br>STATUS: <br><br>";
  s += tankAM.getStatus();
  s += "</center><br></html>\n";
  
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  
  
  

}
