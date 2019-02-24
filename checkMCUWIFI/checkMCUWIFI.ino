#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <time.h>
#include <TaMcP.h>


const char* ssid = "MiInet";
const char* password = "NubasIvan";

String IP = "";
int pins[6] = {D5, D6, D7, D8, D1, D2};
WiFiServer server(80);

TaMcP tank = TaMcP(pins);

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
  Serial.println("Server started");\
  
  IP = WiFi.localIP().toString();
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
  
    tank.moveUp();
    
  }
  else if (req.indexOf("/gpio/down") != -1)
  {
    
    tank.moveBack();
    
  }
  else if (req.indexOf("/gpio/left") != -1)
  {
    
    tank.left();
    
  } 
  else if (req.indexOf("/gpio/right") != -1)
  {
    
    tank.right();
    
  } 
  else if (req.indexOf("/gpio/stop") != -1)
  {
    
    tank.stopMove();
    
  }
  else if (req.indexOf("/gpio/moveUpRight") != -1)
  {
  
    tank.moveUpRight();
    
  }
  else if (req.indexOf("/gpio/moveUpLeft") != -1)
  {
    
   tank.moveUpLeft();
  
  }
  else if (req.indexOf("/gpio/moveBackRight") != -1)
  {
  
    tank.moveBackRight();
  
  }
  else if (req.indexOf("/gpio/moveBackLeft") != -1)
  {
  
    tank.moveBackLeft();
    
  }
  else if (req.indexOf("/gpio/reverse") != -1)
  {
  
    Serial.println("reverse");
    
    tank.setReverse();
    
  }
  else if (req.indexOf("/gpio/moveSlow") != -1)
  {
  
     tank.moveSlow();
    
  }
  else if (req.indexOf("/gpio/moveFast") != -1)
  {
  
    tank.moveFast();
    
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
  s += tank.getStatus();
  s += "</center><br></html>\n";
  
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  
  
  

}
