# Vitinode
https://github.com/acetayls/Vitinode

Code pour la programmation de station météo développé par vitipole, chambre d'agriculture de Sâone et loire


## Pour commencer : 
Installer vscode 
    https://code.visualstudio.com/

Installer platformio
    - https://docs.platformio.org/en/stable//integration/ide/vscode.html#installation
(Attention, on rencontre des bug avevc python11, telechargez une version de python8 plutôt)

Télecharger le dossier 
    https://github.com/acetayls/Vitinode/archive/refs/heads/main.zip


## Hardware :
    ESP32 avec LoRA: ttgo - lora v1
    DHT22 : Capteur de temperature
    2 resistances de 3.3 kOhms (niveau de batterie)
    Boitier sur mesure du vitipole

    cable micro-usb : Pour la programmation et la charge de la batterie

### Cablage
    DHTpin : 4
    DHT+ : +3.3V
    DHT- : Masse
####
    Batterie : GND    <- 3.3 kOhm ->   PIN 36    <- 3.3kOhms ->    +5V


# The Things Network
## Installation :
Créer un compte sur https://www.thethingsnetwork.org/
- Individual
- Europe1
    
Rejoindre l'oganisation : vitilab-chambragri



## Ajouter la Gateway :
suivre : https://www.thethingsindustries.com/docs/gateways/models/thethingsindoorgateway/

Attention : le gateway EUI correspond à XXXXXX-FFFE-XXXXXX (les caractéère FFFE ne sont pas affiché sur la gateway ... )
Frequency plan : Europe 863-870 MHz (SF9 for RX2  - recommended)




## Ajouter un Node : 
choisir l'application vini-meteo (elle doit exister apres avoir rejoind l'organisation vitilab-chambragri).
Sinon créer une application.

End device

    + Add end device
        [X] Enter end device specifics manually
        Frequency plan : Europe 863-870 MHz (SF9 for RX2 - recommended)
        LoRaWAN version : LoRaWAN Specification 1.0.2
        Regional Parameters version : RP001 Regional Parameters 1.0.2

        JoinEUI : 00 00 00 00 00 00 00 00

        DevEUI : Generate
        AppKey : Generate
        End device Id : nom facile à identifier

### Payload formater
Pour communiquer avec le serveur https://iot.protechebdo.fr/devices, les data des stations doivent être mise en forme.
On utilise un uplink payload formatter dans ttn. Il est programmé par defaut dans l'application TTN

    function decodeUplink(input) {
        var data = {};
        var warnings = [];

        if (input.fPort == 10) {
            if (input.bytes[0] == 1) {
            data.sonde = input.bytes[1] + input.bytes[2]/100;
            data.battery = input.bytes[3] + input.bytes[4]/100;
            }
            else if (input.bytes[0] == 2) { 
            //data.counter = (input.bytes[0] << 8) + input.bytes[1];
            data.temperature = input.bytes[3] + input.bytes[4]/100;
            data.humidity = input.bytes[1] + input.bytes[2]/100;
            data.battery = input.bytes[5] + input.bytes[6]/100;
            }
            
        }
        else {
            warnings.push("Unsupported fPort");
        }
        return {
            data: data,
            warnings: warnings
        };
    }

### Webhoock
Le webhook permet la communication entre TTN et protechhebdo

# Programmation de l'esp32 ttgo lora v1

## parametrage de configuration.h
Dans le dossier src, dupliquer le fichier configuration_backup.h et le renommer configuration.h

Renseigner les clé qui doivent correspondre à ce qu'on a sur ttn concernant le end device :

    const char* devEui = "70B3D57ED0054974"; // Change to TTN Device EUI
    const char* appEui = "0000000000000000"; // Change to TTN Application EUI
    const char* appKey = "ED52F1573C19890A23FE023A273A7D3B"; // Chaneg to TTN Application Key

Ce fichier doit avoir la forme :

    /*****************************************
    * 
    * CHANGER LES VALEURS SUIVANTE selon le configuration !!!!
    * 
    /*****************************************/

    // identifiant du node TTN
    const char* devEui = "70B3D57ED0054974"; // Change to TTN Device EUI
    const char* appEui = "0000000000000000"; // Change to TTN Application EUI
    const char* appKey = "ED52F1573C19890A23FE023A273A7D3B"; // Chaneg to TTN Application Key

    // selection station meteo (2) ou vitibox (1)
    #define VITI_TYPE 1 //si vitbox : 1 --> sonde DS18B20 sinon 2 --> sonde DHT22

    // Temps entre les mesures (en secondes)
    const unsigned long secs_between_send = 60*30;

    // Coefficient de la batterie (en fonction des valeurs des resistances, mesure sur le terrain)
    const double batt_coeff = 0.001074481;
    
    /*****************************************
    * 
    * FIN des valeurs à modifier !
    * 
    /*****************************************/


## test de compilation :
Cliquer sur build (dans VS code, symbole valider dans la barre du bas)

    si besoin, supprimer le fichier : .pio/libdepds/ttgo-lora-v32v1/TTTN_esp32/src/BLE*****

La réussite du build permet d'être sur que le programme s'execute correctement

## upload - programmation de l'esp32

# Sur Macos
Si on a une erreur comme :

    Using manually specified: /dev/cu.usbmodem1462401
    Uploading .pio/build/ttgo-lora32-v1/firmware.bin
    esptool.py v3.3
    Serial port /dev/cu.usbmodem1462401
    Connecting......
    Chip is ESP32-D0WDQ6-V3 (revision 3)
    Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
    Crystal is 26MHz
    MAC: 08:3a:f2:66:8d:b4
    Uploading stub...

    A fatal error occurred: Failed to write to target RAM (result was 01070000: Operation timed out)
    *** [upload] Error 2
    ============================================================== [FAILED] Took 52.62 seconds ==============================================================

Modifier le fichier platformio.ini : 

apres :

    build_flags =
    -D ARDUINO_LMIC_PROJECT_CONFIG_H_SUPPRESS
    -D hal_init=LMICHAL_init
    -D CFG_eu868=1
    -D CFG_sx1276_radio=1
    -D LMIC_DEBUG_LEVEL=1
    -D LMIC_PRINTF_TO=Serial

ajouter une ligne du type (à modifier en fonction du nom du port, on peut le trouver dans l'onglet PIO Home / Devices)

    upload_port = /dev/cu.wchusbserial146241
