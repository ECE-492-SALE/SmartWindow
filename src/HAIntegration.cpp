// Adapted from:
// https://github.com/daniloc/PicoW_HomeAssistant_Starter/tree/main/src

#include "HAIntegration.h"
#include "Credentials.h"

#include <ArduinoHA.h>
#include <WiFi.h>

//Adapted via:
//  https://github.com/dawidchyrzynski/arduino-home-assistant/blob/main/examples/nano33iot/nano33iot.ino

#define LED_PIN     LED_BUILTIN
#define DIRECTION          22
#define POWER              21

#define INPUT1_PIN  10
#define INPUT2_PIN  11
#define OUTPUT1_PIN 26
#define OUTPUT2_PIN 27

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);
// HASwitch is an example of a HA device-type. You may need to initialize an instance of a different class for your device.
// See .pio\libdeps\pico\home-assistant-integration\src\device-types
HASwitch led("windowLED");
HASwitch direction("Direction");
HAButton power("Power");


void HAIntegration::configure() {

    //Prepare LED:

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);    
    
    // //Prepare GPIO pins:

    pinMode(DIRECTION, OUTPUT);
    digitalWrite(DIRECTION, LOW);  

    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, LOW);  

    //setup
    pinMode(INPUT1_PIN, INPUT);
    pinMode(INPUT2_PIN, INPUT);
    pinMode(OUTPUT1_PIN, OUTPUT);
    pinMode(OUTPUT2_PIN, OUTPUT);

    //Set device ID as MAC address

    byte mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    //Device metadata:
    device.setName("Pico Window");
    device.setSoftwareVersion("0.1");

    // handle switch state(s)
    led.onCommand(switchHandler);
    led.setName("Window LED"); // optional

    power.onCommand(onButtonCommand);
    power.setName("TOGGLE");
    direction.onCommand(switchHandler);
    direction.setName("Up/Down");


    Serial.print("Connecting to MQTT\n");
    
    if (mqtt.begin(MQTT_BROKER, MQTT_LOGIN, MQTT_PASSWORD) == true) {
        Serial.print("Connected to MQTT broker");
    } else {
        Serial.print("Could not connect to MQTT broker");
    }
}


void HAIntegration::switchHandler(bool state, HASwitch* sender) {
    if (sender == &led){
    digitalWrite(LED_PIN, (state ? HIGH : LOW));
    } else if (sender == &direction) {
    digitalWrite(DIRECTION, (state ? HIGH : LOW));
    }
    sender->setState(state);  // report state back to Home Assistant
}

// might need to remove this.
void HAIntegration::onButtonCommand(HAButton* sender) {
    if (sender == &power) {
        digitalWrite(POWER, HIGH);
    } else if (sender != &power){
        digitalWrite(POWER, LOW);
    }
}



//Cover
HACover cover("Cover", HACover::PositionFeature);
//setup
// cover.onCommand(onCoverCommand);
// cover.setName("My cover"); // optional

// //future use 

// void onCoverCommand(HACover::CoverCommand cmd, HACover* sender) {
//     if (cmd == HACover::CommandOpen) {
//         Serial.println("Command: Open");
//         sender->setState(HACover::StateOpening); // report state back to the HA
//     } else if (cmd == HACover::CommandClose) {
//         Serial.println("Command: Close");
//         sender->setState(HACover::StateClosing); // report state back to the HA
//     } else if (cmd == HACover::CommandStop) {
//         Serial.println("Command: Stop");
//         sender->setState(HACover::StateStopped); // report state back to the HA
//     }

//     // Available states:
//     // HACover::StateClosed
//     // HACover::StateClosing
//     // HACover::StateOpen
//     // HACover::StateOpening
//     // HACover::StateStopped

//     // You can also report position using setPosition() method
// }



// void onNumberCommand(HANumeric number, HANumber* sender)
// {
//     if (!number.isSet()) {
//         // the reset command was send by Home Assistant
//     } else {
//         // you can do whatever you want with the number as follows:
//         int8_t numberInt8 = number.toInt8();
//         int16_t numberInt16 = number.toInt16();
//         int32_t numberInt32 = number.toInt32();
//         uint8_t numberUInt8 = number.toUInt8();
//         uint16_t numberUInt16 = number.toUInt16();
//         uint32_t numberUInt32 = number.toUInt32();
//         float numberFloat = number.toFloat();
//     }

//     sender->setState(number); // report the selected option back to the HA panel
// }





void HAIntegration::loop() {
    mqtt.loop();
}
