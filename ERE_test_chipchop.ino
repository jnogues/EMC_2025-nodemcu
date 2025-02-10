
// ERE_test_chipchop.ino
#include <Arduino.h>

#include <ESP8266WiFi.h>
#define CHIPCHOP_DEBUG true //set to false for production

#include "src/ChipChop_Config.h" 
#include "src/ChipChopEngine.h" 
extern ChipChopEngine ChipChop;
//Plugins included: ChipChop Engine,Keep Alive,Preferences Manager,Auto Mode (pin control),
#include "src/ChipChopPlugins.h"
extern ChipChopPluginsManager ChipChopPlugins;
#include "src/ChipChop_Includes.h"

// Credencials que cal modificar
String server_uri = "wss://api14.chipchop.io/wsdev/";
String uuid = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
String auth_code = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
String device_id = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";

//Set the pins that AutoMode should control for you
AutoStartPins on_off_pins[MAX_COMPONENTS] = 
{
   {"GPIO13",13,LOW,"OFF",AUTO_MODE_RESTORE,{"ON","OFF"}},
};

void ChipChop_onCommandReceived(String target_component,String command_value, String command_source, int command_age)
{
    Serial.println(target_component);
    Serial.println(command_value);
}


void setup()
{
    Serial.begin(115200);
    delay(1000);
   
    WiFi.begin("emed", "pitufito*");
    
    Serial.println("WiFi Connecting...");
    int connRes = WiFi.waitForConnectResult(5000); //wait up to 5 seconds to establish the connection

    if(connRes != WL_CONNECTED)
    {
        Serial.print("Can not connect to the WiFi");
    }

    //basic ChipChop engine settings, all others are in the ChipChop_config.h ////
    ChipChop.debug(true); //set to false for production
    ChipChop.commandCallback(ChipChop_onCommandReceived);
    ChipChop.start(server_uri, uuid, device_id, auth_code);

    //Start all plugins
    ChipChopPlugins.start();
    
    //Start AutoMode pins control
    AutoMode.start(on_off_pins);
}

void loop()
{  
    ChipChop.run();
    ChipChopPlugins.run();   
}


