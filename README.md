# Vitinode
 
## INSTALLATION : 
    Vscode
    Platformio

### MISE EN MARCHE : 
    Fichier -> ouvir le dossier
    si besoin, supprime le fichier : .pio/libdepds/ttgo-lora-v32v1/TTTN_esp32/src/BLE*****

## BUILD

## UPDATE

## HARDWARE :
ESP32 : ttgo - lora v1
Capteur de temperature DHT22
Batterie : Pont diviseur de tension 

### Cablage
    DHTpin : 4
    DHT+ : +3.3V
    DHT- : Masse

        Batterie : GND    <- 3.3 kOhm ->    36    <- 3.3kOhms ->    +5V


# TTN
## Installation :
Créer un compte sur https://www.thethingsnetwork.org/
    Individual
    Europe1
    
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

# Programmation de la carte :

## configuration de configuration.h
Dans le dossier src/, duppliquer le fichier configuration_backup.h et le renommer configuration.h

Renseigner les clé qui doivent correspondre à ce qu'on a sur ttn/end device :
    -D devEui ="70B3D57ED0054974"; // Change to TTN Device EUI
	-D appEui = "0000000000000000"; // Change to TTN Application EUI
	-D appKey = "ED52F1573C19890A23FE023A273A7D3B"; // Change to TTN Application Key

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
Cliquer sur build (dans VS code, symbole valideer dans la barre du bas)
Ce test permet 
