#include "restTaMcP.h"

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

restTaMcP::restTaMcP(int pinIn[6])
:
    _IP(" "),
    _tankRest(pinIn),
    _ssid(" "),
    _password(" "),
    _lastRequest(" "),
    _moving(false),
    _timing(1),
    _timeMove(millis()),
    _server(80),
    _htmlReq("")

{
    _tankRest.moveT(0, 0);
}

void restTaMcP::startWiFiMode(char* ssidIn, char* passwordIn)
{

    _ssid = ssidIn;
    _password = passwordIn;

    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED)
    {

        delay(500);

    }

    _server.begin();

    _IP = WiFi.localIP().toString();
    delay(1000);
    Serial.println(_IP);

}

void restTaMcP::moveRest(String reqIn)
{

    if ( (reqIn.indexOf("move") != -1) && (reqIn.indexOf("left") != -1) && (reqIn.indexOf("right") != -1) && (reqIn.indexOf("time") != -1) )
    {

        int spdLeft = parseMethod(reqIn, "left").toInt();
        int spdRight = parseMethod(reqIn, "right").toInt();
        _timing = parseMethod(reqIn, "time").toInt();
        _timeMove = millis();

        _moving = true;

        _tankRest.moveT(spdLeft, spdRight);

    }

}

void restTaMcP::setSettingRest(String reqIn)
{

    if (reqIn.indexOf("setSetting") != -1)
    {

        if (reqIn.indexOf("reverseMode") != -1)
        {
            bool reverseMode = parseMethod(reqIn, "reverseMode").toInt();
            _tankRest.setReverse(reverseMode);
        }

        if (reqIn.indexOf("stopMoveMode") != -1)
        {
            bool stopMoveMode = parseMethod(reqIn, "stopMoveMode").toInt();
            _tankRest.setStopMoveMode(stopMoveMode);
        }

        if (reqIn.indexOf("msecStop") != -1)
        {
            int msecStop = parseMethod(reqIn, "msecStop").toInt();
            _tankRest.setMsecStop(msecStop);
        }

        if ( (reqIn.indexOf("SpdLeft") != -1) && (reqIn.indexOf("SpdRight") != -1) )
        {
            int spdLeft = parseMethod(reqIn, "spdLeft").toInt();
            int spdRight = parseMethod(reqIn, "spdRight").toInt();
            _tankRest.setSpdT(spdLeft, spdRight);
        }

    }

}

void restTaMcP::setSpdPerRest(String reqIn)
{

    if ( (reqIn.indexOf("setSpdPer") != -1) && (reqIn.indexOf("right") != -1) && (reqIn.indexOf("left") != -1) )
    {

        int rightPer = parseMethod(reqIn, "right").toInt();
        int leftPer = parseMethod(reqIn, "left").toInt();

        if (rightPer)
        {

            if (rightPer > 0)
            {
                _tankRest.moveFast(0, rightPer);
            }
            else
            {
                _tankRest.moveSlow(0, -rightPer);
            }

        }

        if (leftPer)
        {

            if (leftPer > 0)
            {
                _tankRest.moveFast(1, leftPer);
            }
            else
            {
                _tankRest.moveSlow(1, -leftPer);
            }

        }

    }

}

void restTaMcP::printHtmlToClient()
{
        _htmlReq = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n ";
        _htmlReq += "<center><a href='http://"+_IP+"/move?left=0&right="+1000 +"&time=1'>UP AND LEFT</a> &emsp;&emsp; <a href='http://"+_IP+"/move?left=1000&right=1000&time=1'>UP</a> &emsp;&emsp; <a href='http://"+_IP+"/move?left=1000&right=0&time=1'>UP AND RIGHT</a><br>";
        _htmlReq += "<br><a href='http:/"+_IP+"/move?left=-1000&right=1000&time=1'>LEFT</a> &emsp;&emsp; <a href='http://"+_IP+"/move?left=0&right=0&time=1'>STOP</a> &emsp;&emsp; <a href='http://"+_IP+"/move?left=1000&right=-1000&time=1'>RIGHT</a><br>";
        _htmlReq += "<br><a href='http://"+_IP+"/move?left=0&right=-1000&time=1'>DOWN AND LEFT</a> &emsp;&emsp; <a href='http://"+_IP+"/move?left=-1000&right=-1000&time=1'>DOWN</a> &emsp;&emsp;<a href='http://"+_IP+"/move?left=-1000&right=0&time=1'>DOWN AND RIGHT</a><br>";
        _htmlReq += "<br><a href='http://"+_IP+"/setSpdPer?left=1&right=1&rightPer=-20&leftPer=-20'>SLOW MOVE</a> &emsp;&emsp; <a href='http://"+_IP+"/setSetting?reverseMode=1'>REVERSE ON</a> &emsp;&emsp; <a href='http:/"+_IP+"/setSpdPer?right=1&left=1&rightPer=20&leftPer=20'>FAST</a> <br>";
        _htmlReq += " <br> <a href='http://"+_IP+"/setSetting?reverseMode=0'>REVERSE OFF</a> <br>";
        _htmlReq += "<br>STATUS: <br><br>";
        _htmlReq += _tankRest.getStatus();
        _htmlReq += "</center><br></html>\n";
}


