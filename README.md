# Vitinode
https://github.com/acetayls/Vitinode

    Code pour la programmation de station météo et autres capteurs connectés développés par vinipole sud bourgogne (VITILAB), chambre d'agriculture de Sâone et loire
    Voir fiche vitinode pour explications complémentaires
    voir https://www.thingiverse.com/vitilab/designs pour les modèles 3D

Trajet du signal :
Station météo (end device) -----LoRA--------> GATEWAY -----Wifi-----> Internet | TheThingsNework | iot.protechebdo.fr

## Pour commencer : 
Installer vscode 
    https://code.visualstudio.com/

Installer platformio
    - https://docs.platformio.org/en/stable//integration/ide/vscode.html#installation
(Attention, on rencontre des bug avevc python11, telechargez une version de python8 plutôt)

Télecharger le dossier 
    https://github.com/acetayls/Vitinode/archive/refs/heads/main.zip
## liste des projets
    // sélection du capteur connecté (uncomment le type en supprimant // et commenter les autres avec // (Ctrl+/) )
    //#define VITI_TYPE 1 // si VINIBOX (température cuve): Sonde DS18B20 --> 1
    //#define VITI_TYPE 2 // si VITINODE (station météo T°/hygro air): sonde DHT22 --> 2
    //#define VITI_TYPE 3 // si VITISOL (hygro du sol): sonde capa --> 3 
    //#define VITI_TYPE 4 // si VITINODEXTEND (station météo T°/hygro air+sol): sonde capa + DS18B20 + DHT22 --> 4 
    //#define VITI_TYPE 5 // si AGRISONAR (niveau d'eau) : sonde HCSR04 --> 5
    //#define VITI_TYPE 6 // si VINIPOWER (compteur d'énergie) : EASTRON SDM530 --> 6
    //#define VITI_TYPE 7 // si VITIPLUIE (pluviomètre): MS-WH-SP-RG --> 7

