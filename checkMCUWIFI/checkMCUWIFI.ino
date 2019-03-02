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
int timeMove = millis();
int timing = 1;
bool moving = false;

String valuePost(String reqIn, String valIn)
{
  int beginStrIn = reqIn.indexOf(valIn);
  if (beginStrIn == -1)
  {
    return "";
  }
  else
  {
    int questionNext = reqIn.indexOf("?", beginStrIn);
    int spaceNext = reqIn.indexOf(" ", beginStrIn);
    if (questionNext == -1)
    {
       return reqIn.substring( beginStrIn + valIn.length() + 1, spaceNext);
    }
    else
    {
      return reqIn.substring( beginStrIn + valIn.length() + 1, questionNext);
    }
    
  }
}

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
  if ( ( (millis() - timeMove) / 1000.0 > timing) && moving )
  {
    tankAM.moveT(0, 0);
  }
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

  if (req.indexOf("GET") != -1)
  {
    int beginStr = req.indexOf("GET") + 5;
    int endStr = req.lastIndexOf("HTTP") - 1;

    String reqGet = req.substring(beginStr, endStr);

    if (reqGet.indexOf("move") != -1)
    {
      int spdLeft = valuePost(reqGet, "left").toInt();
      int spdRight = valuePost(reqGet, "right").toInt();
      int timing = valuePost(reqGet, "time").toInt();
      int timeMove = millis();
      moving = true;
      
      
      tankAM.moveT(spdLeft, spdRight);
      
    }

  }
  if (req.indexOf("POST") != -1)
  {
    int beginStr = req.indexOf("POST") + 6;
    int endStr = req.lastIndexOf("HTTP") - 1;

    String reqPost = req.substring(beginStr, endStr);
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
  
  s += "<center><a href='http://"+IP+"/move?left=0&right=1000&time=5'>UP AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=200&right=1000&time=5'>UP</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=1000&right=0&time=5'>UP AND RIGHT</a><br>";
  s += "<br><a href='http:/"+IP+"/move?left=-1000&right=1000&time=5'>LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=0&right=0&time=5'>STOP</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=1000&right=-1000&time=5'>RIGHT</a><br>";
  s += "<br><a href='http://"+IP+"/gpio/move?left=0&right=-1000&time=5'>DOWN AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=-1000&right=1000&time=5'>DOWN</a> &emsp;&emsp;<a href='http://"+IP+"/gpio/move?left=-1000&right=0&time=5'>DOWN AND RIGHT</a><br>";
  s += "<br><a href='http://"+IP+"/gpio/moveSlow'>SLOW MOVE</a> &emsp;&emsp; <a href='http://"+IP+"/gpio/reverse'>REVERSE</a> &emsp;&emsp; <a href='http:/"+IP+"/gpio/moveFast'>FAST</a> <br>";
  s += "<br>STATUS: <br><br>";
  s += tankAM.getStatus();
  s += "</center><br></html>\n";
  
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  
  
  

}
