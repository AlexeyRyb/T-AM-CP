#include <time.h>

#include <restTaMcP.h>

bool reverseMode = false;

char* ssid = "MiInet";
char* password = "NubasIvan";

int pins[6] = {D5, D6, D7, D8, D1, D2};


restTaMcP tankAM = restTaMcP(pins);

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(10); 

  tankAM.startWiFiMode(ssid, password);
  
} 

void loop() 
{ 
  
  tankAM.checkRest();

  tankAM.checkSerial(); 
  
}
