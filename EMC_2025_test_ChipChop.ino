// EMC 2025
// Primer programa per testejar la plataforma ChipChop
//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#define TASKER_MAX_TASKS 10
#include "Tasker.h"
Tasker tasker;

// ************************No tocar res**************************************
#define CHIPCHOP_DEBUG true //desactivar per producció
#include "src/ChipChop_Config.h" 
#include "src/ChipChopEngine.h" 
extern ChipChopEngine ChipChop;
//Plugins inclosos: ChipChop Engine,Keep Alive,Preferences Manager,Auto Mode (pin control),
#include "src/ChipChopPlugins.h"
extern ChipChopPluginsManager ChipChopPlugins;
#include "src/ChipChop_Includes.h"
// *************************************************************************


// Credencials de ChipChop obtingudes a la consola
String server_uri = "wss://api14.chipchop.io/wsdev/";
String uuid = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
String auth_code = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
String device_id = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";


// AutoMode per GPIO's de sortida
// Per GPIO13
AutoStartPins on_off_pins[MAX_COMPONENTS] = {
{"GPIO13",13,LOW,"OFF",AUTO_MODE_RESTORE,{"ON","OFF"}},
};

// Callback que es "dispara al rebre un comandament de l'App
void ChipChop_onCommandReceived(String target_component,String command_value, String command_source, int command_age)
{
    Serial.print("[ChCh] Comandament rebut: ");
    Serial.print(target_component);
    Serial.print(" ");
    Serial.println(command_value);
}

void setup()
{
    // Iniciem Serial a 115200 bps
    Serial.begin(115200);
    delay(1000); // Petita espera, 1 segon
    Serial.println(" "); // Retorn de carro
    Serial.println("[boot] Comencem.....");

    // Configurem Inputs/outputs manualment, GPIO13 ja ho ha fet Autostart
    // De moment GPIO16
    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH); // Apagada a l'inici

    // Configurem credencials Wifi  
    WiFi.begin("viscalaterra", "pitufito*");
    // Connectem amb el router...
    Serial.println("[WiFi] Connectant....");
    int connRes = WiFi.waitForConnectResult(20000); //esperem 20 segons per donar temps a connectar al router
    if(connRes != WL_CONNECTED)
    {
        Serial.print("[ERR ] MERDA!! No cennecto al router, resetejo!!!!!");
        delay(1000);
        ESP.restart();
    }
    if(connRes == WL_CONNECTED)
    {
        Serial.println("[WiFi] Ja has connectat al router!");
    }

    // Ajustos de ChipChop engine
    // ************************No tocar res************************************** 
    ChipChop.debug(true); //set to false for production
    ChipChop.commandCallback(ChipChop_onCommandReceived);
    ChipChop.start(server_uri, uuid, device_id, auth_code);
    //Comencem plugins
    ChipChopPlugins.start();    
    //Comencem AutoMode pins control
    AutoMode.start(on_off_pins);
    // set here the initial status for all components that are not handled by the AutoMode plugin
    // ChipChop.updateStatus("<some component>",<some starting value>);
    // *************************************************************************
    
    
    // Configuració de tasques
    tasker.setInterval(intermitaLed16,1000); // Tasca 1, intermito led16 cada 1s
}

void loop()
{
    ChipChop.run(); // Gestió del ChipChop
    ChipChopPlugins.run();  // Idem
    tasker.loop(); // Gestió de tasques
    
}

// *******************Tasques*********************

// Tasca 1
void intermitaLed16()
{
  digitalWrite(16, !digitalRead(16));
}