## Hardware :
    La liste de matériel n'est donnée qu'à titre indicatif et n'est pas une publicité pour un revendeur.
    COMMUN PROJETS :
        ESP32 avec LoRA: ttgo - lora v1    https://fr.aliexpress.com/item/32845370112.html?spm=a2g0o.order_list.order_list_main.5.2d1b5e5b6qKGJf&gatewayAdapt=glo2fra
        2 résistances identiques (e.g. 2 resistances de 3.3 kOhms) (pour lecture niveau de batterie)    https://fr.rs-online.com/web/p/resistances-traversantes/1997822?gb=s
        câbles jumper    https://www.amazon.fr/MQOUO-120pcs-Femelle-Breadboard-Cables/dp/B0BLYRKWTC/ref=sr_1_13?keywords=wire+jumper&sr=8-13
        fer à souder avec étain (et pour aller plus loin tout le matériel de bricolage de base (multimètre, pince a denuder, pistolet à colle...))
        cable micro-usb : Pour la programmation et la charge de la batterie    https://fr.rs-online.com/web/p/cables-usb/1733530
        batterie lipo   https://fr.aliexpress.com/item/4001138773169.htmlspm=a2g0o.order_list.order_list_main.43.d4eb5e5by97gcL&gatewayAdapt=glo2fra
        (attention température d'utilisation des batteries car les lipo craignent le gel--> https://riton-duino.blogspot.com/2020/12/alimentation-par-batterie-panneaux.html
        panneau solaire pour tous les projets sur batteries exposés au soleil   https://fr.rs-online.com/web/p/panneaux-solaires/1793740
        (possible de rajouter régulateur de charge TP4056 en fonction du panneau solaire choisi --> ritonduino/alim batterie+panneaux solaires
        https://fr.aliexpress.com/item/1005004432351925.html?spm=a2g0o.order_list.order_list_main.54.d4eb5e5by97gcL&gatewayAdapt=glo2fra    )
        VITITYPE 1, 3, 5, 7
            boitier étanche    https://fr.aliexpress.com/item/1005001526260365.html?spm=a2g0o.order_list.order_list_main.94.d4eb5e5by97gcL&gatewayAdapt=glo2fra
        VITITYPE 2, 4
            Boitier sur mesure du VITILAB adapté au VITINODE    prendre contact avec VITILAB ou voir dans dossier stl
        VITITYPE 6, 7
            optocoupleur    https://fr.aliexpress.com/item/1005003242672161.html?spm=a2g0o.order_list.order_list_main.23.d4eb5e5by97gcL&gatewayAdapt=glo2fra
    VITITYPE 1 :
        sonde DS18B20 avec plugable terminal    https://fr.aliexpress.com/item/32839776524.html?gatewayAdapt=glo2fra
        en option : écran oled    https://www.amazon.fr/IZOKEE-Affichage-Afficheur-Raspberry-Microcontr%C3%B4leur/dp/B076PDVFQD
    VITITYPE 2 :
        DHT22 : Capteur de température    https://fr.aliexpress.com/item/32753887461.html?spm=a2g0o.order_list.order_list_main.28.d4eb5e5by97gcL&gatewayAdapt=glo2fra
    VITITYPE 3 :
        capteur hygrométrie    https://fr.aliexpress.com/item/32864255890.html?spm=a2g0o.order_list.order_list_main.17.d4eb5e5by97gcL&gatewayAdapt=glo2fra
    VITITYPE 4 :
        matériels VITITYPE 1, 2 et 3
    VITITYPE 5 :
        si capteur ultrason non étanche HCSR04      https://fr.rs-online.com/web/p/complements-bbc-micro-bit/2153181?gb=b
        si capteur ultrason étanche DYPA02YY    https://fr.aliexpress.com/item/1005003107797097.html?spm=a2g0o.order_list.order_list_main.59.d4eb5e5by97gcL&gatewayAdapt=glo2fra
    VITITYPE 6 :
        boite de dérivation étanche https://elec44.fr/legrand/6588-legrand-plexo-bte-rect-220x170x86-tanche-gris-embout-14-ip55-ik07-750c-rf-092062-3245060920623.html
        compteur d'énergie eastron SDM530D     https://fr.aliexpress.com/item/32347384739.html?spm=a2g0o.order_list.order_list_main.64.d4eb5e5by97gcL&gatewayAdapt=glo2fra
        prise mâle triphasé 3P+T+N    https://fr.rs-online.com/web/p/connecteurs-de-puissance-industriels/2144085
        prise femelle triphasé 3P+T+N    https://fr.rs-online.com/web/p/connecteurs-de-puissance-industriels/2144175
        Alimentation à découpage Meanwell rs 15 5    https://fr.rs-online.com/web/p/alimentations-a-decoupage/0413673
        câble triphasé 5 fils 3P+T+N
    VITITYPE 7 :
        pluviomètre     https://fr.aliexpress.com/item/1000001838878.html?spm=a2g0o.order_list.order_list_main.33.d4eb5e5by97gcL&gatewayAdapt=glo2fra
## Cablage 
### alimentation
    VITITYPE 1, 2, 3, 4
        "+" --> +3,3V
        "-" --> GND
    VITITYPE 5, 6, 7
        "+" --> +5V
        "-" --> GND
### données
    VITITYPE 1 --> pin 17
    VITITYPE 2 --> pin 16
    VITITYPE 3 --> pin 38
    VITITYPE 4 --> pins 16, 17 et 38
    VITITYPE 5 --> pins 16 (TRIG) et 17 (ECHO)
    VITITYPE 6 --> pin 16 
    VITITYPE 7 --> pin 16
    
### pont diviseur de tension
    Batterie : GND    <- 3.3 kOhm ->   PIN 36    <- 3.3kOhms ->    +5V


# The Things Network
## Installation :
Créer un compte sur https://www.thethingsnetwork.org/
- Individual
- Europe1
    
Rejoindre l'oganisation : 

    + add organiization
    vitilab-chambragri

## Ajouter la Gateway :
suivre : https://www.thethingsindustries.com/docs/gateways/models/thethingsindoorgateway/

Attention : le gateway EUI correspond à XXXXXX-FFFE-XXXXXX (les caractéère FFFE ne sont pas affiché sur la gateway ... )
Le claim authentification code est celui qui correspond au wifi (derriere apres Wifi PW : )
Frequency plan : Europe 863-870 MHz (SF9 for RX2  - recommended)

## Ajouter un Node : 
choisir l'application vini-meteo (elle doit exister apres avoir rejoins l'organisation vitilab-chambragri).
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
          data.temperature_cuve_celsius = input.bytes[1] + input.bytes[2]/100;
          data.battery_pourcent = input.bytes[3] + input.bytes[4]/100;
        }
        else if (input.bytes[0] == 2) { 
          //data.counter = (input.bytes[0] << 8) + input.bytes[1];
          data.air_humidity_pourcent= input.bytes[1] + input.bytes[2]/100;
          data.air_temperature_Celsius  = input.bytes[3] + input.bytes[4]/100;
          data.battery_pourcent = input.bytes[5] + input.bytes[6]/100;
        }
        else if (input.bytes[0] == 3) {
          data.sol_humidity_pourcent= input.bytes[1] + input.bytes[2]/100;
          data.battery_pourcent = input.bytes[3] + input.bytes[4]/100;
        }
        else if (input.bytes[0] == 4) {
          data.sol_temperature_Celsius = input.bytes[1] + input.bytes[2]/100;
          data.air_humidity_pourcent = input.bytes[3] + input.bytes[4]/100;
          data.air_temperature_Celsius = input.bytes[5] + input.bytes[6]/100;
          data.sol_humidity_pourcent = input.bytes[7] + input.bytes[8]/100;
          data.battery_pourcent = input.bytes[9] + input.bytes[10]/100;
        }
        else if (input.bytes[0] == 5) {
          data.distance_cm = input.bytes[1] + input.bytes[2]/100;
          data.remplissage_pourcent = input.bytes[3] + input.bytes[4]/100;
          data.battery_pourcent = input.bytes[5] + input.bytes[10]/100;
        }
        else if (input.bytes[0] == 6) {
          data.conso_kwh = input.bytes[1] + input.bytes[2]/100;
          data.battery_pourcent = input.bytes[3] + input.bytes[4]/100;
        }
        else if (input.bytes[0] == 7) {
          data.pluie_mm = input.bytes[1] + input.bytes[2]/100;
          data.battery_pourcent = input.bytes[3] + input.bytes[4]/100;
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

Renseigner les clés qui doivent correspondre à ce qu'on a sur ttn concernant le end device :

    const char* devEui = "70B3D57ED0054974"; // Change to TTN Device EUI
    const char* appEui = "0000000000000000"; // Change to TTN Application EUI
    const char* appKey = "ED52F1573C19890A23FE023A273A7D3B"; // Chaneg to TTN Application Key

Ce fichier doit avoir la forme :

    /*****************************************
    * 
    * CHANGER LES VALEURS SUIVANTES selon la configuration !!!!
    * 
    *****************************************/
    
    // identifiant du node TTN
    const char* devEui = "70B3D57ED005E53A"; // Change to TTN Device EUI
    const char* appEui = "0000000000000000"; // Change to TTN Application EUI
    const char* appKey = "C082FB519D4C9EDE56A382B2DA03B201"; // Chaneg to TTN Application Key
    
    // sélection du capteur connecté (uncomment le type en supprimant // et commenter les autres avec // (Ctrl+/) )
    //#define VITI_TYPE 1 // si VINIBOX (température cuve): Sonde DS18B20 --> 1
    //#define VITI_TYPE 2 // si VITINODE (station météo T°/hygro air): sonde DHT22 --> 2
    //#define VITI_TYPE 3 // si VITISOL (hygro du sol): sonde capa --> 3 
    //#define VITI_TYPE 4 // si VITINODEXTEND (station météo T°/hygro air+sol): sonde capa + DS18B20 + DHT22 --> 4 
    //#define VITI_TYPE 5 // si AGRISONAR (niveau d'eau) : sonde HCSR04 --> 5
    //#define VITI_TYPE 6 // si VINIPOWER (compteur d'énergie) : EASTRON SDM530 --> 6
    //#define VITI_TYPE 7 // si VITIPLUIE (pluviomètre): MS-WH-SP-RG --> 7
    #define VITI_TYPE 5
    
    // Temps entre les mesures (en secondes)
    const unsigned long secs_between_send = 60*0.25;
    
    // Coefficient de la batterie (il faut bien prendre 2 résistances identiques pour avoir E/2 en pin 36)
    //avec E la tension délivrée par la batterie, le pont diviseur de tension (pdt) donne  E * R1/(R1+R2)=E *(R1/2R1)=E/2 au pin 36 si les R sont =
    const double batt_coeff = 0.0019; // ( ((0,95/1000)*2*(E/2))  ) // si les resistance sont identiques et (0.95/1000)*2 facteur correctif de l'imprécision des résistances et corriger pdt et passer de mV en V
    //pour l'avoir en %, sur batterie lipo, car 4.2V la batterie au max=100%; Emin=3.2V; E V délivré = X% et on a E/2 au pin 36 si R= -> produit en croix 
     
    /*****************************************
    * 
    * FIN des valeurs à modifier !
    * 
    *****************************************/


## test de compilation :
Cliquer sur build (dans VS code, symbole valider dans la barre du bas)

    si besoin, supprimer le fichier : .pio/libdepds/ttgo-lora-v32v1/TTTN_esp32/src/TTN_BLE_ESP32.cpp
                                      .pio/libdepds/ttgo-lora-v32v1/TTTN_esp32/src/TTN_BLE_ESP32.h

La réussite du build permet d'être sur que le programme s'execute correctement

## upload - programmation de l'esp32

Cliquer sur la flèche - upload (barre du bas)

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


## TEST DE PORTEE
### Allumer la gateway et le node
    test proche de la gateway avec un smartphone sur https://iot.protechebdo.fr ou ttn qu'on recoit bien une donnée
### S'éloigner et régulierement tester que le lien fonctionne encore
    Appuyer sur le boutton reset de la station
    actualiser la page https://iot.protechebdo.fr (ça peux prendre 1 minute avant de recevoir une donnée à jour)


