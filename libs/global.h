/*
 * @Filename: global.h
 * @Authors: Guillermo Ortega Romo
 * @Description: This file contains all includes, defines and global variables
 *              used in the project
*/

#include <Arduino.h>

#ifdef ESP32   
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#endif

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "SinricPro.h"
#include "SinricProBlinds.h"


#define LED_R_PIN 13
#define LED_G_PIN 12
#define LED_B_PIN 14

#define MODE_PIN 21

#define V_MOTOR_1 18
#define V_MOTOR_2 19
#define H_MOTOR_1 17
#define H_MOTOR_2 16

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
AsyncWebServer server(80);

String ssid = "";
String password = "";

const char* PARAM_SSID = "inputSSID";
const char* PARAM_PSW = "inputPSW";

const char* PARAM_V_FRONT = "v_front";
const char* PARAM_V_STOP = "v_stop";
const char* PARAM_V_BACK = "v_back";

const char* PARAM_H_FRONT = "h_front";
const char* PARAM_H_STOP = "h_stop";
const char* PARAM_H_BACK = "h_back";

int wifi_mode = 0;

int vertical_blindsPosition = 0;
bool vertical_powerState = false;
int horizontal_blindsPosition = 0;
bool horizontal_powerState = false;