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
		Serial.println("no file");
		return String();
	}
	String fileContent;
	while (file.available()) {
		fileContent += String((char)file.read());
	}
	file.close();
	return fileContent;
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Write a new value to SPIFFS
 */
void writeFile(fs::FS& fs, const char* path, const char* message) {
	File file = fs.open(path, "w");
	if (!file) {
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
	if (var == "i_ssid") {
		return readFile(SPIFFS, ssid_path);
	}
	else if (var == "i_pswd") {
		return readFile(SPIFFS, pswd_path);
	}
	else if (var == "i_v_time") {
		return vertical_t;
	}
	else if (var == "i_h_time") {
		return horizontal_t;
	}
	else if (var == "s_v_time") {
		return readFile(SPIFFS, v_time_path);
	}
	else if (var == "s_h_time") {
		return readFile(SPIFFS, h_time_path);
	}
	return String();
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Connect to Wifi with ssid and pasword previously saved in
 * 				 SPIFFS.
 */
void setupWiFi(String wifi_ssid, String wifi_pswd) {
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
	if (dnsServer.start(DNS_PORT, domainName, apIP)) {
	}

	// Send web page with input fields to client
	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send_P(200, "text/html", index_html, processor);
		});

	// Send a GET request to <ESP_IP>/get?input1=<inputMessage>
	server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
		String inputMessage = "";

		if (request->hasParam(PARAM_V_FRONT)) {
			digitalWrite(V_MOTOR_1, HIGH);
			digitalWrite(V_MOTOR_2, LOW);
			vertical_initial = millis();
		}

		if (request->hasParam(PARAM_V_STOP)) {
			digitalWrite(V_MOTOR_1, LOW);
			digitalWrite(V_MOTOR_2, LOW);
			vertical_finish = millis();
			vertical_time = vertical_finish - vertical_initial;
			sprintf(vertical_t, "%u", vertical_time);
		}

		if (request->hasParam(PARAM_V_BACK)) {
			digitalWrite(V_MOTOR_1, LOW);
			digitalWrite(V_MOTOR_2, HIGH);
			vertical_initial = millis();
		}

		if (request->hasParam(PARAM_H_FRONT)) {
			digitalWrite(H_MOTOR_1, HIGH);
			digitalWrite(H_MOTOR_2, LOW);
			horizontal_initial = millis();
		}

		if (request->hasParam(PARAM_H_STOP)) {
			digitalWrite(H_MOTOR_1, LOW);
			digitalWrite(H_MOTOR_2, LOW);
			horizontal_finish = millis();
			horizontal_time = horizontal_finish - horizontal_initial;
			sprintf(horizontal_t, "%u", horizontal_time);
		}

		if (request->hasParam(PARAM_H_BACK)) {
			digitalWrite(H_MOTOR_1, LOW);
			digitalWrite(H_MOTOR_2, HIGH);
			horizontal_initial = millis();
		}

		if (request->hasParam(PARAM_SSID) && request->hasParam(PARAM_PSW)) {
			inputMessage = request->getParam(PARAM_SSID)->value();
			writeFile(SPIFFS, ssid_path, inputMessage.c_str());

			inputMessage = request->getParam(PARAM_PSW)->value();
			writeFile(SPIFFS, pswd_path, inputMessage.c_str());
		}

		if (request->hasParam(PARAM_V_TIME)) {
			writeFile(SPIFFS, v_time_path, vertical_t);
		}

		if (request->hasParam(PARAM_H_TIME)) {
			writeFile(SPIFFS, h_time_path, horizontal_t);
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
 * @Description: Moves motor for vertical blinds
 */
void move_vertical_motor(double movement, int flag) {
	unsigned long start_time = millis();
	double current_time = 0;
	while (movement - (current_time) >= 0) {
		if (flag == 1) {
			digitalWrite(V_MOTOR_1, HIGH);
			digitalWrite(V_MOTOR_2, LOW);
		}
		else {
			digitalWrite(V_MOTOR_1, LOW);
			digitalWrite(V_MOTOR_2, HIGH);
		}
		current_time = millis() - start_time;
	}
	digitalWrite(V_MOTOR_1, LOW);
	digitalWrite(V_MOTOR_2, LOW);
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Gets actual position and calculates the movement for
 * vertical blinds
 */
void move_vertical_blinds() {
	double movement = vertical_nextPosition - v_position;

	if (movement > 0) {
		movement = movement / 100;
		movement = movement * vert_time;
		move_vertical_motor(movement, 1);
	}
	else if (movement < 0) {
		movement = movement * -1;
		movement = movement / 100;
		movement = movement * vert_time;
		move_vertical_motor(movement, 0);
	}
	v_position = vertical_nextPosition;
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Moves motor for horizontal blinds
 */
void move_horizontal_motor(double movement, int flag) {
	unsigned long start_time = millis();
	double current_time = 0;
	while (movement - (current_time) >= 0) {
		if (flag == 1) {
			digitalWrite(H_MOTOR_1, HIGH);
			digitalWrite(H_MOTOR_2, LOW);
		}
		else {
			digitalWrite(H_MOTOR_1, LOW);
			digitalWrite(H_MOTOR_2, HIGH);
		}
		current_time = millis() - start_time;
	}
	digitalWrite(H_MOTOR_1, LOW);
	digitalWrite(H_MOTOR_2, LOW);
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Gets actual position and calculates the movement for
 * horizontal blinds
 */
void move_horizontal_blinds() {
	double movement = horizontal_nextPosition - h_position;

	if (movement > 0) {
		movement = movement / 100;
		movement = movement * hor_time;
		move_horizontal_motor(movement, 1);
	}
	else if (movement < 0) {
		movement = movement * -1;
		movement = movement / 100;
		movement = movement * hor_time;
		move_horizontal_motor(movement, 0);
	}
	h_position = horizontal_nextPosition;
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Power state handler for vertical blind
 */
bool vertical_onPowerState(const String& deviceId, bool& state) {
	vertical_powerState = state;
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Callback function for vertical blind
 */
bool vertical_onRangeValue(const String& deviceId, int& position) {
	vertical_nextPosition = position;
	v_move = 1;
	writeFile(SPIFFS, v_pos_path, String(vertical_nextPosition).c_str());
	delay(500);
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description:
 */
bool vertical_onAdjustRangeValue(const String& deviceId, int& positionDelta) {
	vertical_blindsPosition += positionDelta;
	positionDelta = vertical_blindsPosition; // calculate and return absolute position
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Power state handling for horizontal blind
 */
bool horizontal_onPowerState(const String& deviceId, bool& state) {
	horizontal_powerState = state;
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description: Callback function for horizontal blind
 */
bool horizontal_onRangeValue(const String& deviceId, int& position) {

	horizontal_nextPosition = position;
	h_move = 1;
	writeFile(SPIFFS, h_pos_path, String(horizontal_nextPosition).c_str());
	delay(500);
	return true; // request handled properly
}

/*
 * @Author: Guillermo Ortega Romo
 * @Description:
 */
bool horizontal_onAdjustRangeValue(const String& deviceId, int& positionDelta) {
	horizontal_blindsPosition += positionDelta;
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
		return;
	}
#else
	if (!SPIFFS.begin()) {
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

		ssid = readFile(SPIFFS, ssid_path);
		password = readFile(SPIFFS, pswd_path);

		v_time = readFile(SPIFFS, v_time_path);
		if (v_time.length() > 0) {
			vert_time = v_time.toInt();
		}
		else {
			vert_time = 0;
		}

		h_time = readFile(SPIFFS, h_time_path);
		if (h_time.length() > 0) {
			hor_time = h_time.toInt();
		}
		else {
			hor_time = 0;
		}

		v_pos = readFile(SPIFFS, v_pos_path);
		if (v_pos.length() > 0) {
			v_position = v_pos.toInt();
		}
		else {
			v_position = 0;
		}

		h_pos = readFile(SPIFFS, h_pos_path);
		if (h_pos.length() > 0) {
			h_position = h_pos.toInt();
		}
		else {
			h_position = 0;
		}

		Serial.println(ssid);
		Serial.println(password);
		Serial.println(vert_time);
		Serial.println(hor_time);
		Serial.println(v_position);
		Serial.println(h_position);

		setupWiFi(ssid, password);
		setupSinricPro();
	}
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
		if (v_move == 1) {
			move_vertical_blinds();
			v_move = 0;
		}

		if (h_move == 1) {
			move_horizontal_blinds();
			h_move = 0;
		}
	}

	if (digitalRead(MODE_PIN) != wifi_mode) {
		mode_change();
	}
}

