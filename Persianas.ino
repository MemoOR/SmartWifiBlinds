/*
 * @Filename: Persianas.ino
 * @Authors: Guillermo Ortega Romo
 * @Description: This file contains all functions used on the project
*/

#include "libs/global.h"
#include "libs/credentials.h"
#include "libs/index.h"  

// ------------------------------------------------------------------------------ //
// --------------------------Functions for html handle--------------------------- //
// ------------------------------------------------------------------------------ //
/*
 * @Author: Guillermo Ortega Romo
 * @Description: Not found page
 */
void notFound(AsyncWebServerRequest* request) {
	request->send(404, "text/plain", "Not found");
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Read a value from SPIFFS file
 */
String readFile(fs::FS& fs, const char* path) {
	File file = fs.open(path, "r");
	if (!file || file.isDirectory()) {
		Serial.println("- empty file or failed to open file");
		return String();
	}
	String fileContent;
	while (file.available()) {
		fileContent += String((char)file.read());
	}
	file.close();
	Serial.println(fileContent);
	return fileContent;
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Write a new value to SPIFFS
 */
void writeFile(fs::FS& fs, const char* path, const char* message) {
	File file = fs.open(path, "w");
	if (!file) {
		Serial.println("- failed to open file for writing");
		return;
	}
	if (file.print(message)) {
		Serial.println("- file written");
	}
	else {
		Serial.println("- write failed");
	}
	file.close();
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Text processor for html server
 */
String processor(const String& var) {
	//Serial.println(var);
	if (var == "i_ssid") {
		return readFile(SPIFFS, "/inputSSID.txt");
	}
	else if (var == "i_pswd") {
		return readFile(SPIFFS, "/inputPSW.txt");
	}
	return String();
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Connect to Wifi with ssid and pasword previously saved in
 * 				 SPIFFS.
 */
void setupWiFi(String wifi_ssid, String wifi_pswd) {
	Serial.printf("\r\n[Wifi]: Connecting");
	WiFi.begin(wifi_ssid.c_str(), wifi_pswd.c_str());

	while (WiFi.status() != WL_CONNECTED) {
		Serial.printf(".");
		delay(250);
	}
	IPAddress localIP = WiFi.localIP();
	Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Handler for async web server for blinds configuration
 * 				 on acces point mode
 */
void start_server() {

	WiFi.mode(WIFI_AP); //Access Point mode
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	WiFi.softAP("BlindsConfig", "12345678");    //Password length minimum 8 char  

	dnsServer.setTTL(300);
	dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
	if (dnsServer.start(DNS_PORT, "myblinds.config", apIP)) {
		Serial.println("DNS start");
	}

	// Send web page with input fields to client
	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send_P(200, "text/html", index_html, processor);
		});

	// Send a GET request to <ESP_IP>/get?input1=<inputMessage>
	server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
		String inputMessage = "";

		if (request->hasParam(PARAM_V_FRONT)) {
			Serial.println("V_F");
			digitalWrite(V_MOTOR_1, HIGH);
			digitalWrite(V_MOTOR_2, LOW);
		}

		if (request->hasParam(PARAM_V_STOP)) {
			Serial.println("V_S");
			digitalWrite(V_MOTOR_1, LOW);
			digitalWrite(V_MOTOR_2, LOW);
		}

		if (request->hasParam(PARAM_V_BACK)) {
			Serial.println("V_B");
			digitalWrite(V_MOTOR_1, LOW);
			digitalWrite(V_MOTOR_2, HIGH);
		}

		if (request->hasParam(PARAM_H_FRONT)) {
			Serial.println("H_F");
			digitalWrite(H_MOTOR_1, HIGH);
			digitalWrite(H_MOTOR_2, LOW);
		}

		if (request->hasParam(PARAM_H_STOP)) {
			Serial.println("H_S");
			digitalWrite(H_MOTOR_1, LOW);
			digitalWrite(H_MOTOR_2, LOW);
		}

		if (request->hasParam(PARAM_H_BACK)) {
			Serial.println("H_B");
			digitalWrite(H_MOTOR_1, LOW);
			digitalWrite(H_MOTOR_2, HIGH);
		}

		if (request->hasParam(PARAM_SSID)) {
			inputMessage = request->getParam(PARAM_SSID)->value();
			writeFile(SPIFFS, "/inputSSID.txt", inputMessage.c_str());
		}

		if (request->hasParam(PARAM_PSW)) {
			inputMessage = request->getParam(PARAM_PSW)->value();
			writeFile(SPIFFS, "/inputPSW.txt", inputMessage.c_str());
		}

		request->send(200, "text/text", inputMessage);
		});

	server.onNotFound(notFound);
	server.begin();
}

// ------------------------------------------------------------------------------ //
// ----------------------Functions for blinds movement--------------------------- //
// ------------------------------------------------------------------------------ //
/*
 * @Author: Guillermo Ortega Romo
 * @Description: Power state handler for vertical blind
 */
bool vertical_onPowerState(const String& deviceId, bool& state) {
	Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state ? "on" : "off");
	vertical_powerState = state;
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Callback function for vertical blind
 */
bool vertical_onRangeValue(const String& deviceId, int& position) {
	Serial.printf("Device %s set position to %d\r\n", deviceId.c_str(), position);
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description:
 */
bool vertical_onAdjustRangeValue(const String& deviceId, int& positionDelta) {
	vertical_blindsPosition += positionDelta;
	Serial.printf("Device %s position changed about %i to %d\r\n", deviceId.c_str(), positionDelta, vertical_blindsPosition);
	positionDelta = vertical_blindsPosition; // calculate and return absolute position
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Power state handling for horizontal blind
 */
bool horizontal_onPowerState(const String& deviceId, bool& state) {
	Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state ? "on" : "off");
	horizontal_powerState = state;
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Callback function for horizontal blind
 */
bool horizontal_onRangeValue(const String& deviceId, int& position) {
	Serial.printf("Device %s set position to %d\r\n", deviceId.c_str(), position);

	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description:
 */
bool horizontal_onAdjustRangeValue(const String& deviceId, int& positionDelta) {
	horizontal_blindsPosition += positionDelta;
	Serial.printf("Device %s position changed about %i to %d\r\n", deviceId.c_str(), positionDelta, horizontal_blindsPosition);
	positionDelta = horizontal_blindsPosition; // calculate and return absolute position
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Initialize blinds devices
 */
void setupSinricPro() {
	// get a new Blinds device from SinricPro
	SinricProBlinds& verticalBlinds = SinricPro[V_BLINDS_ID];
	verticalBlinds.onPowerState(vertical_onPowerState);
	verticalBlinds.onRangeValue(vertical_onRangeValue);
	verticalBlinds.onAdjustRangeValue(vertical_onAdjustRangeValue);

	// get a new Blinds device from SinricPro
	SinricProBlinds& horizontalBlinds = SinricPro[H_BLINDS_ID];
	horizontalBlinds.onPowerState(horizontal_onPowerState);
	horizontalBlinds.onRangeValue(horizontal_onRangeValue);
	horizontalBlinds.onAdjustRangeValue(horizontal_onAdjustRangeValue);

	// setup SinricPro
	SinricPro.onConnected([]() { Serial.printf("Connected to SinricPro\r\n"); });
	SinricPro.onDisconnected([]() { Serial.printf("Disconnected from SinricPro\r\n"); });
	SinricPro.begin(APP_KEY, APP_SECRET);
}

// ------------------------------------------------------------------------------ //
// ----------------------Main Set-up and Loop functions-------------------------- //
// ------------------------------------------------------------------------------ //
/*
 * @Author: Guillermo Ortega Romo
 * @Description: Interrupt function for handling mode change between AP and
 *  			 Station mode
 */
void mode_change() {
	ESP.restart();
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Set up function for the app.
 */
void setup() {
	Serial.begin(BAUD_RATE);
	Serial.printf("\r\n\r\n");

	pinMode(MODE_PIN, INPUT);

	pinMode(LED_R_PIN, OUTPUT);
	pinMode(LED_G_PIN, OUTPUT);
	pinMode(LED_B_PIN, OUTPUT);

	pinMode(V_MOTOR_1, OUTPUT);
	pinMode(V_MOTOR_2, OUTPUT);
	pinMode(H_MOTOR_1, OUTPUT);
	pinMode(H_MOTOR_2, OUTPUT);

	// Initialize SPIFFS
#ifdef ESP32
	if (!SPIFFS.begin(true)) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}
#else
	if (!SPIFFS.begin()) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}
#endif

	wifi_mode = digitalRead(MODE_PIN);

	if (wifi_mode == 0) {
		digitalWrite(LED_R_PIN, LOW);
		digitalWrite(LED_G_PIN, LOW);
		digitalWrite(LED_B_PIN, HIGH);
		start_server();
	}
	else {
		digitalWrite(LED_R_PIN, LOW);
		digitalWrite(LED_G_PIN, HIGH);
		digitalWrite(LED_B_PIN, LOW);
		ssid = readFile(SPIFFS, "/inputSSID.txt");
		password = readFile(SPIFFS, "/inputPSW.txt");

		setupWiFi(ssid, password);
		setupSinricPro();
	}
	attachInterrupt(digitalPinToInterrupt(MODE_PIN), mode_change, CHANGE);
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Main Loop
 */
void loop() {
	if (wifi_mode == 0) {
		dnsServer.processNextRequest();
	}
	else {
		SinricPro.handle();
	}
}

