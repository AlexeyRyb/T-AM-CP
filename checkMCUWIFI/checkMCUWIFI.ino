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

int timeMove = millis();
int timing = 1;
bool moving = false;

String parseMethod(String reqIn, String valIn)
{
  int beginStrIn = reqIn.indexOf(valIn);
  if (beginStrIn == -1)
  {
    return "";
  }
  else
  {
    int questionNext = reqIn.indexOf("&", beginStrIn);
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
  Serial.setTimeout(0); 
  
  
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
    moving = false;
  
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

    if ( (reqGet.indexOf("move") != -1) && (reqGet.indexOf("left") != -1) && (reqGet.indexOf("right") != -1) && (reqGet.indexOf("time") != -1) ) 
    {
      
        int spdLeft = parseMethod(reqGet, "left").toInt();
        int spdRight = parseMethod(reqGet, "right").toInt();
        timing = parseMethod(reqGet, "time").toInt();
        timeMove = millis();
     
        moving = true;
      
        tankAM.moveT(spdLeft, spdRight);
           
    }

    if (reqGet.indexOf("setSetting") != -1)
    {
      
      if (reqGet.indexOf("reverseMode") != -1)
      {
        bool reverseMode = parseMethod(reqGet, "reverseMode").toInt();
        tankAM.setReverse(reverseMode);
      }
      
      if (reqGet.indexOf("stopMoveMode") != -1)
      {
        bool stopMoveMode = parseMethod(reqGet, "stopMoveMode").toInt();
        tankAM.setStopMoveMode(stopMoveMode);
      }

      if (reqGet.indexOf("msecStop") != -1)
      {
        int msecStop = parseMethod(reqGet, "msecStop").toInt();
        tankAM.setMsecStop(msecStop);
      }

      if ( (reqGet.indexOf("SpdLeft") != -1) && (reqGet.indexOf("SpdRight") != -1) )
      {
        int spdLeft = parseMethod(reqGet, "spdLeft").toInt();
        int spdRight = parseMethod(reqGet, "spdRight").toInt();
        tankAM.setSpdT(spdLeft, spdRight);
      }
       
    }

    if (reqGet.indexOf("setSpdPer") != -1)
    {

      int rightPer = parseMethod(reqGet, "right").toInt();
      int leftPer = parseMethod(reqGet, "left").toInt();

      if (rightPer)
      {
        
        if (rightPer > 0)
        {
          tankAM.moveFast(0, rightPer);
        }
        else
        {
          tankAM.moveSlow(0, -rightPer);
        }
      
      }

      if (leftPer)
      {

        if (leftPer > 0)
        {
          tankAM.moveFast(1, leftPer);
        }
        else
        {
          tankAM.moveSlow(1, -leftPer);
        }
      
      }
    
    }

    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
      
    s += "<center><a href='http://"+IP+"/move?left=0&right="+1000 +"&time=1'>UP AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=1000&right=1000&time=1'>UP</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=1000&right=0&time=1'>UP AND RIGHT</a><br>";
    s += "<br><a href='http:/"+IP+"/move?left=-1000&right=1000&time=1'>LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=0&right=0&time=1'>STOP</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=1000&right=-1000&time=1'>RIGHT</a><br>";
    s += "<br><a href='http://"+IP+"/move?left=0&right=-1000&time=1'>DOWN AND LEFT</a> &emsp;&emsp; <a href='http://"+IP+"/move?left=-1000&right=-1000&time=1'>DOWN</a> &emsp;&emsp;<a href='http://"+IP+"/move?left=-1000&right=0&time=1'>DOWN AND RIGHT</a><br>";
    s += "<br><a href='http://"+IP+"/setSpdPer?left=1&right=1&rightPer=-20&leftPer=-20'>SLOW MOVE</a> &emsp;&emsp; <a href='http://"+IP+"/setSetting?reverseMode=1'>REVERSE ON</a> &emsp;&emsp; <a href='http:/"+IP+"/setSpdPer?right=1&left=1&rightPer=20&leftPer=20'>FAST</a> <br>";
    s += " <br> <a href='http://"+IP+"/setSetting?reverseMode=0'>REVERSE OFF</a> <br>";
    s += "<br>STATUS: <br><br>";
    s += tankAM.getStatus();
    s += "</center><br></html>\n";
      
      client.print(s);
    

  }
  if (req.indexOf("POST") != -1)
  {
    int beginStr = req.indexOf("POST") + 6;
    int endStr = req.lastIndexOf("HTTP") - 1;

    String reqPost = req.substring(beginStr, endStr);

    if ( (reqPost.indexOf("move") != -1) && (reqPost.indexOf("left") != -1) && (reqPost.indexOf("right") != -1) && (reqPost.indexOf("time") != -1) ) 
    {
      
        int spdLeft = parseMethod(reqPost, "left").toInt();
        int spdRight = parseMethod(reqPost, "right").toInt();
        timing = parseMethod(reqPost, "time").toInt();
        timeMove = millis();
     
        moving = true;
      
        tankAM.moveT(spdLeft, spdRight);
           
    }

    if (reqPost.indexOf("setSetting") != -1)
    {
      
      if (reqPost.indexOf("reverseMode") != -1)
      {
        bool reverseMode = parseMethod(reqPost, "reverseMode").toInt();
        tankAM.setReverse(reverseMode);
      }
      
      if (reqPost.indexOf("stopMoveMode") != -1)
      {
        bool stopMoveMode = parseMethod(reqPost, "stopMoveMode").toInt();
        tankAM.setStopMoveMode(stopMoveMode);
      }

      if (reqPost.indexOf("msecStop") != -1)
      {
        int msecStop = parseMethod(reqPost, "msecStop").toInt();
        tankAM.setMsecStop(msecStop);
      }

      if ( (reqPost.indexOf("SpdLeft") != -1) && (reqPost.indexOf("SpdRight") != -1) )
      {
        int spdLeft = parseMethod(reqPost, "spdLeft").toInt();
        int spdRight = parseMethod(reqPost, "spdRight").toInt();
        tankAM.setSpdT(spdLeft, spdRight);
      }
       
    }

    if (reqPost.indexOf("setSpdPer") != -1)
    {

      int rightPer = parseMethod(reqPost, "right").toInt();
      int leftPer = parseMethod(reqPost, "left").toInt();

      if (rightPer)
      {
        
        if (rightPer > 0)
        {
          tankAM.moveFast(0, rightPer);
        }
        else
        {
          tankAM.moveSlow(0, -rightPer);
        }
      
      }

      if (leftPer)
      {

        if (leftPer > 0)
        {
          tankAM.moveFast(1, leftPer);
        }
        else
        {
          tankAM.moveSlow(1, -leftPer);
        }
      
      }
    
    }
  }
  
  
  delay(1);
  Serial.println("Client disonnected");
  
  
  

}
