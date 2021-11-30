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

#include "credentials.h"

// Pins for rgb led
#define LED_R_PIN 14
#define LED_G_PIN 12
#define LED_B_PIN 13

// Pins for manual movement
#define UP_BUTTON 25
#define DOWN_BUTTON 33

// Pin for wifi mode selection
#define MODE_PIN 35

// Pin to choose motor direction 
#define MOTOR_POLARITY 32

// Pins for motor movement
#define V_MOTOR_1 18
#define V_MOTOR_2 19
#define H_MOTOR_1 16
#define H_MOTOR_2 17

//Web server configuration on AP mode
const String domainName = "myblinds.config";
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
AsyncWebServer server(80);

// Variables to read from SPIFFS for Client mode
String ssid = "";
String password = "";
String v_time = "";
String h_time = "";
String v_pos = "";
String h_pos = "";

// Configuration for position and time converted from String 
int vert_time;
int hor_time;
int v_position;
int h_position;

// HTML Parameters for input components
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

// SPIFFS File paths
const char* ssid_path = "/inputSSID.txt";
const char* pswd_path = "/inputPSW.txt";

const char* v_time_path = "/vertical_time.txt";
const char* h_time_path = "/horizontal_time.txt";

const char* v_pos_path = "/v_position.txt";
const char* h_pos_path = "/h_position.txt";

// Check if a blind needs to move 
int v_move = 0;
int h_move = 0;

// wifi mode and motor direction
int wifi_mode = 0;
int motor_polarity = 0;

// Decides wich motor moves on manual mode
int manual_motor = 0;

// Declare sinric pro devices
SinricProBlinds& verticalBlinds = SinricPro[V_BLINDS_ID];
SinricProBlinds& horizontalBlinds = SinricPro[H_BLINDS_ID];

// Variables for vertical blinds managment
int vertical_blindsPosition = 0;
int vertical_nextPosition = 0;
bool vertical_powerState = false;
unsigned long vertical_initial;
unsigned long vertical_finish;
unsigned long vertical_time;
char vertical_t[40];

// Variables for horizontal blinds managment
int horizontal_blindsPosition = 0;
int horizontal_nextPosition = 0;
bool horizontal_powerState = false;
unsigned long horizontal_initial;
unsigned long horizontal_finish;
unsigned long horizontal_time;
char horizontal_t[40];