void restTaMcP::requestProcessingGET(String reqIn)
{

    moveRest(reqIn);
    setSettingRest(reqIn);
    setSpdPerRest(reqIn);
    printHtmlToClient();

}

void restTaMcP::requestProcessingPOST(String reqIn)
{

    moveRest(reqIn);
    setSettingRest(reqIn);
    setSpdPerRest(reqIn);

}

bool restTaMcP::isGET(String reqIn)
{
    return reqIn.indexOf("GET") != -1;
}

String restTaMcP::parseGET(String reqIn)
{

    int beginStr = reqIn.indexOf("GET") + 5;
    int endStr = reqIn.lastIndexOf("HTTP") - 1;

    return reqIn.substring(beginStr, endStr);
}

bool restTaMcP::isPOST(String reqIn)
{
    return reqIn.indexOf("POST") != -1;
}

String restTaMcP::parsePOST(String reqIn)
{

    int beginStr = reqIn.indexOf("POST") + 6;
    int endStr = reqIn.lastIndexOf("HTTP") - 1;

    return reqIn.substring(beginStr, endStr);
}

void restTaMcP::checkRest()
{

    if ( ( (millis() - _timeMove) / 1000.0 > _timing) && _moving )
    {

        _tankRest.moveT(0, 0);
        _moving = false;

    }

    WiFiClient _client = _server.available();

    if (!_client)
    {
        return;
    }

    while (!_client.available())
    {
        delay(1);
    }

    _lastRequest = _client.readStringUntil('\r');
    _client.flush();


    if (isGET(_lastRequest))
    {
        requestProcessingGET( parseGET(_lastRequest) );
        _client.print(_htmlReq);
        _htmlReq = "";
        _client.flush();

    }

    if (isPOST(_lastRequest))
    {
        requestProcessingPOST( parsePOST(_lastRequest) );
    }

    delay(1);

}

void restTaMcP::checkSerial()
{

    if (Serial.available() > 0)
    {

        int ctrl = Serial.parseInt();

        if (ctrl == 1)
        {
            int leftSpd = Serial.parseInt();
            int rightSpd = Serial.parseInt();

            _tankRest.moveT(leftSpd, rightSpd);
        }

        if (ctrl == 2)
        {
            int leftSpd = Serial.parseInt();
            int rightSpd = Serial.parseInt();

            _tankRest.setSpdT(leftSpd, rightSpd);
        }

        if (ctrl == 3)
        {

            bool sideMove = Serial.parseInt();
            int delt = Serial.parseInt();

            if (delt > 0)
            {
                _tankRest.moveFast(sideMove, delt);
            }
            else
            {
                _tankRest.moveSlow(sideMove, -delt);
            }

        }

        if (ctrl == 4)
        {
            bool stopMode = Serial.parseInt();

            _tankRest.setStopMoveMode(stopMode);

        }

        if (ctrl == 5)
        {
            bool reverseMode = Serial.parseInt();

            _tankRest.setReverse(reverseMode);
        }

        if (ctrl == 6)
        {
            int msec = Serial.parseInt();

            _tankRest.setMsecStop(msec);
        }

        if (ctrl == 7)
        {
            Serial.println(_IP);
        }

        if (ctrl == 8)
        {
            Serial.println(_ssid);
            Serial.println(_password);
        }

        if (ctrl == 9)
        {
            Serial.println(_lastRequest);
        }

        if (ctrl == 10)
        {
            Serial.println(_moving);
        }
    }

}





