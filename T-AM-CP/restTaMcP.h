//
// Created by Алексей Рыбалкин on 03.03.2019.
//

#ifndef restTaMcP_H
#define restTaMcP_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "defines.h"
#include "controlTaMcP.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


class restTaMcP
{
    private:

        controlTaMcP _tankRest;

        String _IP;

        char* _ssid;
        char* _password;

        String _lastRequest;

        bool _wiFiMode;

        bool _moving;
        int _timing;
        int _timeMove;

        String _htmlReq;


    public:

        WiFiClient _client;
        WiFiServer _server;

        restTaMcP(int pinIn[6]);

        void startWiFiMode(char* ssidIn, char* passwordIn);

        void printHtmlToClient();

        void moveRest(String reqIn);
        void setSettingRest(String reqIn);
        void setSpdPerRest(String reqIn);

        void requestProcessingGET(String reqIn);
        void requestProcessingPOST(String reqIn);

        void printHTMLtoClient();

        bool isGET(String reqIn);
        bool isPOST(String reqIn);

        String parseGET(String reqIn);
        String parsePOST(String reqIn);

        void checkRest();
        void checkSerial();

};

#endif