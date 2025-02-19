// Adapted from:
// https://github.com/daniloc/PicoW_HomeAssistant_Starter/tree/main/src

#ifndef SRC_HAINTEGRATION
#define SRC_HAINTEGRATION

#include <ArduinoHA.h>

class HAIntegration {
    public:
    void loop();
    void configure();
    // depending on what type of device you have, make different handlers
    // HASwitch is a device-type, see .pio\libdeps\pico\home-assistant-integration\src\device-types
    static void switchHandler(bool state, HASwitch* sender);
};

#endif // SRC_HAINTEGRATION
