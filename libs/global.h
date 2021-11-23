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
#define H_MOTOR_1 16
#define H_MOTOR_2 17

const String domainName = "myblinds.config";
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
AsyncWebServer server(80);

String ssid = "";
String password = "";
String v_time = "";
String h_time = "";
String v_pos = "";
String h_pos = "";

int vert_time;
int hor_time;
int v_position;
int h_position;

const char* PARAM_SSID = "inputSSID";
const char* PARAM_PSW = "inputPSW";

const char* PARAM_V_TIME = "submit_v_time";
const char* PARAM_H_TIME = "submit_h_time";

const char* PARAM_V_FRONT = "v_front";
const char* PARAM_V_STOP = "v_stop";
const char* PARAM_V_BACK = "v_back";

const char* PARAM_H_FRONT = "h_front";
const char* PARAM_H_STOP = "h_stop";
const char* PARAM_H_BACK = "h_back";

const char* ssid_path = "/inputSSID.txt";
const char* pswd_path = "/inputPSW.txt";

const char* v_time_path = "/vertical_time.txt";
const char* h_time_path = "/horizontal_time.txt";

const char* v_pos_path = "/v_position.txt";
const char* h_pos_path = "/h_position.txt";

int v_move = 0;
int h_move = 0;

int wifi_mode = 0;

int vertical_blindsPosition = 0;
int vertical_nextPosition = 0;
bool vertical_powerState = false;

int horizontal_blindsPosition = 0;
int horizontal_nextPosition = 0;
bool horizontal_powerState = false;

unsigned long vertical_initial;
unsigned long vertical_finish;
unsigned long vertical_time;

unsigned long horizontal_initial;
unsigned long horizontal_finish;
unsigned long horizontal_time;

char vertical_t[40];
char horizontal_t[40];