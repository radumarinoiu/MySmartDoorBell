/*
    This sketch sends a string to a TCP server, and prints a one-line response.
    You must run a TCP server in your local network.
    For example, on Linux you can use this command: nc -v -l 3000
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "Radu"
#define STAPSK "sGw4Xvbw"
#endif

#define D1 5
#define D2 4
#define D5 14
#define D6 12
#define D7 13
#define A0 

const char *ssid = STASSID;
const char *password = STAPSK;

const char *host = "192.168.2.10";
const uint16_t port = 3000;

ESP8266WiFiMulti WiFiMulti;
bool doorOpenFlag = false;
bool doorBellRangFlag = false;


void setup()
{
    Serial.begin(115200);

    // We start by connecting to a WiFi network
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while (WiFiMulti.run() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

    pinMode(D5, INPUT_PULLUP);
    pinMode(D6, INPUT_PULLUP);

    attachInterrupt(D5, onDoorBellPressed, RISING);
    attachInterrupt(D6, onDoorStateChanged, CHANGE);
}

ICACHE_RAM_ATTR void onDoorBellPressed() {
    doorBellRangFlag = true;
    Serial.println("Pin D5 rising!");
}


ICACHE_RAM_ATTR void onDoorStateChanged() {
    doorOpenFlag = true;
    Serial.println("Pin D6 switched!");
}

void loop()
{
    if(doorOpenFlag) {
        doorOpenFlag = false;
        if(digitalRead(D6) == HIGH) {
            Serial.println("Pin D6 switched on!");
            sendMessage("door 1");
            doorOpenFlag = false;
        } else {
            Serial.println("Pin D6 switched off!");
            sendMessage("door 0");
        }
    }

    if(doorBellRangFlag) {
        doorBellRangFlag = false;
        sendMessage("bell");
    }

    // handleEvents();

    // if(isDoorStateChanged) {
    //     if(isDoorOpen) {
    //         sendMessage("door 1");
    //         return;
    //     } else {
    //         sendMessage("door 0");
    //         return;
    //     }
    // } else if (isDoorBellRang) {
    //     sendMessage("bell");
    //     return;
    // }
}

void sendMessage(const char str[]) {
    Serial.print("connecting to ");
    Serial.print(host);
    Serial.print(':');
    Serial.println(port);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port))
    {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }

    // This will send the request to the server
    Serial.println(str);
    client.println(str);

    // //read back one line from server
    // Serial.println("receiving from remote server");
    // String line = client.readStringUntil('\n');
    // Serial.println(line);

    Serial.println("closing connection");
    client.stop();
}


// bool doorOpenLast = false;
// bool doorOpenCurrent = false;
// bool doorStateChanged = false;
// bool doorBellRang = false;
// unsigned long doorBellRangTime = 0;

// void handleEvents()
// {
//     unsigned long currentTime = millis();
//     doorOpenCurrent = true;
//     if (doorOpenLast != doorOpenCurrent)  // Switching condition
//     {
//         doorStateChanged = true;
//     }
//     if (true)  // On-Event Condition
//     {
//         if(currentTime < doorBellRangTime)
//             doorBellRangTime = 0;
//         if (!doorBellRang && currentTime - doorBellRangTime > 10 * 1000)
//         {
//             doorBellRang = true;
//             doorBellRangTime = currentTime;
//         }
//     }
//     doorOpenLast = doorOpenCurrent;
// }


// bool isDoorStateChanged()
// {
//     if (doorStateChanged) {
//         doorStateChanged = false;
//         return true;
//     }
//     return false;
// }


// bool isDoorOpen() {
//     return doorOpenCurrent;
// }


// bool isDoorBellRang()
// {
//     if (doorBellRang) {
//         doorBellRang = false;
//         return true;
//     }
//     return false;
// }
